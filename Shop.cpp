#include "Shop.h"
#include "Chest.h"
#include "Wallet.h"
#include "SellButton.h"
#include "ShopButton.h"

Shop::Shop(Chest* chest,Wallet* wallet,SellButton* sellButton,ShopButton* shopButton) :
	m_chest(chest),
	m_isSellItem(false),
	m_wallet(wallet),
	m_sellButton(sellButton),
	m_shopButton(shopButton)
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
	if (m_shopButton->GetCheckOnClick())
	{
		m_shopUi.Update();
	}

	int sellItem = m_chest->GetTakeItem();	

	if (m_sellButton->GetCheckOnClick() && !m_chest->GetItemList().empty())
	{
		m_wallet->InWalletMoney(std::next(m_chest->GetItemList().begin(),sellItem)->GetSellMoney());

		m_chest->LostItem(sellItem);
	}
}

void Shop::Draw()
{
	if(m_shopButton->GetCheckOnClick())
	{
		m_shopUi.Draw(m_shopTransform);
	}
}