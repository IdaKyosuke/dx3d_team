#pragma once
#include "Actor.h"
#include "Transform.h"
#include "Sprite.h"
#include "Vector2.h"


class UiTheWorld : public Actor
{
private:
	static constexpr Vector2 GridSize = Vector2(80, 80);	// アニメーションのサイズ
	static constexpr int GridNum = 5;	// アニメコマ数
	static constexpr Vector2 Offset = Vector2(50, 25);

	int m_imageId;
	Transform m_transform;


	const char* AnimeName[1] =
	{
		"TheWorld"
	};

	const Animation AnimeData[1] =
	{
		Animation("theworld_icon.png",5, 9)
	};

protected:
	virtual void Load() override;	// リソースの読み込み
	virtual void Release() override;// リソースの破棄
	virtual void Update() override;	// 更新
	virtual void Draw() override;	// 描画

public:
	UiTheWorld();
};
