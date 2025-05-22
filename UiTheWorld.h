#pragma once
#include "Actor.h"
#include "Transform.h"
#include "Sprite.h"
#include "Vector2.h"

class LoadPlayer;

class UiTheWorld : public Actor
{
private:
	static constexpr Vector2 GridSize = Vector2(80, 80);	// アニメーションのサイズ
	static constexpr int GridNum = 30;	// アニメコマ数
	static constexpr Vector2 Offset = Vector2(70, 70);
	static constexpr float ChangeAngleTime = 0.8f;	// スキル再使用可能時に砂時計を反転させるのにかかる時間

	int m_glassImageId;
	Sprite m_glass_backImageId;
	LoadPlayer* m_player;
	float m_elapsedTime;
	bool m_isCoolDown;
	bool m_isStop;	// 時間が停止しているか
	float m_angle;	// 静止画を描画する際の角度
	int m_seStopTime;
	int m_seStopTimePlay;
	int m_seStopTimeFinish;
	int m_seCoolDownFinish;
	bool m_seStart;

	const char* AnimeName[1] =
	{
		"TheWorld"
	};

	/*const Animation AnimeData[1] =
	{
		Animation("theworld_icon.png",5, 1)
	};*/

protected:
	virtual void Load() override;	// リソースの読み込み
	virtual void Release() override;// リソースの破棄
	virtual void Update() override;	// 更新
	virtual void Draw() override;	// 描画

public:
	UiTheWorld(LoadPlayer* player);
};
