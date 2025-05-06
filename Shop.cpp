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
	m_dontSellFlag(false),
	m_seSell(0)
{
	//Uiの登録
	m_shopUi.Register("shop_ui.png");

	m_shopTransform.position = ShopUiPos;
}

void Shop::Load()
{
	//seを設定
	m_seSell = LoadSoundMem("Resource/sound/sell_se.mp3");
	ChangeVolumeSoundMem(128, m_seSell);

	m_shopUi.Load();
}

void Shop::Release()
{
	DeleteSoundMem(m_seSell);
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

		PlaySoundMem(m_seSell, DX_PLAYTYPE_BACK);
	}
}

void Shop::Draw()
{
	m_shopUi.Draw(m_shopTransform);
	

	SetFontSize(35);

	DrawString(5, 920,
		"WASD:移動　R:インベントリからチェストへ　C:操作切り替え",
		GetColor(255, 255, 255));

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
	}
}