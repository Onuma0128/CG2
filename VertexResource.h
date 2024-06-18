#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "MT3.h"
#include "LoadObjFile.h"

ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

class VertexResource
{
public:
	void Initialize(ID3D12Device* device);
	void CreateGrid();

	void Update();
	
	bool onCollision(Transform& Sphere, VertexData* Grid);

	void GenerateWave(Transform& Sphere, VertexData* Grid);

	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return vertexBufferView; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewGrid() { return vertexBufferViewGrid; }
	D3D12_INDEX_BUFFER_VIEW& GetIndexBufferViewGrid() { return indexBufferViewGrid; }

	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewSprite() { return vertexBufferViewSprite; }
	D3D12_INDEX_BUFFER_VIEW& GetIndexBufferViewSprite() { return indexBufferViewSprite; }
	ID3D12Resource* GetDirectionalLightResource() { return directionalLightResource; }

	ID3D12Resource* GetMaterialResource() { return materialResource; }
	ID3D12Resource* GetMaterialResourceGrid() { return materialResourceGrid; }
	ID3D12Resource* GetMaterialResourceSprite() { return materialResourceSprite; }

	ID3D12Resource* GetwvpResource() { return wvpResource; }
	ID3D12Resource* GetwvpResourceGrid() { return wvpResourceGrid; }
	TransformationMatrix* GetwvpDataGrid() { return wvpDataGrid; }
	ID3D12Resource* GetTransformationMatrixResourceSprite() { return transformationMatrixResourceSprite; }

	void ImGui(bool& useMonsterBall);

	void Release();
private:
	//実際に頂点リソースを作る(球体)
	ID3D12Resource* vertexResource = nullptr;
	//実際に頂点リソースを作る(グリッド)
	ID3D12Resource* vertexResourceGrid = nullptr;
	ID3D12Resource* indexResourceGrid = nullptr;
	//Sprite用の頂点リソースを作る
	ID3D12Resource* vertexResourceSprite = nullptr;
	ID3D12Resource* indexResourceSprite = nullptr;
	//平行光源用のリソースを作る
	ID3D12Resource* directionalLightResource = nullptr;
	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewGrid{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewGrid{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
	D3D12_VERTEX_BUFFER_VIEW directionalLightBufferView{};
	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	VertexData* vertexDataGrid = nullptr;
	uint32_t* indexDataGrid = nullptr;
	VertexData* vertexDataSprite = nullptr;
	uint32_t* indexDataSprite = nullptr;
	DirectionalLight* directionalLightData = nullptr;
	//分割数
	uint32_t vertexCount = 16;
	//マテリアル用のリソースを作る
	ID3D12Resource* materialResource = nullptr;
	ID3D12Resource* materialResourceGrid = nullptr;
	ID3D12Resource* materialResourceSprite = nullptr;
	//マテリアルにデータを書き込む
	Material* materialData = nullptr;
	Material* materialDataGrid = nullptr;
	Material* materialDataSprite = nullptr;
	//WVP用のリソースを作る
	ID3D12Resource* wvpResource = nullptr;
	//グリッド用
	ID3D12Resource* wvpResourceGrid = nullptr;
	//Sprite用
	ID3D12Resource* transformationMatrixResourceSprite = nullptr;
	//データを書き込む
	TransformationMatrix* wvpData = nullptr;
	TransformationMatrix* wvpDataGrid = nullptr;
	TransformationMatrix* transformationMatrixDataSprite = nullptr;

	//Transform変数を作る
	//球体
	Transform transform{ {0.3f,0.3f,0.3f},{0.0f,0.0f,0.0f},{0.0f,0.5f,0.0f} };
	Vector3 sphereVelocity{};
	Vector3 sphereAcceleration{};
	//球体の落下フラグ
	bool sphereFalling = false;
	//グリッド
	Transform transformGrid{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//Camera変数を作る
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.26f,0.0f,0.0f},{0.0f,1.7f,-10.0f} };
	//UVTransform変数
	Transform uvTransformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Matrix4x4 uvTransformMatrix{};

	//波の処理に必要な変数
	float waveAmplitude[3721]; // 波の振幅を保存する配列
	float waveTime[3721]; // 波の伝播時間を保存する配列
	float wavePropagationSpeed; // 波の伝播速度
	float waveAttenuationValue; // 波の減衰値
	bool waveInitialize = false;
	bool gridInitialize = false;
};

