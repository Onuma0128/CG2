#pragma once
#include "LoadTexture.h"
#include "DescriptorHeap.h"
#include "DepthStencilTexture.h"
#include "MT3.h"

class TextureResource
{
public:
	void Initialize(ID3D12Device* device,ID3D12DescriptorHeap* srvDescriptorHeap, const uint32_t& descriptorSizeSRV);

	bool& GetuseMonsterBall() { return useMonsterBall; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return textureSrvHandleGPU; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU2() { return textureSrvHandleGPU2; }

	void Release();

private:
	ID3D12Resource* textureResource = nullptr;
	ID3D12Resource* textureResource2 = nullptr;
	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU{};
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2{};
	//texture切り替え用のフラグ
	bool useMonsterBall = true;
};

