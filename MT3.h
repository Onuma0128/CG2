#pragma once
#include <vector>
#include <xstring>
#include <random>
#include <cassert>
#include <list>

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
struct AABB {
	Vector3 min;
	Vector3 max;
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
	float shininess;
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
struct Particle {
	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};
struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};
struct Emitter {
	Transform transform;
	uint32_t count;
	float frequency;
	float frequencyTime;
};
struct AccelerationField {
	Vector3 acceleration;
	AABB area;
};
struct CameraForGPU {
	Vector3 worldPosition;
};

const float pi = 3.141592653589793238462643383279f;
//クライアント領域のサイズ
const int32_t kClientWidth = 1280;
const int32_t kClientHeight = 720;

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);


//単位行列
Matrix4x4 MakeIdentity4x4();
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
// 回転行列
Matrix4x4 MakeRotateMatrix(const Vector3& rotate);
//移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 3次元アフィン変換
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
Vector3 Transform_(const Vector3& vector, const Matrix4x4& matrix);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 投資投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

//Sphere
VertexData* DrawSphere(VertexData* vertexData, uint32_t kSubdivision);
//Particle生成関数
Particle MakeNewParticle(std::mt19937& randomEngine,const Vector3& translate);
//Emit関数
std::list<Particle> Emit(const Emitter& emitter, std::mt19937& randomEngine);
// AABBとpoint
bool IsCollision(const AABB& aabb, const Vector3& point);

//単項演算子
Vector3 operator+(const Vector3& v);
Vector3 operator-(const Vector3& v);

//二項演算子
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator*(float s, const Vector3& v);
Vector3 operator*(const Vector3& v, float s);

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);