#pragma once
#include "ItemDate.h"
#include "Actor.h" 
#include <list>

class Item : public Actor
{
private:
	char m_itemNumder;

	enum class itemName
	{
		Diamond,

		Length,
	};

	const char* ItemName[static_cast<int>(itemName::Length)] =
	{
		"Diamond"
	};

		const ItemDate itemDate[static_cast<int>(itemName::Length)] =
	{
		ItemDate("Diamond.mv1",100)
	};


protected:
	virtual void Update();

public:
	Item(char itemName);

	void ItemSearch();
};