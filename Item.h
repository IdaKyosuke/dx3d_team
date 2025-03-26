#pragma once
#include "ItemDate.h"
#include "Actor.h" 
#include <list>

class Item : public Actor
{
private:
	int m_itemNumber;

	enum class ItemName
	{
		Diamond,

		Length,
	};

	const char* ItemNameList[static_cast<int>(ItemName::Length)] =
	{
		"Diamond"
	};

		const ItemDate itemDate[static_cast<int>(ItemName::Length)] =
	{
		ItemDate("Diamond.mv1",100)
	};


protected:
	virtual void Load();
	virtual void Release();
	virtual void Update();

public:
	Item(int itemNumber);

	void ItemSearch();
};