#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <cassert>
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")
#include "wrl.h"

using Microsoft::WRL::ComPtr;

std::string ConvertString(const std::wstring& str);

//Log関数
void Log(const std::string& message);

//CompileShader関数
ComPtr<IDxcBlob> CompileShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するProfile
	const wchar_t* profile,
	//初期化で生成したものを3つ
	ComPtr<IDxcUtils> dxcUtils,
	ComPtr<IDxcCompiler3> dxcCompiler,
	ComPtr<IDxcIncludeHandler> includeHandler);