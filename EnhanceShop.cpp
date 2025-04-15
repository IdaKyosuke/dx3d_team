#include "EnhanceShop.h"
#include "Inventory.h"

EnhanceShop::EnhanceShop()
{
	//Ui‚Ì“o˜^
	m_enhanceShopUi.Register("shop_ui.png");

	m_enhanceShopTransform.position = Vector2(750, 250);
}

void EnhanceShop::Load()
{
	m_enhanceShopUi.Load();
}

void EnhanceShop::Release()
{
	m_enhanceShopUi.Release();
}

void EnhanceShop::Update()
{
	m_enhanceShopUi.Update();
}

void EnhanceShop::Draw()
{
	m_enhanceShopUi.Draw(m_enhanceShopTransform);
}