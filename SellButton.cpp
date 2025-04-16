#include "SellButton.h"
#include "SceneGame.h"
#include "DxLib.h"

SellButton::SellButton() :
	Actor("SellButton", "sell_button.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&SellButton::OnClick, this)),
	m_checkOnClick(false)
{
}

void SellButton::OnClick()
{
	m_checkOnClick = true;
}

void SellButton::Update()
{
	//本来の更新処理
	Actor::Update();
	if (m_checkOnClick)
	{
		m_checkOnClick = false;
	}

	//ボタン
	m_button.Update(m_transform.position);
}

void SellButton::Draw()
{
	//本来の描画処理
	Actor::Draw();
}