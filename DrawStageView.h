#pragma once
#include"Node.h"

// ステージの見た目だけを表示する
class DrawStageView : public Node
{
private:
	int m_model;

protected:
	virtual void Draw() override;
	virtual void Release() override;

public:
	DrawStageView(const char* model);

};