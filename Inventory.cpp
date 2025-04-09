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
		GetParent()->AddChild(new ItemIcon(m_itemNum, countGetItem - 1));

		m_gettingItem = false;
	}
}

void Inventory::Draw()
{
	m_inventoryUi.Draw(m_transform);

	
	DrawFormatString(0, 300, GetColor(255, 255, 255),
		"m_itemNum(%)",
		m_itemNum
	);
	//DrawString(0, 230, m_iconName[1], GetColor(255, 255, 255));
}