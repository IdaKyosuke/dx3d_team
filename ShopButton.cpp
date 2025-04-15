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
	//�{���̍X�V����
	Actor::Update();

	//�{�^��
	m_button.Update(m_transform.position);
}

void ShopButton::Draw()
{
	//�{���̕`�揈��
	Actor::Draw();
}