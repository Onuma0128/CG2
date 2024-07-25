#pragma once
#include "MT3.h"
#include <fstream>
#include <sstream>

std::wstring s2ws(const std::string& str);

ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

MaterialData LoadmaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
