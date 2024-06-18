#include "VertexResource.h"
#include <cassert>

void VertexResource::Initialize(ID3D12Device* device)
{
	//実際に頂点リソースを作る(球体)
	vertexResource = CreateBufferResource(device, sizeof(VertexData) * 1536);
	//実際に頂点リソースを作る(グリッド)
	vertexResourceGrid = CreateBufferResource(device, sizeof(VertexData) * 3721);
	indexResourceGrid = CreateBufferResource(device, sizeof(VertexData) * 21600);
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
	//球体用
	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResource = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//今回は白を書き込んでいく
	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->enableLighting = true;
	materialData->uvTransform = MakeIdentity4x4();
	//グリッド用
	//マテリアル用のリソースを作る。今回はcolor1つ分のサイズを用意する
	materialResourceGrid = CreateBufferResource(device, sizeof(Material));
	//書き込むためのアドレスを取得
	materialResourceGrid->Map(0, nullptr, reinterpret_cast<void**>(&materialDataGrid));
	//今回は白を書き込んでいく
	materialDataGrid->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialDataGrid->enableLighting = true;
	materialDataGrid->uvTransform = MakeIdentity4x4();

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
	//グリッド用
	wvpResourceGrid = CreateBufferResource(device, sizeof(Matrix4x4));
	//Sprite用
	transformationMatrixResourceSprite = CreateBufferResource(device, sizeof(Matrix4x4));
	//データを書き込む
	//書き込むためのアドレスを取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	wvpResourceGrid->Map(0, nullptr, reinterpret_cast<void**>(&wvpDataGrid));
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	//単位行列を書き込んでおく
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();
	wvpDataGrid->WVP = MakeIdentity4x4();
	wvpDataGrid->World = MakeIdentity4x4();
	transformationMatrixDataSprite->WVP = MakeIdentity4x4();
	transformationMatrixDataSprite->World = MakeIdentity4x4();


	// 頂点バッファビューの設定
	vertexBufferViewGrid.BufferLocation = vertexResourceGrid->GetGPUVirtualAddress();
	vertexBufferViewGrid.SizeInBytes = sizeof(VertexData) * 3721;
	vertexBufferViewGrid.StrideInBytes = sizeof(VertexData);

	// インデックスバッファビューの設定
	indexBufferViewGrid.BufferLocation = indexResourceGrid->GetGPUVirtualAddress();
	indexBufferViewGrid.SizeInBytes = sizeof(uint32_t) * 21600;
	indexBufferViewGrid.Format = DXGI_FORMAT_R32_UINT;

	// グリッドの頂点データを作成
	CreateGrid();

	//波の初期化
	const int gridSize = 61;
	for (int z = 0; z < gridSize; ++z) {
		for (int x = 0; x < gridSize; ++x) {
			int index = z * gridSize + x;
			waveAmplitude[index] = 0.0f; // 波の振幅を初期化
			waveTime[index] = -1.0f; // 波の時間を初期化
		}
	}
	wavePropagationSpeed = 3.0f;
	waveAttenuationValue = 0.3f;
	//球体の速度
	sphereVelocity = { 0.0f, 0.0f, 0.0f };
	sphereAcceleration = { 0.0f, -0.01f, 0.0f };
}

void VertexResource::CreateGrid()
{
	const int gridSize = 61;

	vertexResourceGrid->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataGrid));

	for (int z = 0; z < gridSize; ++z) {
		for (int x = 0; x < gridSize; ++x) {
			int index = z * gridSize + x;
			vertexDataGrid[index].position = { (x - gridSize / 2.0f) * 0.1f, -1.0f, (z - gridSize / 2.0f) * 0.1f, 1.0f };
			vertexDataGrid[index].normal = { 0.0f, 1.0f, 0.0f }; // 法線は上向き
			vertexDataGrid[index].texcoord = { x / (float)(gridSize - 1), 1.0f - z / (float)(gridSize - 1) };
		}
	}

	indexResourceGrid->Map(0, nullptr, reinterpret_cast<void**>(&indexDataGrid));

	int idx = 0;
	for (int z = 0; z < gridSize - 1; ++z) {
		for (int x = 0; x < gridSize - 1; ++x) {
			uint32_t topLeft = z * gridSize + x;
			uint32_t topRight = topLeft + 1;
			uint32_t bottomLeft = (z + 1) * gridSize + x;
			uint32_t bottomRight = bottomLeft + 1;

			indexDataGrid[idx++] = topLeft;
			indexDataGrid[idx++] = bottomLeft;
			indexDataGrid[idx++] = topRight;

			indexDataGrid[idx++] = topRight;
			indexDataGrid[idx++] = bottomLeft;
			indexDataGrid[idx++] = bottomRight;
		}
	}
}

void VertexResource::Update()
{
	//カメラのワールド座標を取得
	Matrix4x4 cameraMatrix = MakeAfineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);

	//球体の変換
	transform.scale.y = transform.scale.x;
	transform.scale.z = transform.scale.x;
	Matrix4x4 worldMatrix = MakeAfineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewMatrix = Multiply(worldMatrix, viewMatrix); // カメラから見たワールド座標に変換
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldViewMatrix, projectionMatrix); // 射影行列を適用してワールドビュープロジェクション行列を計算
	wvpData->WVP = worldViewProjectionMatrix; // 球体のワールドビュープロジェクション行列を更新
	wvpData->World = worldViewMatrix; // 球体のワールド座標行列を更新

	//グリッドの変換
	Matrix4x4 worldMatrixGrid = MakeAfineMatrix(transformGrid.scale, transformGrid.rotate, transformGrid.translate);
	Matrix4x4 worldViewMatrixGrid = Multiply(worldMatrixGrid, viewMatrix); // カメラから見たワールド座標に変換
	wvpDataGrid->WVP = Multiply(worldViewMatrixGrid, projectionMatrix); // 射影行列を適用してワールドビュープロジェクション行列を計算
	wvpDataGrid->World = worldViewMatrixGrid; // グリッドのワールド座標行列を更新

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

	directionalLightData->direction = Normalize(directionalLightData->direction);

	//球の落下の処理
	if (sphereFalling) {
		sphereVelocity = Add(sphereVelocity, sphereAcceleration);
		transform.translate = Add(transform.translate, sphereVelocity);
	}
	//球とグリッドが当たってるなら波の初期化
	if (onCollision(transform, vertexDataGrid)) {
		//materialDataGrid->color = Vector4{ 1.0f,0.0f,0.0f,1.0f };
		if (!waveInitialize) {
			GenerateWave(transform, vertexDataGrid);
		}
		waveInitialize = true;
	}
	else {
		//materialDataGrid->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
		waveInitialize = false;
	}

	// 波の減衰処理と伝播処理
	const int gridSize = 61;
	for (int z = 0; z < gridSize; ++z) {
		for (int x = 0; x < gridSize; ++x) {
			int index = z * gridSize + x;
			waveTime[index] += 0.1f; // 時間経過の更新
			if (waveTime[index] >= 0) {
				float distance = waveTime[index] * wavePropagationSpeed; // 波の伝播距離
				// この波を減衰させていきたいのでstd::expという関数を利用する⤵
				float waveHeight = waveAmplitude[index] * std::exp(-distance * waveAttenuationValue) * -std::sinf(distance * pi); // 減衰した波の高さ
				// 上で出した波の高さをグリッドのy座標に加算していく
				vertexDataGrid[index].position.y = -1.0f + waveHeight; // 高さの更新
			}
		}
	}

	if (transform.translate.y < -10.0f) {
		transform.translate.y = 0.5f;
		sphereVelocity = { 0.0f,0.0f,0.0f };
		sphereFalling = false;
	}
	if (gridInitialize) {
		for (int z = 0; z < gridSize; ++z) {
			for (int x = 0; x < gridSize; ++x) {
				int index = z * gridSize + x;
				vertexDataGrid[index].position = { (x - gridSize / 2.0f) * 0.1f, -1.0f, (z - gridSize / 2.0f) * 0.1f, 1.0f };
				waveAmplitude[index] = 0.0f; // 波の振幅を初期化
				waveTime[index] = -20.0f; // 波の時間を初期化（-1は未初期化を意味する）
			}
		}
		wavePropagationSpeed = 3.0f;
		waveAttenuationValue = 0.3f;
		gridInitialize = false;
	}

}

bool VertexResource::onCollision(Transform& Sphere, VertexData* Grid)
{
	const int gridSize = 61;
	//グリッドの各セルの間隔
	const float kGridEvery = 0.1f;

	for (int z = 0; z < gridSize; ++z) {
		for (int x = 0; x < gridSize; ++x) {
			int index = z * gridSize + x;

			// 各セルの中心位置を計算
			Vector3 cellCenter = {
				kGridEvery * (x - (gridSize / 2)),
				Grid[index].position.y,
				kGridEvery * (z - (gridSize / 2)),
			};

			// 平面の法線と距離を計算
			float distance = std::fabs(Dot(Subtract(Sphere.translate, cellCenter), Grid[index].normal));

			// 球の半径以下なら当たり判定あり
			if (distance <= Sphere.scale.y) {
				return true;
			}
		}
	}

	// どのセルとも当たらない場合
	return false;
}

void VertexResource::GenerateWave(Transform& Sphere, VertexData* Grid)
{
	const int gridSize = 61;
	// 接触した地点を取得
	Vector3 collisionPoint = Subtract(Sphere.translate, Vector3{ 0, Sphere.scale.y - sphereVelocity.y, 0 });

	// 影響範囲内の各グリッド点の高さを更新
	for (int z = 0; z < gridSize; ++z) {
		for (int x = 0; x < gridSize; ++x) {
			int index = z * gridSize + x;
			// 各グリッド点と接触地点との距離を計算
			float distanceToCollision = Distance(Grid[index].position, collisionPoint);
			// 減衰係数の計算（距離が増えると振幅が減少）
			float attenuation = std::exp(-distanceToCollision * waveAttenuationValue);

			// 各グリッドの座標の波の振幅の初期化
			// 波の振幅と時間を設定
			waveAmplitude[index] = -std::sinf((distanceToCollision / Sphere.scale.y / 2.0f) * pi)
				* (Sphere.scale.y + std::fabsf(sphereVelocity.y)) * attenuation; // 波の振幅
			waveTime[index] = (-distanceToCollision / Sphere.scale.y) * (Sphere.scale.y / 2.0f) + std::fabs(sphereVelocity.y); // 波の伝播時間をリセット
		}
	}
}

void VertexResource::ImGui(bool& useMonsterBall)
{
	ImGui::Begin("Sphere");
	ImGui::ColorEdit3("Color", (float*)&materialData->color.x);
	ImGui::DragFloat("Scale", &transform.scale.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transform.translate.x, 0.01f);
	ImGui::Checkbox("sphereFalling", &sphereFalling);
	ImGui::DragFloat("Velocity", &sphereVelocity.y,0.01f);
	ImGui::Checkbox("useMonsterBall", &useMonsterBall);
	/*ImGui::ColorEdit3("LightColor", (float*)&directionalLightData->color.x);
	ImGui::DragFloat3("DirectionalLightData.Direction", &directionalLightData->direction.x, 0.01f);*/
	ImGui::End();

	ImGui::Begin("Grid");
	ImGui::ColorEdit3("GridColor", (float*)&materialDataGrid->color.x);
	ImGui::DragFloat3("GridScale", &transformGrid.scale.x, 0.01f);
	ImGui::DragFloat3("GridRotate", &transformGrid.rotate.x, 0.01f);
	ImGui::DragFloat3("GridTranslate", &transformGrid.translate.x, 0.01f);
	ImGui::SliderFloat("waveSpeed", &wavePropagationSpeed, 0.01f, 10.0f, "%.2f");
	ImGui::SliderFloat("waveAttenuation", &waveAttenuationValue, 0.0f, 1.0f, "%.2f");
	ImGui::Checkbox("gridInitialize", &gridInitialize);
	ImGui::End();

	/*ImGui::Begin("Sprite");
	ImGui::DragFloat3("Scale", &transformSprite.scale.x, 0.01f);
	ImGui::DragFloat3("Rotate", &transformSprite.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &transformSprite.translate.x, 5.0f);
	ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
	ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
	ImGui::End();*/

	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraRotate", &cameraTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("CameraTranslate", &cameraTransform.translate.x, 0.01f);
	ImGui::End();
}

void VertexResource::Release()
{
	vertexResource->Release();
	vertexResourceGrid->Release();
	indexResourceGrid->Release();
	vertexResourceSprite->Release();
	indexResourceSprite->Release();
	directionalLightResource->Release();

	materialResource->Release();
	materialResourceGrid->Release();
	materialResourceSprite->Release();

	wvpResource->Release();
	wvpResourceGrid->Release();
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