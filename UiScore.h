#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class UiScore : public Node
{
private:
	static constexpr Vector2 ScoreSize = Vector2(40, 11);	// SCORE画像の幅・高さ
	static constexpr Vector2 FontSize = Vector2(20, 35);	// 数字1文字の幅・高さ
	static constexpr Vector2 Offset = Vector2(0, 35);		// 画面右上からの差分
	static constexpr int FontMargin = 5;					// 数字と数字の間の余白

	Sprite m_scoreImg;		// SCORE画像
	int m_fontTextureId;	// 数字フォント
	Transform m_transform;	// 姿勢
	int m_score;			// スコアの値
	int m_se;
	int m_itemNum;			// アイテムの総数

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	// コンストラクタ
	UiScore();

	// アイテム獲得後にカウントを減らす
	void GetItem(int num);

	// アイテム数の設定
	void SetNum(int num)
	{
		m_itemNum = num;
	}

	// 座標を設定
	void SetPosition(const Vector2& position)
	{
		m_transform.position = position;
	}
};
