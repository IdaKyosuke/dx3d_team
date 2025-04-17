#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "MenuInventory.h"
#include "Chest.h"
#include "Shop.h"
#include "ShopButton.h"
#include "Wallet.h"
#include "SellButton.h"
#include "EnhanceShopButton.h"
#include "EnhanceShop.h"

#include "EnhanceType.h"
#include "EnhanceInventory.h"

#include"Inventory.h"

void SceneMenu::Initialize()
{
	m_rootNode = new Node();

	// アクターレイヤー
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UIレイヤー
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);

	//チェスト
	m_chest = new Chest();
	uiLayer->AddChild(m_chest);
	//メニューのインベントリ
	m_menuInventory = new MenuInventory(m_chest);
	uiLayer->AddChild(m_menuInventory);
	
	//売るボタン
	m_sellButton = new SellButton();
	uiLayer->AddChild(m_sellButton);

	//ショップ切り替えボタン
	m_shopButton = new ShopButton();
	uiLayer->AddChild(m_shopButton);
	//強化ショップ切り替えボタン
	m_enhanceShopButton = new EnhanceShopButton();
	uiLayer->AddChild(m_enhanceShopButton);

	//財布
	m_wallet = new Wallet();
	uiLayer->AddChild(m_wallet);

	//ショップ
	m_shop = new Shop(m_chest,m_wallet,m_sellButton, m_shopButton);
	uiLayer->AddChild(m_shop);
	

	m_enhanceType = new EnhanceType(m_inventory,m_maxHaveItem);

	//強化ストア
	m_enhanceShop = new EnhanceShop(m_chest);
	uiLayer->AddChild(m_enhanceShop);
	
	//強化ボタン
	uiLayer->AddChild(new EnhanceInventory(m_chest,m_wallet,m_enhanceType));


	if (!m_inventory->TakeItMenu().empty())
	{
		for (int i = 0; i <= m_inventory->TakeItMenu().size() - 1; i++)
		{
			//持って帰ったアイテムを格納する
			m_menuInventory->Change(std::next(m_inventory->TakeItMenu().begin(), i)->GetItemNum());
		}
	}

	if (!m_chestItem.empty())
	{
		for (int i = 0; i <= m_chestItem.size() - 1; i++)
		{
			//チェストに保管していたアイテム
			m_chest->Change(std::next(m_chestItem.begin(), i)->GetItemNum());
			m_chest->CreateIcon(i);
		}
	}

}

void SceneMenu::Finalize()
{
	// ノードの削除
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneMenu::Update()
{
	// ノードの更新
	m_rootNode->TreeUpdate();

	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
	{
		return new SceneGame(m_chest->GetItemList(), m_enhanceType->GetMaxHaveInventory());
	}

	return this;
}

void SceneMenu::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}