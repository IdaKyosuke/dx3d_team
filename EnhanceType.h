#pragma once
#include "Inventory.h"

class Inventory;

class EnhanceType
{
private:
	bool m_onInventoryButton;
	bool m_onTheWorldTimeButton;
	bool m_onUseTheWorldButton;
	bool m_onStaminaRecoveryButton;
	bool m_onStaminaDecreaseButton;

	int m_maxHaveInventory;	//インベントリの最大容量

	float m_maxTheWorldTime;	//時を止めれる最大時間
	int m_maxUseTheWorld;	//時を止めれる最大回数

	float m_staminaRecovery;	//追加されるスタミナの回復量
	float m_staminaDecrease;	//軽減されるされるスタミナの消費用

public:

	EnhanceType(int maxHaveInventory,
		float maxTheWorldTime,
		int maxUseTheWorld,
		float staminaRecovery,
		float staminaDecrease) :
		m_onInventoryButton(false),
		m_onTheWorldTimeButton(false),
		m_onUseTheWorldButton(false),
		m_onStaminaRecoveryButton(false),
		m_onStaminaDecreaseButton(false),
		m_maxHaveInventory(maxHaveInventory),
		m_maxTheWorldTime(maxTheWorldTime),
		m_maxUseTheWorld(maxUseTheWorld),
		m_staminaRecovery(staminaRecovery),
		m_staminaDecrease(staminaDecrease){};

	enum class EnhanceTypeChoice
	{
		EnhanceInventory,
		EnhanceTheWorldTime,
		EnhanceUseTheWorld,
		EnhanceStaminaRecovery,
		EnhanceStaminaDecrease,

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
			m_onTheWorldTimeButton = true;
			break;

		case EnhanceTypeChoice::EnhanceUseTheWorld:
			m_maxUseTheWorld++;
			m_onUseTheWorldButton = true;
			break;

		case EnhanceTypeChoice::EnhanceStaminaRecovery:
			m_staminaRecovery += 2;
			m_onStaminaRecoveryButton = true;
			break;

		case EnhanceTypeChoice::EnhanceStaminaDecrease:
			m_staminaDecrease -= 2;
			m_onStaminaDecreaseButton = true;
			break;
		}
	};

	//ボタン押されたのを返す
	bool OnInventoryButton()
	{
		return m_onInventoryButton;
	}

	bool OnTheWorldTimeButton()
	{
		return m_onTheWorldTimeButton;
	}

	bool OnUseTheWorldButton()
	{
		return m_onUseTheWorldButton;
	}

	bool OnStaminaRecoveryButton()
	{
		return m_onStaminaRecoveryButton;
	}

	bool OnStaminaDecreaseButton()
	{
		return m_onStaminaDecreaseButton;
	}

	//ボタンリセット用
	void InventoryButtonReset()
	{
		m_onInventoryButton = false;
	}

	void TheWorldButtonTimeReset()
	{
		m_onTheWorldTimeButton = false;
	}

	void UseTheWorldButtonReset()
	{
		m_onUseTheWorldButton = false;
	}

	void StaminaRecoveryButtonReset()
	{
		m_onStaminaRecoveryButton = false;
	}

	void StaminaDecreaseButtonReset()
	{
		m_onStaminaDecreaseButton = false;
	}

	//強化した値を返す
	int GetMaxHaveInventory() const
	{
		return m_maxHaveInventory;
	}

	float GetMaxTheWorldTime() const
	{
		return m_maxTheWorldTime;
	}

	int GetMaxUseTheWorldCount() const
	{
		return m_maxUseTheWorld;
	}

	float GetStaminaRecovery() const
	{
		return m_staminaRecovery;
	}

	float GetStaminaDecrease() const
	{
		return m_staminaDecrease;
	}
};