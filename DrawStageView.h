#pragma once
#include"Actor3D.h"

// ステージの見た目だけを
class DrawStageView : public Actor3D
{
private:
	static constexpr Vector3 DrawPos = Vector3(0, 0, 0);
};