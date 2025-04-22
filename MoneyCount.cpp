#include "MoneyCount.h"
#include "Screen.h"
#include "ImageLoader.h"
#include "Wallet.h"
#include <DxLib.h>

MoneyCount::MoneyCount(Wallet* wallet, int restDays, int clearCount) :
	m_needMoney(NeedMoney),
	m_restDays(restDays),
	m_wallet(wallet),
	m_clearCount(clearCount),
	m_fontTextureId(0),
	m_taskClear(false),
	m_backMenu(false)
{
	m_transform.position = UiPos;
	m_transform.scale = 0.3f;
	m_moneyCountUi.Register("money_count_ui.png");
}

void MoneyCount::Load()
{
	m_moneyCountUi.Load();
	m_fontTextureId = ImageLoader::GetInstance()->Load("score_font.png");
}

void MoneyCount::Release()
{
	m_moneyCountUi.Release();
	ImageLoader::GetInstance()->Delete("score_font.png");
}

void MoneyCount::Update()
{
	m_moneyCountUi.Update();

	if (m_clearCount > 0)
	{
		m_needMoney *= m_clearCount;
	}

	if (m_backMenu)
	{
		m_restDays--;
		m_backMenu = false;
	}

	if (m_restDays <= 0)
	{
		if (m_wallet->HaveMoney() >= m_needMoney)
		{
			m_taskClear = true;
		}
		else
		{
			m_taskClear = false;
		}
	}

	if (m_restDays < 0)
	{
		m_restDays = RestDays;
	}
}

void MoneyCount::Draw()
{
	m_moneyCountUi.Draw(m_transform);

	//残り日数の描画
	Vector2 dest = m_transform.position + Vector2(40, -65);
	dest.y += FontMargin;
	int score = m_restDays;
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

	//必要なお金の描画
	Vector2 dest1 = m_transform.position + Vector2(80, 20);
	dest1.y += FontMargin;
	int score1 = m_needMoney;
	int digit1 = 1;
	do
	{
		int value = score1 % 10;	// 1の位の値を取り出す

		DrawRectGraph(
			static_cast<int>(dest1.x - FontSize.x * digit1), static_cast<int>(dest1.y),
			static_cast<int>(FontSize.x) * value, 0,
			static_cast<int>(FontSize.x), static_cast<int>(FontSize.y),
			m_fontTextureId,
			true
		);

		score1 /= 10;	// スコアの桁下げ
		digit1++;		// 次の桁へ
	} while (score1 > 0);
}