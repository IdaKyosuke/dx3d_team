#include "EnhanceShopButton.h"
#include "SceneGame.h"
#include "DxLib.h"

EnhanceShopButton::EnhanceShopButton() :
	Actor("EnhanceShopButton", "enhance_button_ui.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&EnhanceShopButton::OnClick, this)),
	m_checkOnClick(false),
	m_shopOrEnhance(false),
	m_shopEnhanceNum()
{
}

void EnhanceShopButton::OnClick()
{
	m_checkOnClick = true;
}

void EnhanceShopButton::Update()
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

void EnhanceShopButton::Draw()
{
	//本来の描画処理
	Actor::Draw();
}