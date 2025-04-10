#include "Shop.h"
#include "Inventory.h"

Shop::Shop(Inventory* inventry) : 
	m_inventory(inventry)
{
	//Ui‚Ì“o˜^
	m_shopUi.Register("");

	m_transform.position = Vector3(0, 0, 0);
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
}

void Shop::Draw()
{
	m_shopUi.Draw(m_transform);
}