#include "Inventory.h""
#include "Screen.h"
#include "ImageLoader.h"

Inventory::Inventory() :
	m_haveItemCount(0),
	m_canGetItem(false),
	m_itemNum(0)
{
	m_transform.position = Screen::BottomCenter + Vector2(-400,-70);
	m_Icontransform.position = Screen::Center;
	m_inventoryUi.Register("inventoryUi.png");
	m_iconUi.Register(m_iconName[m_itemNum]);
}

void Inventory::Load()
{
	m_inventoryUi.Load();
	m_iconUi.Load();
}

void Inventory::Release()
{
	m_inventoryUi.Release();
	m_iconUi.Release();
}

void Inventory::Update()
{
	m_inventoryUi.Update();
	m_iconUi.Update();

	

	if (m_haveItemCount >= MaxHaveItem)
	{
		m_canGetItem = false;
	}
	else
	{
		m_canGetItem = true;
	}

	if (m_haveItemCount >= 1)
	{

		m_itemNum = m_itemList.begin()->GetItemNum();

		//m_iconUi.Register(m_iconName[m_itemList.begin()->GetItemNum()]);
		


		for (int i = 0; i <= MaxHaveItem; i++)
		{
			//	m_iconName[0] = m_itemList.begin()-> GetIconName() + i;
		}
	}	
}

void Inventory::Draw()
{
	m_inventoryUi.Draw(m_transform);
	m_iconUi.Draw(m_Icontransform);

	
	DrawFormatString(0, 300, GetColor(255, 255, 255),
		"m_itemNum(%)",
		m_itemNum
	);
	//DrawString(0, 230, m_iconName[1], GetColor(255, 255, 255));
}