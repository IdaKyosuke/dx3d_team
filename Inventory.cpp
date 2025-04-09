#include "Inventory.h""
#include "Screen.h"
#include "ImageLoader.h"

Inventory::Inventory() :
	m_haveItemCount(MaxHaveItem),
	m_canGetItem(false)
{
	m_transform.position = Screen::BottomCenter + Vector2(-400,-70);
	m_inventoryUi.Register("inventoryUi.png");
}

void Inventory::Load()
{
	m_inventoryUi.Load();
}

void Inventory::Release()
{
	m_inventoryUi.Release();
}

void Inventory::Update()
{
	m_inventoryUi.Update();

	if (m_haveItemCount <= 0)
	{
		m_canGetItem = false;
	}
	else
	{
		m_canGetItem = true;
	}


}

void Inventory::Draw()
{
	m_inventoryUi.Draw(m_transform);
}