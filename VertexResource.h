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

	ModelData& GetModelData() { return modelData_; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() { return vertexBufferView_; }
	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewSphere() { return vertexBufferViewSphere_; }

	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewGrid() { return vertexBufferViewGrid_; }
	D3D12_INDEX_BUFFER_VIEW& GetIndexBufferViewGrid() { return indexBufferViewGrid_; }

	D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferViewSprite() { return vertexBufferViewSprite_; }
	D3D12_INDEX_BUFFER_VIEW& GetIndexBufferViewSprite() { return indexBufferViewSprite_; }
	ComPtr<ID3D12Resource> GetDirectionalLightResource() { return directionalLightResource_; }

	///=============================================================================================

	ComPtr<ID3D12Resource> GetMaterialResource() { return materialResource_; }
	ComPtr<ID3D12Resource> GetMaterialResourceSphere() { return materialResourceSphere_; }
	ComPtr<ID3D12Resource> GetMaterialResourceGrid() { return materialResourceGrid_; }
	ComPtr<ID3D12Resource> GetMaterialResourceSprite() { return materialResourceSprite_; }
	ComPtr<ID3D12Resource> GetwvpResource() { return wvpResource_; }
	ComPtr<ID3D12Resource> GetwvpResourceSphere() { return wvpResourceSphere_; }
	ComPtr<ID3D12Resource> GetwvpResourceGrid() { return wvpResourceGrid_; }
	ComPtr<ID3D12Resource> GetTransformationMatrixResourceSprite() { return transformationMatrixResourceSprite_; }

	void Update();

	void ImGui(bool& useMonsterBall);

private:
	//モデル読み込み
	ModelData modelData_;
	//実際に頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	//Sphere用の頂点リソース
	ComPtr<ID3D12Resource> vertexResourceSphere_ = nullptr;
	//Grid用の頂点リソース
	ComPtr<ID3D12Resource> vertexResourceGrid_ = nullptr;
	ComPtr<ID3D12Resource> indexResourceGrid_ = nullptr;
	//Sprite用の頂点リソースを作る
	ComPtr<ID3D12Resource> vertexResourceSprite_ = nullptr;
	ComPtr<ID3D12Resource> indexResourceSprite_ = nullptr;
	//平行光源用のリソースを作る
	ComPtr<ID3D12Resource> directionalLightResource_ = nullptr;

	///=============================================================================================

	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	//Obj
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//Sphere
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere_{};
	//Grid
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewGrid_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewGrid_{};
	//Sprite
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite_{};
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite_{};
	//Light
	D3D12_VERTEX_BUFFER_VIEW directionalLightBufferView_{};

	///=============================================================================================

	//頂点リソースにデータを書き込む
	//Obj
	VertexData* vertexData_ = nullptr;
	//Sphere
	VertexData* vertexDataSphere_ = nullptr;
	//Grid
	VertexData* vertexDataGrid_ = nullptr;
	uint32_t* indexDataGrid_ = nullptr;
	//Sprite
	VertexData* vertexDataSprite_ = nullptr;
	uint32_t* indexDataSprite_ = nullptr;
	//Light
	DirectionalLight* directionalLightData_ = nullptr;
	//分割数(球体)
	uint32_t vertexCount_ = 16;

	///=============================================================================================

	//マテリアル用のリソースを作る
	ComPtr<ID3D12Resource> materialResource_ = nullptr;
	ComPtr<ID3D12Resource> materialResourceSphere_ = nullptr;
	ComPtr<ID3D12Resource> materialResourceGrid_ = nullptr;
	ComPtr<ID3D12Resource> materialResourceSprite_ = nullptr;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;
	Material* materialDataSphere_ = nullptr;
	Material* materialDataGrid_ = nullptr;
	Material* materialDataSprite_ = nullptr;

	///=============================================================================================

	//WVP用のリソースを作る
	//Obj用
	ComPtr<ID3D12Resource> wvpResource_ = nullptr;
	//Sphere用
	ComPtr<ID3D12Resource> wvpResourceSphere_ = nullptr;
	//Grid用
	ComPtr<ID3D12Resource> wvpResourceGrid_ = nullptr;
	//Sprite用
	ComPtr<ID3D12Resource> transformationMatrixResourceSprite_ = nullptr;
	//データを書き込む
	TransformationMatrix* wvpData_ = nullptr;
	TransformationMatrix* wvpDataSphere_ = nullptr;
	TransformationMatrix* wvpDataGrid_ = nullptr;
	TransformationMatrix* transformationMatrixDataSprite_ = nullptr;

	///=============================================================================================

	//Transform変数を作る
	Transform transform_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{2.0f,0.0f,0.0f} };
	Transform transformSphere_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{-2.0f,0.0f,0.0f} };
	Transform transformGrid_{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,-0.5f,0.0f} };
	Transform transformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//Camera変数を作る
	Transform cameraTransform{ {1.0f,1.0f,1.0f},{0.25f,0.0f,0.0f},{0.0f,2.5f,-10.0f} };
	//UVTransform変数
	Transform uvTransformSprite{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Matrix4x4 uvTransformMatrix{};
};

