#include "TextureResource.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"

void TextureResource::Initialize(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap, const uint32_t& descriptorSizeSRV)
{
	mipImages = LoadTexture("resources/white1x1.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	textureResource = CreateTextureResource(device, metadata);
	UploadTextureData(textureResource, mipImages);
	//metaDataを基にSRVの設定
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2DTexture
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	//SRVを制作するDescriptorHeapの場所を決める
	textureSrvHandleCPU = srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	textureSrvHandleGPU = srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
	//先頭はImGuiが使っているのでその次を使う
	textureSrvHandleCPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureSrvHandleGPU.ptr += device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの生成
	device->CreateShaderResourceView(textureResource, &srvDesc, textureSrvHandleCPU);
	//2枚目のTextureを読んで転送する
	mipImages2 = LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	textureResource2 = CreateTextureResource(device, metadata2);
	UploadTextureData(textureResource2, mipImages2);
	//metaDataを基にSRVの設定
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU2 = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	textureSrvHandleGPU2 = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 2);
	//SRVの作成
	device->CreateShaderResourceView(textureResource2, &srvDesc2, textureSrvHandleCPU2);
	//3枚目のTextureを読んで転送する
	mipImages3 = LoadTexture("resources/me.png");
	const DirectX::TexMetadata& metadata3 = mipImages3.GetMetadata();
	textureResource3 = CreateTextureResource(device, metadata3);
	UploadTextureData(textureResource3, mipImages3);
	//metaDataを基にSRVの設定
	srvDesc3.Format = metadata3.format;
	srvDesc3.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc3.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc3.Texture2D.MipLevels = UINT(metadata3.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	textureSrvHandleCPU3 = GetCPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 3);
	textureSrvHandleGPU3 = GetGPUDescriptorHandle(srvDescriptorHeap, descriptorSizeSRV, 3);
	//SRVの作成
	device->CreateShaderResourceView(textureResource3, &srvDesc3, textureSrvHandleCPU3);
}

void TextureResource::ImGui()
{
	static int selectedTextureIndex = 0;
	//画像名
    const char* textureNames[] = { "white1x1.png", "uvChecker.png", "me.png" };

    ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_DefaultOpen;

    if (ImGui::TreeNodeEx("Texture Selection", flag))
    {
		ImGui::Image((void*)currentTextureSrvHandleGPU.ptr, ImVec2(64, 64));
		ImGui::SameLine();
        // ComboBoxでテクスチャを選択
        ImGui::Combo("Select Texture", &selectedTextureIndex, textureNames, IM_ARRAYSIZE(textureNames));

        // 選択されたテクスチャを表示
        switch (selectedTextureIndex)
        {
        case 0:
			currentTextureSrvHandleGPU = textureSrvHandleGPU;
            break;
        case 1:
			currentTextureSrvHandleGPU = textureSrvHandleGPU2;
            break;
        case 2:
			currentTextureSrvHandleGPU = textureSrvHandleGPU3;
            break;
        }

        ImGui::TreePop();
    }
}

void TextureResource::Release()
{
	textureResource->Release();
	textureResource2->Release();
	textureResource3->Release();
}