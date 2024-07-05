#include "VertexResource.h"
#include <cassert>

void VertexResource::Initialize(ComPtr<ID3D12Device> device)
{
	//modelData_ = LoadObjFile("resources", "plane.obj");
	modelData_ = LoadObjFile("resources", "teapot.obj");
	//modelData_ = LoadObjFile("resources", "bunny.obj");
	//modelData_ = LoadObjFile("resources", "multiMesh.obj");
	
	//実際に頂点リソースを作る
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * modelData_.vertices.size());
	vertexResourceSphere_ = CreateBufferResource(device, sizeof(VertexData) * 1536);
	vertexResourceGrid_ = CreateBufferResource(device, sizeof(VertexData) * 121);
	indexResourceGrid_ = CreateBufferResource(device, sizeof(uint32_t) * 600);
	//Sprite用の頂点リソースを作る
	vertexResourceSprite_ = CreateBufferResource(device, sizeof(VertexData) * 4);
	indexResourceSprite_ = CreateBufferResource(device, sizeof(uint32_t) * 6);
	//平行光源用のリソースを作る
	directionalLightResource_ = CreateBufferResource(device, sizeof(DirectionalLight));

	///=============================================================================================

	//リソースの先頭のアドレスから使う
	//obj
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//sphere
	vertexBufferViewSphere_.BufferLocation = vertexResourceSphere_->GetGPUVirtualAddress();
	//grid
	vertexBufferViewGrid_.BufferLocation = vertexResourceGrid_->GetGPUVirtualAddress();
	indexBufferViewGrid_.BufferLocation = indexResourceGrid_->GetGPUVirtualAddress();
	//sprite
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	indexBufferViewSprite_.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();
	//light
	directionalLightBufferView_.BufferLocation = directionalLightResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	//obj
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	//sphere
	vertexBufferViewSphere_.SizeInBytes = sizeof(VertexData) * 1536;
	//grid
	vertexBufferViewGrid_.SizeInBytes = sizeof(VertexData) * 121;
	indexBufferViewGrid_.SizeInBytes = sizeof(uint32_t) * 600;
	//sprite
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 4;
	indexBufferViewSprite_.SizeInBytes = sizeof(uint32_t) * 6;
	//light
	directionalLightBufferView_.SizeInBytes = sizeof(DirectionalLight);
	//頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	vertexBufferViewSphere_.StrideInBytes = sizeof(VertexData);
	vertexBufferViewGrid_.StrideInBytes = sizeof(VertexData);
	indexBufferViewGrid_.Format = DXGI_FORMAT_R32_UINT;
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;
	directionalLightBufferView_.StrideInBytes = sizeof(DirectionalLight);

	///=============================================================================================

	//Resourceにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	//sphere
	vertexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere_));
	vertexDataSphere_ = DrawSphere(vertexDataSphere_, vertexCount_);
	//法線情報の追加
	for (uint32_t index = 0; index < 1536; ++index) {
		vertexDataSphere_[index].normal = Normalize(vertexDataSphere_[index].position);
	}
	//grid
	vertexResourceGrid_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataGrid_));
	const int gridSize = 11; // 11x11のグリッド
	const float gridSpacing = 4.0f / (gridSize - 1); // グリッドの間隔
	int vertexIndex = 0;
	for (int z = 0; z < gridSize; ++z) {
		for (int x = 0; x < gridSize; ++x) {
			float posX = -2.0f + x * gridSpacing;
			float posZ = -2.0f + z * gridSpacing;

			vertexDataGrid_[vertexIndex].position = { posX, 0.0f, posZ, 1.0f };
			vertexDataGrid_[vertexIndex].texcoord = { x / static_cast<float>(gridSize - 1), z / static_cast<float>(gridSize - 1) };
			vertexDataGrid_[vertexIndex].normal = Vector3{ 0.0f,1.0f,0.0f };
			++vertexIndex;
		}
	}
	indexResourceGrid_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataGrid_));
	int index = 0;
	for (int z = 0; z < gridSize - 1; ++z) {
		for (int x = 0; x < gridSize - 1; ++x) {
			int topLeft = z * gridSize + x;
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + gridSize;
			int bottomRight = bottomLeft + 1;

			// 1つ目の三角形
			indexDataGrid_[index++] = topLeft;
			indexDataGrid_[index++] = bottomLeft;
			indexDataGrid_[index++] = topRight;

			// 2つ目の三角形
			indexDataGrid_[index++] = topRight;
			indexDataGrid_[index++] = bottomLeft;
			indexDataGrid_[index++] = bottomRight;
		}
	}

	//書き込むためのアドレスを取得
	vertexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite_));
	//四角形の4つの頂点
	vertexDataSprite_[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	vertexDataSprite_[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexDataSprite_[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite_[2].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexDataSprite_[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite_[3].position = { 640.0f,0.0f,0.0f,1.0f };//右上
	vertexDataSprite_[3].texcoord = { 1.0f,0.0f };
	//法線情報の追加
	vertexDataSprite_[0].normal = { 0.0f,0.0f,-1.0f };
	//IndexData
	indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite_));
	indexDataSprite_[0] = 0; indexDataSprite_[1] = 1; indexDataSprite_[2] = 2;
	indexDataSprite_[3] = 1; indexDataSprite_[4] = 3; indexDataSprite_[5] = 2;

	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//デフォルト値はとりあえず以下のようにする
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,1.0f,0.0f };
	directionalLightData_->direction = Normalize(directionalLightData_->direction);
	directionalLightData_->intensity = 1.0f;

	///=============================================================================================

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//今回は白を書き込んでいく
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();

	//Sphere用のマテリアルリソースを作る
	materialResourceSphere_ = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere_));
	//今回は白を書き込んでいく
	materialDataSphere_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataSphere_->enableLighting = true;
	materialDataSphere_->uvTransform = MakeIdentity4x4();

	//Grid用のマテリアルリソースを作る
	materialResourceGrid_ = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResourceGrid_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataGrid_));
	//今回は白を書き込んでいく
	materialDataGrid_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataGrid_->enableLighting = true;
	materialDataGrid_->uvTransform = MakeIdentity4x4();

	//Sprite用のマテリアルリソースを作る
	materialResourceSprite_ = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite_));
	//今回は白を書き込んでいく
	materialDataSprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataSprite_->enableLighting = false;
	materialDataSprite_->uvTransform = MakeIdentity4x4();

	///=============================================================================================

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource_ = CreateBufferResource(device, sizeof(Matrix4x4));
	//Sphere用
	wvpResourceSphere_ = CreateBufferResource(device, sizeof(Matrix4x4));
	//Grid用
	wvpResourceGrid_ = CreateBufferResource(device, sizeof(Matrix4x4));
	//Sprite用
	transformationMatrixResourceSprite_ = CreateBufferResource(device, sizeof(Matrix4x4));
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	wvpResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataSphere_));
	wvpResourceGrid_->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataGrid_));
	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	//単位行列を書き込んでおく
	wvpData_->WVP = MakeIdentity4x4();
	wvpData_->World = MakeIdentity4x4();
	wvpDataSphere_->WVP = MakeIdentity4x4();
	wvpDataSphere_->World = MakeIdentity4x4();
	wvpDataGrid_->WVP = MakeIdentity4x4();
	wvpDataGrid_->World = MakeIdentity4x4();
	transformationMatrixDataSprite_->WVP = MakeIdentity4x4();
	transformationMatrixDataSprite_->World = MakeIdentity4x4();
}

void VertexResource::Update()
{
	//カメラのワールド座標を取得
	Matrix4x4 cameraMatrix = MakeAfineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);

	//Transform変換
	Matrix4x4 worldMatrix = MakeAfineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewMatrix = Multiply(worldMatrix, viewMatrix);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix);
	wvpData_->WVP = worldViewProjectionMatrix;
	wvpData_->World = worldViewMatrix;

	//Sphere用
	Matrix4x4 worldMatrixSphere = MakeAfineMatrix(transformSphere_.scale, transformSphere_.rotate, transformSphere_.translate);
	Matrix4x4 worldViewMatrixSphere = Multiply(worldMatrixSphere, viewMatrix);
	Matrix4x4 worldViewProjectionMatrixSphere = Multiply(worldViewMatrixSphere, projectionMatrix);
	wvpDataSphere_->WVP = worldViewProjectionMatrixSphere;
	wvpDataSphere_->World = worldViewMatrixSphere;

	//Grid用
	Matrix4x4 worldMatrixGrid = MakeAfineMatrix(transformGrid_.scale, transformGrid_.rotate, transformGrid_.translate);
	Matrix4x4 worldViewMatrixGrid = Multiply(worldMatrixGrid, viewMatrix);
	Matrix4x4 worldViewProjectionMatrixGrid = Multiply(worldViewMatrixGrid, projectionMatrix);
	wvpDataGrid_->WVP = worldViewProjectionMatrixGrid;
	wvpDataGrid_->World = worldViewMatrixGrid;

	//Sprite用
	Matrix4x4 worldMatrixSprite = MakeAfineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(kClientWidth), float(kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixDataSprite_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixDataSprite_->World = worldViewProjectionMatrixSprite;

	//UVの行列を生成
	uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
	materialDataSprite_->uvTransform = uvTransformMatrix;

	//LightのNormalize
	directionalLightData_->direction = Normalize(directionalLightData_->direction);
}

void VertexResource::ImGui(bool& useMonsterBall)
{
	ImGui::Begin("Object Settings");

	//Object設定
	if (ImGui::CollapsingHeader("General Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Checkbox("Use Monster Ball", &useMonsterBall);
		ImGui::ColorEdit3("Color", (float*)&materialData_->color.x);
		ImGui::Checkbox("Enable Lighting", (bool*)&materialData_->enableLighting);
		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
		ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f);
	}
	//Sphereの設定
	if (ImGui::CollapsingHeader("Sphere Settings")) {
		ImGui::ColorEdit3("Sphere Color", (float*)&materialDataSphere_->color.x);
		ImGui::Checkbox("Enable Sphere Lighting", (bool*)&materialDataSphere_->enableLighting);
		ImGui::DragFloat3("Sphere Scale", &transformSphere_.scale.x, 0.01f);
		ImGui::DragFloat3("Sphere Rotate", &transformSphere_.rotate.x, 0.01f);
		ImGui::DragFloat3("Sphere Translate", &transformSphere_.translate.x, 0.01f);
	}
	//Gridの設定
	if (ImGui::CollapsingHeader("Grid Settings")) {
		ImGui::ColorEdit3("Grid Color", (float*)&materialDataGrid_->color.x);
		ImGui::Checkbox("Enable Grid Lighting", (bool*)&materialDataGrid_->enableLighting);
		ImGui::DragFloat3("Grid Scale", &transformGrid_.scale.x, 0.01f);
		ImGui::DragFloat3("Grid Rotate", &transformGrid_.rotate.x, 0.01f);
		ImGui::DragFloat3("Grid Translate", &transformGrid_.translate.x, 0.01f);
	}

	ImGui::End();

	//Sprite設定
	ImGui::Begin("Sprite Settings");

	if (ImGui::CollapsingHeader("Transform")) {
		ImGui::DragFloat3("Scale", &transformSprite.scale.x, 0.01f);
		ImGui::DragFloat3("Rotate", &transformSprite.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &transformSprite.translate.x, 5.0f);
	}
	if (ImGui::CollapsingHeader("UV Transform")) {
		ImGui::DragFloat2("UV Translate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat2("UV Scale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UV Rotate", &uvTransformSprite.rotate.z);
	}

	ImGui::End();

	//Camera設定
	ImGui::Begin("Camera Settings");

	if (ImGui::CollapsingHeader("Transform")) {
		ImGui::DragFloat3("Rotate", &cameraTransform.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &cameraTransform.translate.x, 0.01f);
	}
	if (ImGui::CollapsingHeader("Lighting")) {
		ImGui::ColorEdit3("Light Color", (float*)&directionalLightData_->color.x);
		ImGui::DragFloat3("Light Direction", &directionalLightData_->direction.x, 0.01f);
	}

	ImGui::End();
}

ComPtr<ID3D12Resource> CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes)
{
	// 頂点リソースのヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties = {};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc = {};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Material) * sizeInBytes;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点リソースを作成する
	ID3D12Resource* vertexResource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexResource));

	assert(SUCCEEDED(hr)); // エラーチェック

	return vertexResource;

}
