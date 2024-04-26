#pragma once
#include <Windows.h>
#include <string>
#include <format>
#include <cassert>
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

std::string ConvertString(const std::wstring& message);

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