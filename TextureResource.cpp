#include "TextureResource.h"

void TextureResource::Initialize(ComPtr<ID3D12Device>& device, ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, const uint32_t& descriptorSizeSRV)
{
	device_ = device;
	srvDescriptorHeap_ = srvDescriptorHeap;
	descriptorSizeSRV_ = descriptorSizeSRV;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureResource::GetTextureSrvHandleGPU(const std::string& filePath, const uint32_t& index)
{
	DirectX::ScratchImage mipImages = LoadTexture(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource_ = CreateTextureResource(device_, metadata);
	UploadTextureData(textureResource_, mipImages);

	///=============================================================================================================

	//metaDataを基にSRVの設定
	srvDesc_.Format = metadata.format;
	srvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2DTexture
	srvDesc_.Texture2D.MipLevels = UINT(metadata.mipLevels);

	///=============================================================================================================

	//SRVを制作するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = GetCPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, index);
	textureSrvHandleGPU_ = GetGPUDescriptorHandle(srvDescriptorHeap_, descriptorSizeSRV_, index);
	//SRVの生成
	device_->CreateShaderResourceView(textureResource_.Get(), &srvDesc_, textureSrvHandleCPU_);

	return textureSrvHandleGPU_;
}
