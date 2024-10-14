#pragma once

#include <math.h>
//#include "Matrix4x4.h"
//#include "Vector3.h"
#include "AABB.h"

#include <KamataEngine.h>
using namespace KamataEngine;


Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 代入演算子オーバーロード
// Vector3の足算
Vector3& operator+=(Vector3& lhs, const Vector3& rhv);
// Vector3の引算
Vector3& operator-=(Vector3& lhs, const Vector3& rhv);
// Vector3の掛け算
Vector3& operator*=(Vector3& v, float s);
// Vector3の割り算
Vector3& operator/=(Vector3& v, float s);
// Matrix4x4の掛け算
Matrix4x4& operator*=(Matrix4x4& lhm, const Matrix4x4& rhm);

// 2項演算子オーバーロード
// Vector3の足算
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);





// 行列の掛け算
Matrix4x4 MatrixMultiply(Matrix4x4& m1, Matrix4x4& m2);

float EaseInOut(float x1, float x2, float t);
float Lerp(float x1, float x2, float t);
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

Matrix4x4 MakeRotateZMatrix(float theta);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// ノルム(長さ)を求める
float Length(const Vector3& v);
// 正規化する
Vector3 Normalize(const Vector3& v);
// 逆行列を求める
Matrix4x4 Inverse(const Matrix4x4& m);

