#include "ItemIcon.h"
#include "Inventory.h"
#include "Screen.h"


ItemIcon::ItemIcon(int itemNum, int countGetItem,Inventory* inventry) :
	m_itemNum(itemNum),
	m_countGetItem(countGetItem),
	m_inventory(inventry)
{
	m_itemIconUi.Register(std::next(m_inventory->GetItemList().begin(), m_countGetItem)->GetItemIconName());
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

	if (m_inventory->GetDropItem())
	{
		if (m_countGetItem == m_inventory->GetTakeItem())
		{
			Destroy();
		}
		if (m_countGetItem >= m_inventory->GetTakeItem())
		{
			m_countGetItem--;
		}
	}
}

void ItemIcon::Draw()
{
	m_itemIconUi.Draw(m_transform);
}