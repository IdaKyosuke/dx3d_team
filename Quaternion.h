#pragma once
#include<cmath>
#include"DxLib.h"

class Vector3;

class Quaternion
{
public:
	float x, y, z, w;
	// -----コンストラクタ-----
	// デフォルト
	Quaternion() :
		x(0),
		y(0),
		z(0),
		w(0){}

	// 各要素を指定して初期化
	template<typename T1, typename T2, typename T3, typename T4>
	constexpr Quaternion(T1 x, T2 y, T3 z, T4 w) :
		x(static_cast<float>(x)),
		y(static_cast<float>(y)),
		z(static_cast<float>(z)),
		w(static_cast<float>(w)){}

	// Vector3で要素を指定
	constexpr Quaternion(const Vector3& v);

	// ベクトルへのキャスト
	operator Vector3() const;

	// 行列への変換
	operator DxLib::MATRIX() const;

	// 共役複素数を取得
	constexpr Quaternion Conjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	// 長さの２乗を返す
	constexpr float GetLengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	// 長さを返す
	float GetLength() const
	{
		return std::sqrt(GetLengthSquared());
	}

	// 正規化する
	void Normalized()
	{
		float l = GetLength();

		x /= l;
		y /= l;
		z /= l;
		w /= l;
	}

	// ----演算子オペレータ----
	// 代入
	constexpr Quaternion operator =(const Quaternion& q) const
	{
		return q;
	}

	// 乗算
	constexpr Quaternion operator *(const Quaternion& q) const
	{
		return	Quaternion(
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w
		);
	}

	// ----四元数の作成----
	// 単位四元数の作成
	static Quaternion CreateIdentity()
	{
		return Quaternion(0, 0, 0, 1);
	}
	
	// ベクトルを回転させる
	static Vector3 RotateVec(const Vector3& v, const Quaternion& q);

	// 任意軸回転の四元数の作成
	static Quaternion CreateRotationAxis(const Vector3& axis, const float angle);

	// 向きを変更するために回転の四元数を作成（fromからtoへの角度）
	static Quaternion CreateRotateDirection(const Vector3& from, const Vector3& to);

	// ----四元数の設定----
	// 単位四元数の設定
	void SetIdentity() const
	{
		*this = CreateIdentity();
	}

	// 任意軸回転の四元数の設定
	void SetRotateAxis(const Vector3& v, const float angle)
	{
		*this = CreateRotationAxis(v, angle);
	}

	// 向きを変更するための回転の四元数設定
	void SetRotationDirection(const Vector3& from, const Vector3& to)
	{
		*this = CreateRotateDirection(from, to);
	}

	// 内積
	static float Dot(const Quaternion& q1, const Quaternion& q2);

	// 指定したモデルの回転（Y軸）
	static void RotateAxisY(const int model, const float rotAngle, const Vector3& pos);

	// 指定したモデルの回転（上下反転させた状態）
	static void RotateAxisYZ(const int model, const float rotAngle, const Vector3& pos);
};