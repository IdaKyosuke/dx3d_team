#include "EnhanceButton.h"
#include "SceneGame.h"
#include "DxLib.h"

EnhanceButton::EnhanceButton() :
	Actor("EnhanceShopButton", "enhance_button_ui.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&EnhanceButton::OnClick, this)),
	m_checkOnClick(false),
	m_shopOrEnhance(false),
	m_shopEnhanceNum()
{
}

void EnhanceButton::OnClick()
{
	m_checkOnClick = true;
}

void EnhanceButton::Update()
{
	//本来の更新処理
	Actor::Update();

	//ボタン
	m_button.Update(m_transform.position);
}

void EnhanceButton::Draw()
{
	//本来の描画処理
	Actor::Draw();
}