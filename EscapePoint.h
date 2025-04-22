#pragma once
#include"Actor3D.h"

class EscapePoint : public Actor3D
{
private:
	static constexpr int PointNum = 8;
	static constexpr Vector3 ColSize = Vector3(200, 200, 200);
	Vector3 pos[PointNum] =
	{
		Vector3(9694, 0, -1876),
		Vector3(9699, 0, -256),
		Vector3(-3696, 0, 4381),
		Vector3(-8741, 0, 598),
		Vector3(-9671, 0, -1576),
		Vector3(4020, 0, -9387),
		Vector3(6068, 0, -9649),
		Vector3(9055, 0, -4712),
	};

	bool m_isEscape;

protected:
	virtual void OnCollision(const Actor3D* other) override;

public:
	EscapePoint();

	// íEèoÇ≈Ç´ÇΩÇ©
	bool IsEscape()
	{
		return m_isEscape;
	}
};