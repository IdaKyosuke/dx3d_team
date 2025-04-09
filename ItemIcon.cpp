#include "ItemIcon.h"
<<<<<<< HEAD
#include "Screen.h"

ItemIcon::ItemIcon(int itemNum, int countGetItem) :
=======
#include "Screen.h"	

ItemIcon::ItemIcon(int itemNum,int countGetItem) :
>>>>>>> origin/item
	m_itemNum(itemNum),
	m_countGetItem(countGetItem)
{
	m_itemIconUi.Register(m_iconName[m_itemNum]);
	m_transform.position = SetPos[m_countGetItem];
}

<<<<<<< HEAD


=======
>>>>>>> origin/item
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
}

void ItemIcon::Draw()
{
	m_itemIconUi.Draw(m_transform);
}