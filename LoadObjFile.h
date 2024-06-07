#pragma once
#include "MT3.h"
#include <fstream>
#include <sstream>

ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

MaterialData LoadmaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
