#pragma once
#include "Inventory.h"

class Inventory;

class EnhanceType
{
private:
	bool m_onInventoryButton;
	bool m_onTheWorldButton;
	bool m_onStaminaButton;

	int m_maxHaveInventory;	//インベントリの最大容量

	int m_maxTheWorldTime;	//時を止めれる最大時間
	int m_maxUseTheWorld;	//時を止めれる最大回数

	float m_addStaminaRecovery;			//追加されるスタミナの回復量
	float m_alleviationStaminaDecrease;	//軽減されるされるスタミナの消費用

public:

	EnhanceType(int maxHaveInventory,
		int maxTheWorldTime,
		int maxUseTheWorld,
		float addStaminaRecovery,
		float alleviationStaminaDecrease) :
		m_onInventoryButton(false),
		m_onTheWorldButton(false),
		m_onStaminaButton(false),
		m_maxHaveInventory(maxHaveInventory),
		m_maxTheWorldTime(maxTheWorldTime),
		m_maxUseTheWorld(maxUseTheWorld),
		m_addStaminaRecovery(addStaminaRecovery),
		m_alleviationStaminaDecrease(alleviationStaminaDecrease){};

	enum class EnhanceTypeChoice
	{
		EnhanceInventory,
		EnhanceTheWorldTime,
		EnhanceStamina,

		Lenght
	};

	//強化
	void Enhance(EnhanceTypeChoice enhanceType)
	{
		switch (enhanceType)
		{
		case EnhanceTypeChoice::EnhanceInventory:
			m_maxHaveInventory++;
			m_onInventoryButton = true;
			break;

		case EnhanceTypeChoice::EnhanceTheWorldTime:
			m_maxTheWorldTime += 2;
			m_onTheWorldButton = true;
			break;

		case EnhanceTypeChoice::EnhanceStamina:
			m_addStaminaRecovery += 2;
			m_alleviationStaminaDecrease -= 2;
			m_onStaminaButton = true;
			break;
		}
	};

	bool OnInventoryButton()
	{
		return m_onInventoryButton;
	}

	bool OnTheWorldButton()
	{
		return m_onTheWorldButton;
	}

	bool OnStaminaButton()
	{
		return m_onStaminaButton;
	}

	void InventoryButtonReset()
	{
		m_onInventoryButton = false;
	}

	void TheWorldButtonReset()
	{
		m_onTheWorldButton = false;
	}

	void StaminaButtonReset()
	{
		m_onStaminaButton = false;
	}

	int GetMaxHaveInventory() const
	{
		return m_maxHaveInventory;
	}

	int GetMaxTheWorldTime() const
	{
		return m_maxTheWorldTime;
	}

	int GetMaxUseTheWorldCount() const
	{
		return m_maxUseTheWorld;
	}

	float GetAddStaminaRecovery() const
	{
		return m_addStaminaRecovery;
	}

	float GetAlleviationStaminaDecrease() const
	{
		return m_alleviationStaminaDecrease;
	}
};