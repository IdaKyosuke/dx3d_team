#pragma once
#include"Actor3D.h"

class EscapePoint : public Actor3D
{
private:
	static constexpr Vector3 ColSize = Vector3(400, 400, 400);
	static constexpr Vector3 ColOffset = Vector3(0, 200, 0);

	int m_model;
	bool m_isEscape;

	const char* m_stairPath = "stair.mv1";

protected:
	virtual void OnCollision(const Actor3D* other) override;
	virtual void Draw() override;

public:
	EscapePoint(const Vector3& pos, const float rot);

	// íEèoÇ≈Ç´ÇΩÇ©
	bool IsEscape()
	{
		return m_isEscape;
	}
};