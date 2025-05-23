#pragma once
#include"Actor.h"
#include"Transform.h"
#include<vector>

class LoadPlayer;

class ScreenFilter : public Node
{
private:
	static constexpr int FilterNum = 3;
	static constexpr int MaxPal = 150;	// 輝度の最大値
	// 画面のフィルター用
	static const char* FilterTexture[FilterNum];
	LoadPlayer* m_player;
	Sprite m_redImg;
	Sprite m_grayImg;
	Sprite m_whiteImg;

	Transform m_transform;

protected:
	virtual void Draw() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Load() override;

public:
	ScreenFilter(LoadPlayer* player);
};