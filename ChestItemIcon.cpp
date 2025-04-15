#include "ChestItemIcon.h"
#include "Chest.h"
#include "Screen.h"

ChestItemIcon::ChestItemIcon(int itemNum, int countGetItem, Chest* chest) :
	m_itemNum(itemNum),
	m_countGetItem(countGetItem),
	m_chest(chest)
{
	m_itemIconUi.Register(m_iconName[m_itemNum]);
}

void ChestItemIcon::Load()
{
	m_itemIconUi.Load();
}

void ChestItemIcon::Release()
{
	m_itemIconUi.Release();
}

void ChestItemIcon::Update()
{
	m_itemIconUi.Update();

	if (m_chest->DestoryItemIcon())
	{
		if (m_countGetItem == m_chest->DestroyTakeItem())
		{
			Destroy();
		}
		if (m_countGetItem >= m_chest->DestroyTakeItem())
		{
			m_countGetItem--;
		}
	}

	m_transform.position = SetPos + Vector2(90 * m_countGetItem,90 * m_countGetItem % 10);
}

void ChestItemIcon::Draw()
{
	m_itemIconUi.Draw(m_transform);
}