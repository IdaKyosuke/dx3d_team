#pragma once
#include"Actor3D.h"

class EscapePoint : public Actor3D
{
private:
	static constexpr int PointNum = 8;
	static constexpr Vector3 ColSize = Vector3(400, 400, 400);
	static constexpr Vector3 ColOffset = Vector3(0, 200, 0);

	Vector3 pos[PointNum] =
	{
		Vector3(9694, 0, -1876),	// 90
		Vector3(9699, 0, -256),		// 180
		Vector3(-3696, 0, 4381),	// -90
		Vector3(-8741, 0, 598),		// 180
		Vector3(-9671, 0, -1576),	// 0
		Vector3(4020, 0, -9387),	// 90
		Vector3(6068, 0, -9649),	// 180
		Vector3(9055, 0, -4712),	// 0
	};

	float Rotate[PointNum] =
	{
		90.0f,
		180.0f,
		-90.0f,
		180.0f,
		0.0f,
		90.0f,
		180.0f,
		0.0f,
	};

	int m_model;
	bool m_isEscape;

	const char* m_stairPath = "stair.mv1";

protected:
	virtual void OnCollision(const Actor3D* other) override;
	virtual void Draw() override;

public:
	EscapePoint();

	// íEèoÇ≈Ç´ÇΩÇ©
	bool IsEscape()
	{
		return m_isEscape;
	}
};