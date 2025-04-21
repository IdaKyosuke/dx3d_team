#include "ItemIcon.h"
#include "Inventory.h"
#include "Screen.h"


ItemIcon::ItemIcon(int itemNum, int countGetItem,Inventory* inventry) :
	m_itemNum(itemNum),
	m_countGetItem(countGetItem),
	m_inventory(inventry)
{
	m_itemIconUi.Register(m_iconName[m_itemNum]);
}

void ItemIcon::Load()
{
	m_itemIconUi.Load();
}

void ItemIcon::Release()
{
	m_itemIconUi.Release();
}

void ItemIcon::Update()
{
	m_itemIconUi.Update();
	m_transform.position = SetPos[m_countGetItem];

	if (m_inventory->DestoryItemIcon())
	{
		if (m_countGetItem == m_inventory->DestroyTakeItem())
		{
			Destroy();
		}
		if (m_countGetItem >= m_inventory->DestroyTakeItem())
		{
			m_countGetItem--;
		}
	}
}

void ItemIcon::Draw()
{
	m_itemIconUi.Draw(m_transform);
}