#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class UiTime : public Node
{
private:
	static constexpr Vector2 TimeSize = Vector2(80, 19);	// SCORE画像の幅・高さ
	static constexpr Vector2 FontSize = Vector2(20, 35);	// 数字1文字の幅・高さ
	static constexpr Vector2 Offset = Vector2(-50, 25);		// 画面右上からの差分
	static constexpr int FontMargin = 5;
	static constexpr float LimitTime = 300.0f;	// 制限時間

	int m_fontTextureId;	// 数字フォント
	Transform m_transform;	// 姿勢
	float m_limitTime;			// 時間の値
	bool m_isFinsh;		// 時間切れフラグ

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	UiTime();

	// 時間切れフラグを受け取る
	bool IsFinsh()
	{
		return m_isFinsh;
	}
};
