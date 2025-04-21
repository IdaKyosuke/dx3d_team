#include "MenuItemIcon.h"
#include "MenuInventory.h"
#include "Inventory.h"
#include "Screen.h"

MenuItemIcon::MenuItemIcon(int itemNum, int countGetItem, MenuInventory* menuInventory) :
	m_itemNum(itemNum),
	m_countGetItem(countGetItem),
	m_menuInventory(menuInventory)
{
	m_itemIconUi.Register(m_iconName[m_itemNum]);
}

void MenuItemIcon::Load()
{
	m_itemIconUi.Load();
}

void MenuItemIcon::Release()
{
	m_itemIconUi.Release();
}

void MenuItemIcon::Update()
{
	m_itemIconUi.Update();

	if (m_menuInventory->DestoryItemIcon())
	{
		if (m_countGetItem == m_menuInventory->DestroyTakeItem())
		{
			Destroy();
		}
		if (m_countGetItem >= m_menuInventory->DestroyTakeItem())
		{
			m_countGetItem--;
		}
	}

	m_transform.position = SetPos + Vector2(90 * m_countGetItem, 0);
}

void MenuItemIcon::Draw()
{
	m_itemIconUi.Draw(m_transform);
}