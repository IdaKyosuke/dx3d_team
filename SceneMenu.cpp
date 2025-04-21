#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "MenuInventory.h"
#include "Chest.h"
#include "Shop.h"
#include "Wallet.h"
#include "SellButton.h"
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

	//財布
	m_wallet = new Wallet();
	uiLayer->AddChild(m_wallet);
	m_wallet->InWalletMoney(m_haveMoney);

	//ショップ
	m_shop = new Shop(m_chest,m_wallet,m_sellButton);
	uiLayer->AddChild(m_shop);
	
	//強化の種類
	m_enhanceType = new EnhanceType(m_inventory,m_maxHaveItem);
	
	//強化ボタン
	m_enhanceInventory = new EnhanceInventory(m_chest, m_wallet, m_enhanceType);
	uiLayer->AddChild(m_enhanceInventory);
	
	

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
		m_haveMoney = m_wallet->HaveMoney();

		return new SceneGame(m_chest->GetItemList(), m_enhanceType->GetMaxHaveInventory(),m_haveMoney);
	}

	return this;
}

void SceneMenu::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}