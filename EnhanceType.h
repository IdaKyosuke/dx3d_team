#pragma once
#include "Inventory.h"

class Inventory;

class EnhanceType
{
private:
	bool m_onButton;

	int m_maxHaveInventory;	//インベントリの最大容量

	int m_maxTheWorldTime;	//時を止めれる最大時間
	int m_maxUseTheWorld;	//時を止めれる最大回数

public:

	EnhanceType(int maxHaveInventory,int maxTheWorldTime,int maxUseTheWorld) :
		m_onButton(false),
		m_maxHaveInventory(maxHaveInventory),
		m_maxTheWorldTime(maxTheWorldTime),
		m_maxUseTheWorld(maxUseTheWorld){};

	enum class EnhanceTypeChoice
	{
		EnhanceInventory,
		EnhanceTheWorldTime,

		Lenght
	};

	//強化
	void Enhance(EnhanceTypeChoice enhanceType)
	{
		switch (enhanceType)
		{
		case EnhanceTypeChoice::EnhanceInventory:
			m_maxHaveInventory++;
			m_onButton = true;
			break;

		case EnhanceTypeChoice::EnhanceTheWorldTime:
			m_maxTheWorldTime += 2;
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

	int GetMaxHaveInventory() const
	{
		return m_maxHaveInventory;
	}

	int GetMaxTheWorldTime() const
	{
		return m_maxTheWorldTime;
	}
};