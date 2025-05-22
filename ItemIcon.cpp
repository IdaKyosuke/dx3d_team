#include "ItemIcon.h"
#include "Inventory.h"
#include "Screen.h"


ItemIcon::ItemIcon(const int countGetItem, Item* item, Inventory* inventry) :
	m_inventory(inventry),
	m_countGetItem(countGetItem)
{
	m_itemIconUi.Register(item->GetItemIconName());
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
	m_transform.position = SetPos + Vector2(90 * m_countGetItem, 0);

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