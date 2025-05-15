#include "ChestItemIcon.h"
#include "Chest.h"

ChestItemIcon::ChestItemIcon(int itemNum, int countGetItem, Chest* chest) :
	m_itemNum(itemNum),
	m_countGetItem(countGetItem),
	m_chest(chest)
{
	m_itemIconUi.Register(std::next(m_chest->GetItemList().begin(), m_countGetItem)->GetItemIconName());
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

	if (m_chest->DestroyItemIcon())
	{
		if (m_countGetItem == m_chest->DestroyTakeItem())
		{
			Destroy();
		}
		if (m_countGetItem >= m_chest->DestroyTakeItem())
		{
			m_countGetItem--;
		}

		m_chest->AddDestroyItemIconCount();
	}

	//Т▓Ро
	int verticalCount = m_countGetItem / 10;
	if (m_countGetItem >= 10)
	{
		int takeItem = m_countGetItem % 10;

		m_transform.position = SetPos + Vector2(90 * takeItem, 90 * verticalCount);
	}
	else
	{
		m_transform.position = SetPos + Vector2(90 * m_countGetItem, 0);
	}
}

void ChestItemIcon::Draw()
{
	m_itemIconUi.Draw(m_transform);
}