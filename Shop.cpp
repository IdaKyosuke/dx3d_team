#include "Shop.h"
#include "Inventory.h"
#include "Chest.h"

Shop::Shop(Chest* chest) :
	m_chest(chest)
{
	//Ui‚Ì“o˜^
	m_shopUi.Register("shop_ui.png");

	m_shopTransform.position = Vector2(750, 260);
}

void Shop::Load()
{
	m_shopUi.Load();
}

void Shop::Release()
{
	m_shopUi.Release();
}

void Shop::Update()
{
	m_shopUi.Update();

	m_chest->GetItemList().begin()->GetSellMoney();
}

void Shop::Draw()
{
	m_shopUi.Draw(m_shopTransform);
}