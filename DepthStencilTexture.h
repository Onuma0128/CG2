#pragma once
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")

ID3D12Resource* CreateDepthStencilTextureResource(ID3D12Device* device, int width, int height);