#include "ShopButton.h"
#include "SceneGame.h"
#include "DxLib.h"

ShopButton::ShopButton() :
	Actor("ShopButton", "change_button_ui.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&ShopButton::OnClick, this)),
	m_checkOnClick(false)
{
}

void ShopButton::OnClick()
{
	m_checkOnClick = true;
}

void ShopButton::Update()
{
	//本来の更新処理
	Actor::Update();

	//ボタン
	m_button.Update(m_transform.position);
}

void ShopButton::Draw()
{
	//本来の描画処理
	Actor::Draw();
}