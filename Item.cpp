#include "Item.h"
#include "LoaderBase.h"

Item::Item(int itemNumber) : Actor("Item"),
	m_itemNumber(itemNumber)
{
	
}

void Item::Load()
{

}

void Item::Release()
{

}

void Item::Update()
{
	

	Actor::Update();
}

void Item::ItemSearch()
{
	 itemDate[m_itemNumber].m_modelName;
}