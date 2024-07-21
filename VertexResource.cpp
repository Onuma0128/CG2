#include "VertexResource.h"
#include <cassert>

void VertexResource::Initialize(ComPtr<ID3D12Device> device)
{
	modelData_ = LoadObjFile("resources", "plane.obj");
	//modelData_ = LoadObjFile("resources", "bunny.obj");
	//実際に頂点リソースを作る
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * modelData_.vertices.size());
	//Sprite用の頂点リソースを作る
	vertexResourceSprite_ = CreateBufferResource(device, sizeof(VertexData) * 4);
	indexResourceSprite_ = CreateBufferResource(device, sizeof(uint32_t) * 6);
	//平行光源用のリソースを作る
	directionalLightResource_ = CreateBufferResource(device, sizeof(DirectionalLight));
	//Instancing用
	instancingResource_ = CreateBufferResource(device, sizeof(ParticleForGPU) * kNumMaxInstance);

	///=============================================================================================================

	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite_->GetGPUVirtualAddress();
	indexBufferViewSprite_.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();
	directionalLightBufferView_.BufferLocation = directionalLightResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 4;
	indexBufferViewSprite_.SizeInBytes = sizeof(uint32_t) * 6;
	directionalLightBufferView_.SizeInBytes = sizeof(DirectionalLight);
	//頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);
	indexBufferViewSprite_.Format = DXGI_FORMAT_R32_UINT;
	directionalLightBufferView_.StrideInBytes = sizeof(DirectionalLight);

	///=============================================================================================================

	//Resourceにデータを書き込む
	//書き込むためのアドレスを取得
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//Objの頂点
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
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

	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	}

	///=============================================================================================================

	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource_ = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//今回は白を書き込んでいく
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = true;
	materialData_->uvTransform = MakeIdentity4x4();
	//Sprite用のマテリアルリソースを作る
	materialResourceSprite_ = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite_));
	//今回は白を書き込んでいく
	materialDataSprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataSprite_->enableLighting = false;
	materialDataSprite_->uvTransform = MakeIdentity4x4();

	///=============================================================================================================

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	wvpResource_ = CreateBufferResource(device, sizeof(Matrix4x4));
	//Sprite用
	transformationMatrixResourceSprite_ = CreateBufferResource(device, sizeof(Matrix4x4));
	//データを書き込む
	wvpData_ = nullptr;
	transformationMatrixDataSprite_ = nullptr;
	//書き込むためのアドレスを取得
	wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
	transformationMatrixResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite_));
	//単位行列を書き込んでおく
	wvpData_->WVP = MakeIdentity4x4();
	wvpData_->World = MakeIdentity4x4();
	transformationMatrixDataSprite_->WVP = MakeIdentity4x4();
	transformationMatrixDataSprite_->World = MakeIdentity4x4();

	std::mt19937 randomEngine_(seedGenerator_());

	emitter_.transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	emitter_.frequency = 0.5f;
	emitter_.frequencyTime = 0.0f;
	// emitter_.frequencyごとに出すパーティクルの個数
	emitter_.count = 3;
	particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));

	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData_[index].WVP = MakeIdentity4x4();
		instancingData_[index].World = MakeIdentity4x4();
	}
	moveStart = false;
}

void VertexResource::Update()
{
	//カメラのワールド座標を取得
	Matrix4x4 cameraMatrix = MakeAfineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);

	// エミッターによるパーティクルの発生
	if (moveStart) {
		emitter_.frequencyTime += kDeltaTime;
	}
	if (emitter_.frequency <= emitter_.frequencyTime) {
		std::mt19937 randomEngine_(seedGenerator_());
		particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));
		emitter_.frequencyTime -= emitter_.frequency;
	}

	// パーティクルの更新
	numInstance = 0;
	for (std::list<Particle>::iterator particleIterator = particles_.begin();
		particleIterator != particles_.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
		Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, cameraMatrix);
		billboardMatrix.m[3][0] = 0.0f; billboardMatrix.m[3][1] = 0.0f; billboardMatrix.m[3][2] = 0.0f;
		Matrix4x4 worldMatrix = MakeScaleMatrix(particleIterator->transform.scale) * billboardMatrix * MakeTranslateMatrix(particleIterator->transform.translate);
		Matrix4x4 worldViewMatrix = Multiply(worldMatrix, viewMatrix);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix);
		//パーティクルの動き
		if (moveStart) {
			particleIterator->transform.translate = particleIterator->velocity * kDeltaTime + particleIterator->transform.translate;
			particleIterator->currentTime += kDeltaTime;
		}
		float alpha = 1.0f - (particleIterator->currentTime / particleIterator->lifeTime);
		if (numInstance < kNumMaxInstance) {
			instancingData_[numInstance].WVP = worldViewProjectionMatrix;
			instancingData_[numInstance].World = worldViewMatrix;
			instancingData_[numInstance].color = particleIterator->color;
			instancingData_[numInstance].color.w = alpha;
			++numInstance;
		}
		++particleIterator;
	}

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
	ImGui::Begin("Sprite");
	ImGui::DragFloat3("Scale", &transformSprite.scale.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transformSprite.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transformSprite.translate.x, 5.0f);
	ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("cameraRotate", &cameraTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("cameraTranslate", &cameraTransform.translate.x, 0.01f);
	ImGui::ColorEdit4("LightColor", (float*)&directionalLightData_->color.x);
	ImGui::DragFloat3("DirectionalLightData.Direction", &directionalLightData_->direction.x, 0.01f);
	ImGui::DragFloat("Intensity", &directionalLightData_->intensity, 0.01f);
	ImGui::End();

	uint32_t min = 0;uint32_t max = 20;
	ImGui::Begin("Settings");
	ImGui::SliderScalar("Emitter_count", ImGuiDataType_U32, &emitter_.count, &min, &max);
	ImGui::DragFloat3("EmitterTranslate", &emitter_.transform.translate.x, 0.01f, -100.0f, 100.0f);
	ImGui::ColorEdit4("Color", (float*)&materialData_->color.x);
	ImGui::Checkbox("circle", &useMonsterBall);
	ImGui::Checkbox("move", &moveStart);
	ImGui::Text("%d", numInstance);
	if (ImGui::Button("Add Particle")) {
		std::mt19937 randomEngine_(seedGenerator_());
		particles_.splice(particles_.end(), Emit(emitter_, randomEngine_));
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
