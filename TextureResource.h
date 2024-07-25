#pragma once
#include "LoadTexture.h"
#include "DescriptorHeap.h"
#include "DepthStencilTexture.h"
#include "MT3.h"

class TextureResource
{
public:
	void Initialize(ComPtr<ID3D12Device>& device, ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, const uint32_t& descriptorSizeSRV);

	bool& GetuseMonsterBall() { return useMonsterBall; }

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU(const std::string& filePath,const uint32_t& index);

private:
	ComPtr<ID3D12Device> device_ = {};
	ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_{};
	uint32_t descriptorSizeSRV_{};

	ComPtr<ID3D12Resource> textureResource_ = nullptr;

	///=============================================================================================================

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};

	///=============================================================================================================

	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_{};

	///=============================================================================================================

	//texture切り替え用のフラグ
	bool useMonsterBall = true;
};

