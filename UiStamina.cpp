#include "UiStamina.h"
#include "LoadPlayer.h"

UiStamina::UiStamina(LoadPlayer* player) :
	m_leftX(GaugeLeft),
	m_rightX(GaugeRight),
	m_player(player),
	m_staminaRatio(0),
	m_halfWidth(0)
{
}

void UiStamina::Update()
{
	// 現在のスタミナ÷スタミナの最大値で残スタミナの比率を計算
	m_staminaRatio = m_player->GetStamina() / m_player->GetMaxStamina();

	// 左右で半分ずつの幅を計算
	m_halfWidth = static_cast<int>((GaugeWidth * m_staminaRatio) / 2);

	// スタミナゲージの拡縮（中央に縮む）
	m_leftX = GaugeCenter - m_halfWidth;
	m_rightX = GaugeCenter + m_halfWidth;
}

void UiStamina::Draw()
{
	// 現在のスタミナが最大値じゃないとき
	if (m_player->GetStamina() != m_player->GetMaxStamina())
	{
		// スタミナゲージの背景
		DrawBox(GaugeLeft, GaugeY, GaugeRight, GaugeY + GaugeHeight, GetColor(100, 100, 100), true);

		// 現在のスタミナゲージ（中央に縮む）
		DrawBox(m_leftX, GaugeY, m_rightX, GaugeY + GaugeHeight, GetColor(200, 200, 0), true);
	}
}
