#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <cassert>
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

std::string ConvertString(const std::wstring& str);

//Log関数
void Log(const std::string& message);

//CompileShader関数
IDxcBlob* CompileShader(
	//CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	//Compilerに使用するProfile
	const wchar_t* profile,
	//初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler);
