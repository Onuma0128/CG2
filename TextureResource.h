#pragma once
#include "LoadTexture.h"
#include "DescriptorHeap.h"
#include "DepthStencilTexture.h"

class TextureResource
{
public:
	void Initialize(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap, const uint32_t& descriptorSizeSRV);

	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return currentTextureSrvHandleGPU; }

	void ImGui();

	void Release();

private:
	DirectX::ScratchImage mipImages;
	DirectX::ScratchImage mipImages2;
	DirectX::ScratchImage mipImages3;
	ID3D12Resource* textureResource = nullptr;
	ID3D12Resource* textureResource2 = nullptr;
	ID3D12Resource* textureResource3 = nullptr;
	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc3{};
	//SRVを制作するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU{};
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2{};
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU3{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU3{};
	D3D12_GPU_DESCRIPTOR_HANDLE currentTextureSrvHandleGPU{};
};