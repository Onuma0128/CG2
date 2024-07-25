#include "LoadObjFile.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <string>

// ワイド文字列への変換関数
std::wstring s2ws(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename) {
    std::wstring filePath = s2ws(directoryPath + "/" + filename);
    HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    assert(hFile != INVALID_HANDLE_VALUE);

    DWORD fileSize = GetFileSize(hFile, nullptr);
    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, fileSize, nullptr);
    assert(hMapping != nullptr);

    char* data = static_cast<char*>(MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, fileSize));
    assert(data != nullptr);

    CloseHandle(hMapping);
    CloseHandle(hFile);

    ModelData modelData;
    std::vector<Vector4> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords;

    char* ptr = data;
    char* end = data + fileSize;
    while (ptr < end) {
        char* lineEnd = std::find(ptr, end, '\n');
        std::string line(ptr, lineEnd);
        ptr = lineEnd + 1;

        std::istringstream s(line);
        std::string identifier;
        s >> identifier;

        if (identifier == "v") {
            Vector4 position{};
            s >> position.x >> position.y >> position.z;
            position.x *= -1.0f;
            position.w = 1.0f;
            positions.push_back(position);
        }
        else if (identifier == "vt") {
            Vector2 texcoord{};
            s >> texcoord.x >> texcoord.y;
            texcoord.y = 1.0f - texcoord.y;
            texcoords.push_back(texcoord);
        }
        else if (identifier == "vn") {
            Vector3 normal{};
            s >> normal.x >> normal.y >> normal.z;
            normal.x *= -1.0f;
            normals.push_back(normal);
        }
        else if (identifier == "f") {
            VertexData triangle[3]{};
            for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
                std::string vertexDefinition;
                s >> vertexDefinition;
                std::istringstream v(vertexDefinition);
                uint32_t elementIndices[3]{};
                for (int32_t element = 0; element < 3; ++element) {
                    std::string index;
                    std::getline(v, index, '/');
                    elementIndices[element] = std::stoi(index);
                }
                Vector4 position = positions[elementIndices[0] - 1];
                Vector2 texcoord = texcoords[elementIndices[1] - 1];
                Vector3 normal = normals[elementIndices[2] - 1];
                triangle[faceVertex] = { position, texcoord, normal };
            }
            modelData.vertices.push_back(triangle[2]);
            modelData.vertices.push_back(triangle[1]);
            modelData.vertices.push_back(triangle[0]);
        }
        else if (identifier == "mtllib") {
            std::string materialFilename;
            s >> materialFilename;
            modelData.material = LoadmaterialTemplateFile(directoryPath, materialFilename);
        }
    }
    UnmapViewOfFile(data);
    return modelData;
}

MaterialData LoadmaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData; // 構築するMaterialData
	std::string line; // ファイルから読んだ1行を格納するもの
	std::ifstream file(directoryPath + "/" + filename); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;
		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}
