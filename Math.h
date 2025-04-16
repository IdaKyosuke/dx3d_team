#pragma once
#include"Vector3.h"
#include"Quaternion.h"

class Math
{
private:


public:
	static constexpr float Pi = 180.0f;	// π

	// 1つの値を回転値に変換する（度数 -> ラジアン）
	static float DegtoRad(float value);

	// ラジアン -> 度数に変換
	static float RadtoDeg(float rad);

	// VECTORを返す(回転値)
	static Vector3 ChangeVec(const Vector3& v);

	// ベクトルを正規化する（方向ベクトルを作る）
	static Vector3 Normalized(const Vector3& rotate);

	// ベクトルを回転させる
	static Vector3 RotateVec(const Vector3& v, const Quaternion& q);

	// ベクトルのなす角を計算する(XZ平面)
	static float ACosPlane(const Vector3& v1, const Vector3& v2);
	
	// ベクトルのなす角を計算する
	static float ACos(const Vector3& v1, const Vector3& v2);

	// 前方ベクトルを基準に回転行列を作成する
	static MATRIX GetLookRotMat(const Vector3& forword, const Vector3& camFront);

	// 行列を使ったベクトルの変換
	static Vector3 TransformVec(const Vector3& v, const MATRIX mat);

	// 点の回転
	static Vector3 PointRotate(const Vector3& satellite, const Vector3& center, const float angle);

	// 符号を取ってくる
	template<typename T>
	static int GetSign(T v)
	{
		return (v > 0) - (v < 0);
	}

	// 回転で現在の向きと回転予定の向きの符号が違うときに符号を合わせる（180 ～ -180でしか取れないため）
	static void MatchAngleSign(float& afterAngle, const Vector3& moveDir, Vector3& transformAngle);
};
