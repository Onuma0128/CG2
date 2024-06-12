#include "VertexResource.h"
#include <cassert>

void VertexResource::Initialize(ID3D12Device* device)
{
	//実際に頂点リソースを作る
	vertexResource = CreateBufferResource(device, sizeof(VertexData) * 1536);
	//Sprite用の頂点リソースを作る
	vertexResourceSprite = CreateBufferResource(device, sizeof(VertexData) * 4);
	indexResourceSprite = CreateBufferResource(device, sizeof(uint32_t) * 6);
	//平行光源用のリソースを作る
	directionalLightResource = CreateBufferResource(device, sizeof(DirectionalLight));

	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	directionalLightBufferView.BufferLocation = directionalLightResource->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 1536;
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 4;
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	directionalLightBufferView.SizeInBytes = sizeof(DirectionalLight);
	//頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;
	directionalLightBufferView.StrideInBytes = sizeof(DirectionalLight);
	//Resourceにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData = DrawSphere(vertexData, vertexCount);
	//法線情報の追加
	for (uint32_t index = 0; index < 1536; ++index) {
		vertexData[index].normal = Normalize(vertexData[index].position);
	}
	//書き込むためのアドレスを取得
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	//四角形の4つの頂点
	vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f };//左下
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };//右下
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };
	vertexDataSprite[3].position = { 640.0f,0.0f,0.0f,1.0f };//右上
	vertexDataSprite[3].texcoord = { 1.0f,0.0f };
	//法線情報の追加
	vertexDataSprite[0].normal = { 0.0f,0.0f,-1.0f };
	//IndexData
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0; indexDataSprite[1] = 1; indexDataSprite[2] = 2;
	indexDataSprite[3] = 1; indexDataSprite[4] = 3; indexDataSprite[5] = 2;

	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	//デフォルト値はとりあえず以下のようにする
	directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData->direction = { 0.0f,1.0f,-1.0f };
	directionalLightData->direction = Normalize(directionalLightData->direction);
	directionalLightData->intensity = 1.0f;

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//今回は白を書き込んでいく
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->enableLighting = true;
	materialData->uvTransform = MakeIdentity4x4();
	//Sprite用のマテリアルリソースを作る
	materialResourceSprite = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	//今回は白を書き込んでいく
	materialDataSprite->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataSprite->enableLighting = false;
	materialDataSprite->uvTransform = MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource = CreateBufferResource(device, sizeof(Matrix4x4));
	//Sprite用
	transformationMatrixResourceSprite = CreateBufferResource(device, sizeof(Matrix4x4));
	//データを書き込む
	wvpData = nullptr;
	transformationMatrixDataSprite = nullptr;
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	//単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
	transformationMatrixDataSprite->WVP = MakeIdentity4x4();
	transformationMatrixDataSprite->World = MakeIdentity4x4();

	cameraDistance = 10.0f;
	cameraHorizontalAngle = -1.575f;
	cameraVerticalAngle = 0.26f;

	cameraTarget = { 0.0f, 0.0f, 0.0f }; // カメラが見るターゲット（原点）
	up = { 0.0f, 1.0f, 0.0f }; // 上方向
}

void VertexResource::Update()
{
	//Transform変換
	Matrix4x4 worldMatrix = MakeAfineMatrix(transform.scale, transform.rotate, transform.translate);
	wvpData->WVP = worldMatrix;
	wvpData->World = worldMatrix;
	//Sprite用
	Matrix4x4 worldMatrixSprite = MakeAfineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(kClientWidth), float(kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixDataSprite->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixDataSprite->World = worldViewProjectionMatrixSprite;
	//UVの行列を生成
	uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
	materialDataSprite->uvTransform = uvTransformMatrix;
	////Camera変換
	//Matrix4x4 cameraMatrix = MakeAfineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	//Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	//Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	//Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	// カメラ位置の更新（球面座標系から直交座標系への変換）
	cameraTransform.translate.x = cameraDistance * std::cosf(cameraVerticalAngle) * std::cosf(cameraHorizontalAngle);
	cameraTransform.translate.y = cameraDistance * std::sinf(cameraVerticalAngle);
	cameraTransform.translate.z = cameraDistance * std::cosf(cameraVerticalAngle) * std::sinf(cameraHorizontalAngle);

	// ビュー行列の計算
	Matrix4x4 viewMatrix = MakeLookAtMatrix(cameraTransform.translate, cameraTarget, up);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.1f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	//Matrix4x4 cameraWorldMatrix = MakeAfineMatrix(traiangleScale, traiangleRotate, traiangleTranslate);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldViewProjectionMatrix;
	//LightのNormalize
	directionalLightData->direction = Normalize(directionalLightData->direction);
}

void VertexResource::ImGui(bool& useMonsterBall)
{
	ImGui::Begin("Window");
	ImGui::ColorEdit3("Color", (float*)&materialData->color.x);
	ImGui::DragFloat3("Scale", &transform.scale.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
	ImGui::ColorEdit3("LightColor", (float*)&directionalLightData->color.x);
	ImGui::DragFloat3("DirectionalLightData.Direction", &directionalLightData->direction.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Sprite");
	ImGui::DragFloat3("Scale", &transformSprite.scale.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transformSprite.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transformSprite.translate.x, 5.0f);
	ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::Text("cameraPosition.x %f", cameraTransform.translate.x);
	ImGui::DragFloat("CameraDistance", &cameraDistance, 0.01f);
	ImGui::DragFloat("CameraHorizontalAngle", &cameraHorizontalAngle, 0.01f);
	ImGui::DragFloat("CameraVerticalAngle", &cameraVerticalAngle, 0.01f);
	ImGui::End();
}

void VertexResource::Release()
{
	vertexResource->Release();
	vertexResourceSprite->Release();
	indexResourceSprite->Release();
	directionalLightResource->Release();
	materialResource->Release();
	materialResourceSprite->Release();
	wvpResource->Release();
	transformationMatrixResourceSprite->Release();
}

ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
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
