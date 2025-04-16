#include "EnhanceShop.h"
#include "Chest.h"
#include "EnhanceButton.h"
#include "EnhanceInventory.h"

EnhanceShop::EnhanceShop(Chest* chest, EnhanceButton* enhanceButton, EnhanceInventory* enhanceInventory) :
	m_chest(chest),
	m_enhanceButton(enhanceButton),
	m_enhanceInventory(enhanceInventory)
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
	if (m_enhanceInventory->GetCheckOnClick())
	{
		if (m_enhanceButton->GetCheckOnClick())
		{
			//ƒCƒ“ƒxƒ“ƒgƒŠ‚Ì‹­‰»
		}
	}
}

void EnhanceShop::Draw()
{
	m_enhanceShopUi.Draw(m_enhanceShopTransform);
}