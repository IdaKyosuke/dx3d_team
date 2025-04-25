#include "Shop.h"
#include "Chest.h"
#include "Wallet.h"
#include "SellButton.h"
#include "ImageLoader.h"

Shop::Shop(Chest* chest,Wallet* wallet,SellButton* sellButton) :
	m_chest(chest),
	m_isSellItem(false),
	m_wallet(wallet),
	m_sellButton(sellButton),
	m_takeItem(0),
	m_dontSellFlag(false)
{
	//Ui‚Ì“o˜^
	m_shopUi.Register("shop_ui.png");

	m_shopTransform.position = ShopUiPos;
}

void Shop::Load()
{
	m_shopUi.Load();
}

void Shop::Release()
{
	m_shopUi.Release();
}

void Shop::Update()
{
	m_shopUi.Update();	

	m_takeItem = m_chest->GetTakeItem();	

	if (m_sellButton->GetCheckOnClick() && !m_chest->GetItemList().empty())
	{	
		m_wallet->InWalletMoney(std::next(m_chest->GetItemList().begin(), m_takeItem)->GetSellMoney());

		m_chest->LostItem(m_chest->GetTakeItem());
	}
}

void Shop::Draw()
{
	m_shopUi.Draw(m_shopTransform);
	
	if (!m_chest->GetItemList().empty())
	{
		DrawString(1100, 300,
			std::next(m_chest->GetItemList().begin(), m_chest->GetTakeItem())->GetItemName()
			, GetColor(255, 255, 255));

		DrawFormatString(1100, 350,
			GetColor(255, 255, 255),
			"%d $", 
			std::next(m_chest->GetItemList().begin(), m_chest->GetTakeItem())->GetSellMoney());

		DrawFormatString(1100, 400,
			GetColor(255, 255, 255),
			"%d kg",
			std::next(m_chest->GetItemList().begin(), m_chest->GetTakeItem())->GetItemWeight());

		SetFontSize(35);
	}
}