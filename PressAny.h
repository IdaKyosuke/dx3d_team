#pragma once
#include "Actor.h"

class PressAny : public Actor
{
private:
	static constexpr float BlinkDuration = 1.0f;	// 明滅の間隔

	float m_elapsedTime;
	bool m_drawFlag;

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	// コンストラクタ
	PressAny();
};
