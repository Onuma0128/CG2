#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <string>
#include "externals/DirectXTex/DirectXTexP.h"

std::wstring ConvertString(const std::string& str);
//Textureデータを読む
DirectX::ScratchImage LoadTexture(const std::string& filePath);
//DirectX12のTextureResourceを作る
ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
//TextureResourceにデータを転送する
void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);