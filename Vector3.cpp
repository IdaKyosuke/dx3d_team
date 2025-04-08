#include "Vector3.h"
#include "Vector2.h"
#include <cmath>

// Vector2へのキャスト
Vector3::operator Vector2() const
{
	return Vector2(x, y);
}

// VECTOR(DxLib)へのキャスト
Vector3::operator VECTOR() const
{
	return VECTOR(VGet(x, y, z));
}

// VECTOR_D(DxLib)へのキャスト
Vector3::operator VECTOR_D() const
{
	return VECTOR_D(VGetD(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)));
}

// 長さを取得
float Vector3::Magnitude() const
{
	return std::sqrtf(SqrMagnitude());
}
