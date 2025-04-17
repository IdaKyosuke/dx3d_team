#include "EnhanceShop.h"
#include "Chest.h"

EnhanceShop::EnhanceShop(Chest* chest)://, EnhanceButton* enhanceButton) :
	m_chest(chest)
	//m_enhanceButton(enhanceButton)
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

	//‹­‰»
	
	
}

void EnhanceShop::Draw()
{
	m_enhanceShopUi.Draw(m_enhanceShopTransform);
}