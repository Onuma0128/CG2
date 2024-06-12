#pragma once
#include <vector>
#include <xstring>

struct Vector2 {
	float x;
	float y;
};
struct Vector3 {
	float x;
	float y;
	float z;
};
struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};
struct Matrix3x3 {
	float m[3][3];
};
struct Matrix4x4 {
	float m[4][4];
};
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct MaterialData {
	std::string textureFilePath;
};
struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
};
struct Sphere {
	Vector3 center;
	float radius;
};
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};
const float pi = 3.141592653589793238462643383279f;
//クライアント領域のサイズ
const int32_t kClientWidth = 1280;
const int32_t kClientHeight = 720;

//単位行列
Matrix4x4 MakeIdentity4x4();
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 正規化
Vector3 Normalize(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector4& v);

//拡縮行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 1.x軸の回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2.y軸の回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3.z軸の回転行列
Matrix4x4 MakeRotateZMatrix(float radian);
//移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 座標変換
Vector3 Transform_(const Vector3& vector, const Matrix4x4& matrix);
// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 3次元アフィン変換
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 投資投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
// デバッグカメラ
Matrix4x4 MakeLookAtMatrix(const Vector3& Position, const Vector3& target, const Vector3& up);

//Sphere
VertexData* DrawSphere(VertexData* vertexData, uint32_t kSubdivision);