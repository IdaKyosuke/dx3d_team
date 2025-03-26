#include "Item.h"

Item::Item(char itemName) : Actor("Item"),
m_itemNumder(itemName)
{
}

void Item::Update()
{
	Actor::Update();
}

void Item::ItemSearch()
{

}