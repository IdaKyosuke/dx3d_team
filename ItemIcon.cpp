#include "ItemIcon.h"
#include "Screen.h"

ItemIcon::ItemIcon(int itemNum) :
	m_itemNum(itemNum)
{
	m_Icontransform.position = Screen::Center;
	m_iconUi.Register(m_iconName[m_itemNum]);
}

void ItemIcon::Load()
{
	m_iconUi.Load();
}

void ItemIcon::Release()
{
	m_iconUi.Release();
}

void ItemIcon::Update()
{
	m_iconUi.Update();
}

void ItemIcon::Draw()
{
	m_iconUi.Draw(m_Icontransform);
}