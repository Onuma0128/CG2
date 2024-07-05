#pragma once
#include "LoadTexture.h"
#include "DescriptorHeap.h"
#include "DepthStencilTexture.h"
#include "MT3.h"

class TextureResource
{
public:
	void Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, const uint32_t& descriptorSizeSRV);

	bool& GetuseMonsterBall() { return useMonsterBall_; }
	void SetModelData(ModelData modelData);
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return textureSrvHandleGPU_; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU2() { return textureSrvHandleGPU2_; }

private:
	ComPtr<ID3D12Resource> textureResource_ = nullptr;
	ComPtr<ID3D12Resource> textureResource2_ = nullptr;
	ModelData modelData_{};
	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2_{};
	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_{};
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2_{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2_{};
	//texture切り替え用のフラグ
	bool useMonsterBall_ = true;
};

