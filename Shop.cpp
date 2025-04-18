#include "Shop.h"
#include "Chest.h"
#include "Wallet.h"
#include "SellButton.h"
#include "ImageLoader.h"

Shop::Shop(Chest* chest,Wallet* wallet,SellButton* sellButton) :
	m_chest(chest),
	m_isSellItem(false),
	m_wallet(wallet),
	m_sellButton(sellButton)
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

	int takeItem = m_chest->GetTakeItem();	

	if (m_sellButton->GetCheckOnClick() && !m_chest->GetItemList().empty())
	{
		m_wallet->InWalletMoney(std::next(m_chest->GetItemList().begin(), takeItem)->GetSellMoney());

		m_chest->LostItem(takeItem);
	}
}

void Shop::Draw()
{
	m_shopUi.Draw(m_shopTransform);
	
}