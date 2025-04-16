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
	//�{���̍X�V����
	Actor::Update();

	if (m_checkOnClick)
	{
		m_checkOnClick = false;
	}

	//�{�^��
	m_button.Update(m_transform.position);
}

void EnhanceShopButton::Draw()
{
	//�{���̕`�揈��
	Actor::Draw();
}