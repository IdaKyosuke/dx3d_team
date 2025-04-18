#pragma once
#include "Inventory.h"

class Inventory;

class EnhanceType
{
private:

	Inventory* m_inventory;

	bool m_onButton;

	int m_maxHaveInventory;

public:

	EnhanceType(Inventory* inventory,int i) :
		m_inventory(inventory),
		m_onButton(false),
		m_maxHaveInventory(i){
	};

	enum class EnhanceTypeChoice
	{
		EnhanceInventory
	};

	//‹­‰»
	void Enhance(EnhanceTypeChoice enhanceType)
	{
		switch (enhanceType)
		{
		case EnhanceTypeChoice::EnhanceInventory:
			m_maxHaveInventory++;
			m_onButton = true;
			break;
		}
	};

	bool OnButton()
	{
		return m_onButton;
	}

	void ButtonReset()
	{
		m_onButton = false;
	}

	int GetMaxHaveInventory()
	{
		return m_maxHaveInventory;
	}
};