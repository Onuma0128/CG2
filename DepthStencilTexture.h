#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "wrl.h"
#include "ResourceObject.h"

using Microsoft::WRL::ComPtr;

ResourceObject CreateDepthStencilTextureResource(ComPtr<ID3D12Device> device, int width, int height);
