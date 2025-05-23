#include "Wallet.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "DxLib.h"

Wallet::Wallet() :
	m_fontTextureId(0),
	m_haveMoney(0)
{
	m_transform.position = WalletPos;
	m_transform.scale = 0.3f;
	m_walletImg.Register("wallet.png");
}

void Wallet::Load()
{
	m_walletImg.Load();
	m_fontTextureId = ImageLoader::GetInstance()->Load("score_font.png");
}

void Wallet::Release()
{
	m_walletImg.Release();
	ImageLoader::GetInstance()->Delete("score_font.png");
}

void Wallet::Update()
{
	m_walletImg.Update();
}

void Wallet::Draw()
{
	// Walletを描画
	m_walletImg.Draw(m_transform);

	Vector2 dest = m_transform.position + Vector2(40, -17);
	dest.y += FontMargin;
	int score = m_haveMoney;
	int digit = 1;
	do
	{
		int value = score % 10;	// 1の位の値を取り出す

		DrawRectGraph(
			static_cast<int>(dest.x - FontSize.x * digit), static_cast<int>(dest.y),
			static_cast<int>(FontSize.x) * value, 0,
			static_cast<int>(FontSize.x), static_cast<int>(FontSize.y),
			m_fontTextureId,
			true
		);

		score /= 10;	// スコアの桁下げ
		digit++;		// 次の桁へ
	} while (score > 0);
}