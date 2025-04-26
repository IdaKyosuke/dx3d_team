#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class UiStamina : public Node
{
private:
	static constexpr int LeftEdge = 200;	// 四角形の左端の座標
	static constexpr int RightEdge = 1079;	// 四角形の右端の座標

	int m_topLeft;
	int m_bottomRight;
	//static constexpr int Offset;

	unsigned int m_color;
	Transform m_transform;	// 姿勢

protected:
	//virtual void Load() override;
	//virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	UiStamina();
};