#pragma once
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
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
struct DirectionalLight {
	Vector4 color;
	Vector3 direction;
	float intensity;
};

//単位行列
Matrix4x4 MakeIdentity4x4();
// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);
// 減算
Vector3 Subtract(const Vector4& v1, const Vector4& v2);
// 正規化
Vector3 Normalize(const Vector3& v);
// 正規化
Vector3 Normalize(const Vector4& v);
// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);
//
Vector3 CalculateNormal(const VertexData& v0, const VertexData& v1, const VertexData& v2);

// 1.x軸の回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// 2.y軸の回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// 3.z軸の回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 3次元アフィン変換
Matrix4x4 MakeAfineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 投資投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspecRatio, float nearClip, float farClip);