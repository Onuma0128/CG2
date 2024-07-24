#pragma once
#include "wrl.h"
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include "CompileShader.h"
#include <assert.h>

using Microsoft::WRL::ComPtr;

class PipelineState
{
public:

	void Initialize(
		ComPtr<ID3D12Device>& device, ComPtr<IDxcUtils> dxcUtils, 
		ComPtr<IDxcCompiler3> dxcCompiler, ComPtr<IDxcIncludeHandler> includeHandler);
	// シェーダーコンパイル
	void Object3dShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	void ParticleShader(ComPtr<IDxcBlob>& vertexShader, ComPtr<IDxcBlob>& pixelShader);
	// インプットレイアウト
	void Object3dInputLayout();
	void ParticleInputLayout();
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> CreateNewRootSignature();
	// パイプラインの生成
	ComPtr<ID3D12PipelineState> CreateObject3dPipelineState();
	ComPtr<ID3D12PipelineState> CreateParticlePipelineState();

private:
	ComPtr<ID3D12Device> device_;
	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs_[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_ = {};

	ComPtr<ID3D12RootSignature> newRootSignature_;
	ComPtr<ID3D12PipelineState> newPipelineState_;
	HRESULT hr_;
};

