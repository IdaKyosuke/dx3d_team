#include "EnhanceInventory.h"
#include "EnhanceType.h"
#include "Chest.h"
#include "Wallet.h"

EnhanceInventory::EnhanceInventory(Chest* chest,Wallet* wallet,EnhanceType* enhanceType) :
	Enhance(EnhanceType::EnhanceTypeChoice::EnhanceInventory,
		m_useItemNum,
		NeedMoney,
		Position,
		"enhance_button_ui.png",
		chest,
		enhanceType,
		wallet),
	m_chest(chest),
	m_canEnhance(false),
	m_needItemNum(0),
	m_needItemCount(0),
	m_useItemNum(0),
	m_enhanceType(enhanceType),
	m_wallet(wallet)
{
	m_needItemNum = 0;
}

//ボタンが有効かどうかチェック
bool EnhanceInventory::CheckCondition()
{
	if (!m_chest->GetItemList().empty())
	{
		if (m_needItemNum == std::next(m_chest->GetItemList().begin(),
			m_chest->GetTakeItem())->GetItemNum() &&
			m_wallet->HaveMenoy() >= NeedMoney)
		{
			m_canEnhance = true;
			m_useItemNum = m_chest->GetTakeItem();
		}
		else
		{
			m_canEnhance = false;
		}
	}
	else
	{
		m_canEnhance = false;
	}


	return m_canEnhance;
}
