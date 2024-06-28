#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <string>
#include "externals/DirectXTex/DirectXTex.h"
#include "wrl.h"

using Microsoft::WRL::ComPtr;

std::wstring ConvertString(const std::string& str);
//Textureデータを読む
DirectX::ScratchImage LoadTexture(const std::string& filePath);
//DirectX12のTextureResourceを作る
ComPtr<ID3D12Resource> CreateTextureResource(ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);
//TextureResourceにデータを転送する
void UploadTextureData(ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);