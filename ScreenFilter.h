#pragma once
#include"Actor.h"
#include<vector>

class LoadPlayer;

class ScreenFilter : public Actor
{
private:
	static constexpr int MaxPal = 150;	// 輝度の最大値
	// 画面のフィルター用
	static const char* FilterTexture[2];
	LoadPlayer* m_player;

protected:
	void Draw() override;

public:
	ScreenFilter(LoadPlayer* player);
};