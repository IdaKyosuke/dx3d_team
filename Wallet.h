#pragma once
#include "Node.h"
#include "Transform.h"
#include "Sprite.h"

class Wallet : public Node
{
private:
	static constexpr Vector2 WalletSize = Vector2(40, 11);	// SCORE画像の幅・高さ
	static constexpr Vector2 FontSize = Vector2(20, 35);	// 数字1文字の幅・高さ
	static constexpr Vector2 WalletPos = Vector2(125, 70);		// 画面右上からの差分
	static constexpr int FontMargin = 5;					// 数字と数字の間の余白

	Sprite m_walletImg;		// Wallet画像
	int m_fontTextureId;	// 数字フォント
	Transform m_transform;	// 姿勢
	int m_haveMoney;		// スコアの値

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	// コンストラクタ
	Wallet();

	/*
	// 座標を設定
	void SetPosition(const Vector2& position)
	{
		m_transform.position = position;
	}
	*/

	void InWalletMoney(int money)
	{
		m_haveMoney += money;
	}

	void LostMoney(int money)
	{
		m_haveMoney -= money;
	}

	//今持っているお金
	int HaveMoney() const
	{
		return m_haveMoney;
	}
};
