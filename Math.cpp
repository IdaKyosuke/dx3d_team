#include"Math.h"
#include<math.h>

// 1つの値を回転値に変換する（度数 -> ラジアン）
float Math::DegtoRad(float value)
{
	return value * DX_PI_F / Pi;
}

// ラジアン -> 度数に変換
float Math::RadtoDeg(float rad)
{
	return rad * Pi / DX_PI_F;
}

// Vector3を返す(回転値)
Vector3 Math::ChangeVec(const Vector3& v)
{
	VECTOR rot;
	rot = VGet(v.x * DX_PI_F / Pi, v.y * DX_PI_F / Pi, v.z * DX_PI_F / Pi);
	return Vector3(rot.x, rot.y, rot.z);
}

// ベクトルを正規化する（方向ベクトルを作る）
Vector3 Math::Normalized(const Vector3& rotate)
{
	float length = rotate.Magnitude();
	if (length == 0)
	{
		// 長さが0の場合正規化できないので、元のベクトルを返す
		return rotate;
	}
	return Vector3(rotate / length);
}

// ベクトルを回転させる
Vector3 Math::RotateVec(const Vector3& v, const Quaternion& q)
{
	Quaternion v_q(v.x, v.y, v.z, 0);
	Quaternion qConj = q.Conjugate();
	Quaternion result = q * v_q * qConj;

	return Vector3(result.x, result.y, result.z);
}

// ベクトルのなす角(XZ)を計算する
float Math::ACosPlane(const Vector3& v1, const Vector3& v2)
{
	Vector3 vec1 = Vector3(v1.x, 0, v1.z);
	Vector3 vec2 = Vector3(v2.x, 0, v2.z);
	// ベクトルのなす角（cosθ）を出す
	float cosSita = VDot(vec1, vec2) / (vec1.Magnitude() * vec2.Magnitude());
	// cosSitaの値が-1～1に収まるように制限
	if (cosSita > 1.0f) cosSita = 1.0f;
	if (cosSita < -1.0f) cosSita = -1.0f;

	// ラジアンに変換
	float sita = acosf(cosSita);

	// 外積で角度の符号を判定
	Vector3 crossProduct = v1.CrossP(v2);
	if (crossProduct.y < 0)
	{
		// 外積のy成分が負の時、反時計回り（角度 - 180度）
		//cosSita = 2.0f * DX_PI_F - cosSita;
		sita = 2.0f * DX_PI_F - sita;
	}

	// 0～180度で返す
	return sita * Pi / DX_PI_F;
}

// ベクトルのなす角(XYZ)を計算する
float Math::ACos(const Vector3& v1, const Vector3& v2)
{
	Vector3 vec1 = Vector3(v1.x, v1.y, v1.z);
	Vector3 vec2 = Vector3(v2.x, v2.y, v2.z);
	// ベクトルのなす角（cosθ）を出す
	float cosSita = VDot(vec1, vec2) / (vec1.Magnitude() * vec2.Magnitude());
	// cosSitaの値が-1～1に収まるように制限
	if (cosSita > 1.0f) cosSita = 1.0f;
	if (cosSita < -1.0f) cosSita = -1.0f;

	// 外積で角度の符号を判定
	Vector3 crossProduct = v1.CrossP(v2);
	if (crossProduct.z < 0)
	{
		// 外積のz成分が負の時、反時計回り（角度 - 180度）
		cosSita = 2.0f * DX_PI_F - cosSita;
	}

	// ラジアンに変換
	float sita = acosf(cosSita);

	// 0～180度で返す
	return sita * Pi / DX_PI_F;
}

// 前方ベクトルを基準に回転行列を作成する
MATRIX Math::GetLookRotMat(const Vector3& forword, const Vector3& camFront)
{
	return MGetRotVec2(Normalized(forword), Normalized(camFront));
}

// 行列を使ったベクトルの変換
Vector3 Math::TransformVec(const Vector3& v, const MATRIX mat)
{
	float x = v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + mat.m[3][0];
	float y = v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + mat.m[3][1];
	float z = v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + mat.m[3][2];
	return Vector3(x, y, z);
}

// 点の回転（XZ平面を回転し続ける）
Vector3 Math::PointRotate(const Vector3& satellite, const Vector3& center, const float angle)
{
	float ox = satellite.x - center.x;
	float oz = satellite.z - center.z;

	return Vector3(
		ox * cos(angle) + oz * sin(angle) + center.x,
		satellite.y,
		-ox * sin(angle) + oz * cos(angle) + center.z
		);
}


void Math::MatchAngleSign(float& afterAngle, const Vector3& moveDir, Vector3& transformAngle)
{
	afterAngle = atan2f(moveDir.z, moveDir.x);
	// 現在の角度の絶対値が符号を変えた後の数の絶対値より小さかったら何もしない(0 => 180)
	if (std::abs(transformAngle.y) > Math::GetSign(Math::Pi / 2))
	{
		if (Math::GetSign(transformAngle.y) < Math::GetSign(afterAngle))
		{
			// - => +
			transformAngle.y += Math::DegtoRad(Math::Pi * 2);
		}
		else if (Math::GetSign(transformAngle.y) > Math::GetSign(afterAngle))
		{
			// + => -
			transformAngle.y -= Math::DegtoRad(Math::Pi * 2);
		}
	}
}
