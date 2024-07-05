#include "TextureResource.h"

void TextureResource::Initialize(ComPtr<ID3D12Device> device, ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, const uint32_t& descriptorSizeSRV)
{
	DirectX::ScratchImage mipImages = LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource_ = CreateTextureResource(device, metadata);
	UploadTextureData(textureResource_, mipImages);
	//metaDataを基にSRVの設定
	srvDesc_.Format = metadata.format;
	srvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2DTexture
	srvDesc_.Texture2D.MipLevels = UINT(metadata.mipLevels);
	//SRVを制作するDescriptorHeapの場所を決める
	textureSrvHandleCPU_ = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU_ = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU_.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU_.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	device->CreateShaderResourceView(textureResource_.Get(), &srvDesc_, textureSrvHandleCPU_);
	//２枚目のTextureを読んで転送する
	DirectX::ScratchImage mipImages2 = LoadTexture(modelData_.material.textureFilePath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	textureResource2_ = CreateTextureResource(device, metadata2);
	UploadTextureData(textureResource2_, mipImages2);
	//metaDataを基にSRVの設定
	srvDesc2_.Format = metadata2.format;
	srvDesc2_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2_.Texture2D.MipLevels = UINT(metadata2.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU2_ = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	textureSrvHandleGPU2_ = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	//SRVの作成
	device->CreateShaderResourceView(textureResource2_.Get(), &srvDesc2_, textureSrvHandleCPU2_);
}

void TextureResource::SetModelData(ModelData modelData)
{
	modelData_ = modelData;
}
