#include "EnhanceInventory.h"
#include "SceneGame.h"
#include "DxLib.h"

EnhanceInventory::EnhanceInventory() :
	Actor("EnhanceShopButton", "enhance_inventory_ui.png", Position),
	m_button(Size, MOUSE_INPUT_LEFT, std::bind(&EnhanceInventory::OnClick, this)),
	m_checkOnClick(false),
	m_shopOrEnhance(false),
	m_shopEnhanceNum()
{
}

void EnhanceInventory::OnClick()
{
	m_checkOnClick = true;
}

void EnhanceInventory::Update()
{
	//�{���̍X�V����
	Actor::Update();

	//�{�^��
	m_button.Update(m_transform.position);
}

void EnhanceInventory::Draw()
{
	//�{���̕`�揈��
	Actor::Draw();
}