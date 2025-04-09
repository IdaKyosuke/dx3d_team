#include "Inventory.h""
#include "Screen.h"
#include "ImageLoader.h"
#include "ItemIcon.h"

Inventory::Inventory() :
	m_haveItemCount(0),
	m_canGetItem(false),
	m_itemNum(0),
	m_gettingItem(false),
	m_takeItem(0)
{
	m_transform.position = Screen::BottomCenter + Vector2(-400, -70);
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

	int countGetItem = std::distance(m_itemList.begin(), m_itemList.end());

	if (m_haveItemCount >= MaxHaveItem)
	{
		m_canGetItem = false;
	}
	else
	{
		m_canGetItem = true;
	}

	if (m_gettingItem)
	{
		decltype(m_itemList)::iterator Item = std::next(m_itemList.begin(), countGetItem -1);

		m_itemNum = Item->GetItemNum();

		GetParent()->AddChild(new ItemIcon(m_itemNum, countGetItem - 1));

		m_gettingItem = false;
	}
}

void Inventory::Draw()
{
	m_inventoryUi.Draw(m_transform);
}