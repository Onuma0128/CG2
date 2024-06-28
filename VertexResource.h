#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "MT3.h"
#include "LoadObjFile.h"
#include "wrl.h"

using Microsoft::WRL::ComPtr;

ComPtr<ID3D12Resource> CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes);

class VertexResource
{
public:
	void Initialize(ComPtr<ID3D12Device> device);

	void Update();
	ModelData& GetModelData() { return modelData; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return vertexBufferView; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewSprite() { return vertexBufferViewSprite; }
	D3D12_INDEX_BUFFER_VIEW& GetIndexBufferViewSprite() { return indexBufferViewSprite; }
	ComPtr<ID3D12Resource> GetDirectionalLightResource() { return directionalLightResource; }
	ComPtr<ID3D12Resource> GetMaterialResource() { return materialResource; }
	ComPtr<ID3D12Resource> GetMaterialResourceSprite() { return materialResourceSprite; }
	ComPtr<ID3D12Resource> GetwvpResource() { return wvpResource; }
	ComPtr<ID3D12Resource> GetTransformationMatrixResourceSprite() { return transformationMatrixResourceSprite; }

	void ImGui(bool& useMonsterBall);

private:
	//モデル読み込み
	ModelData modelData;
	//実際に頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource = nullptr;
	//Sprite用の頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResourceSprite = nullptr;
	ComPtr<ID3D12Resource> indexResourceSprite = nullptr;
	//平行光源用のリソースを作る
	ComPtr<ID3D12Resource> directionalLightResource = nullptr;
	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
	D3D12_VERTEX_BUFFER_VIEW directionalLightBufferView{};
	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	VertexData* vertexDataSprite = nullptr;
	uint32_t* indexDataSprite = nullptr;
	DirectionalLight* directionalLightData = nullptr;
	//分割数
	uint32_t vertexCount = 16;
	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource = nullptr;
	ComPtr<ID3D12Resource> materialResourceSprite = nullptr;
	//マテリアルにデータを書き込む
	Material* materialData = nullptr;
	Material* materialDataSprite = nullptr;
	//WVP用のリソースを作る
	ComPtr<ID3D12Resource> wvpResource = nullptr;
	//Sprite用
	ComPtr<ID3D12Resource> transformationMatrixResourceSprite = nullptr;
	//データを書き込む
	TransformationMatrix* wvpData = nullptr;
	TransformationMatrix* transformationMatrixDataSprite = nullptr;

	//Transform変数を作る
	Transform transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//Camera変数を作る
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
	//UVTransform変数
	Transform uvTransformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Matrix4x4 uvTransformMatrix{};
};

