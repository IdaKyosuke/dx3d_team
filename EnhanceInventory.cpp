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
	//本来の更新処理
	Actor::Update();

	//ボタン
	m_button.Update(m_transform.position);
}

void EnhanceInventory::Draw()
{
	//本来の描画処理
	Actor::Draw();
}