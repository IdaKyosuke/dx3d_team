#include "SceneMenu.h"
#include "SceneGame.h"
#include "SceneGameOver.h"
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
#include "EnhanceHaveWeight.h"
#include "EnhanceTheWorldTime.h"
#include "EnhanceUseTheWorld.h"
#include "EnhanceStaminaRecovery.h"
#include "EnhanceStaminaDecrease.h"
#include "MoneyCount.h"

#include"Inventory.h"

#include "Item.h"

#include "SceneTitle.h"

void SceneMenu::Initialize()
{
	m_rootNode = new Node();

	// アクターレイヤー
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UIレイヤー
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);	

	//強化の種類＋強化した内容の保持
	m_maxHaveInventory = m_enhanceType->GetMaxHaveInventory();
	m_maxHaveWeight = m_enhanceType->GetMaxHaveWeight();
	m_theWorldTime = m_enhanceType->GetMaxTheWorldTime();
	m_useCountTheWorld = m_enhanceType->GetMaxUseTheWorldCount();
	m_staminaRecovery = m_enhanceType->GetStaminaRecovery();
	m_staminaDecrease = m_enhanceType->GetStaminaDecrease();
	m_enhanceType = new EnhanceType(m_maxHaveInventory, m_maxHaveWeight, m_theWorldTime, m_useCountTheWorld, m_staminaRecovery, m_staminaDecrease);

	//チェスト
	m_chest = new Chest();
	uiLayer->AddChild(m_chest);
	//メニューのインベントリ
	m_menuInventory = new MenuInventory(m_chest,m_enhanceType);
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
	
	//強化ボタンインベントリの最大
	m_enhanceInventory = new EnhanceInventory(m_chest, m_wallet, m_enhanceType);
	uiLayer->AddChild(m_enhanceInventory);
	//強化ボタン持てる最大容量
	m_enhanceHaveWeight = new EnhanceHaveWeight(m_chest, m_wallet, m_enhanceType);
	uiLayer->AddChild(m_enhanceHaveWeight);
	//強化ボタンザワールド(時間)
	m_enhanceTheWorldTime = new EnhanceTheWorldTime(m_chest,m_wallet,m_enhanceType);
	uiLayer->AddChild(m_enhanceTheWorldTime);
	//強化ボタンザワールド(回数)
	m_enhanceUseTheWorld = new EnhanceUseTheWorld(m_chest, m_wallet, m_enhanceType);
	uiLayer->AddChild(m_enhanceUseTheWorld);
	//強化ボタンスタミナ
	m_enhanceStaminaRecovery = new EnhanceStaminaRecovery(m_chest, m_wallet, m_enhanceType);
	uiLayer->AddChild(m_enhanceStaminaRecovery);
	//強化ボタンスタミナ
	m_enhanceStaminaDecrease = new EnhanceStaminaDecrease(m_chest, m_wallet, m_enhanceType);
	uiLayer->AddChild(m_enhanceStaminaDecrease);

	m_restDays = m_moneyCount->GetRestDays();
	m_clearCount = m_moneyCount->GetClearCount();
	//残り日数の表示、必要金額の表示
	m_moneyCount = new MoneyCount(m_wallet, m_restDays, m_clearCount);
	uiLayer->AddChild(m_moneyCount);
	m_moneyCount->BackMenu();

	if (!m_inventory->GetItemList().empty())
	{
		
		int i = 0;
		for (Item item : m_inventory->GetItemList())
		{
			m_item = new Item(
				m_inventory->GetItemList()[i].GetItemNum(),
				m_inventory->GetItemList()[i].GetItemData()
			);
			m_menuInventory->AddAdvanceItemList(m_item);
			i++;
		}
	}

	if (!m_chestItem.empty())
	{
		for (int i = 0; i <= m_chestItem.size() - 1; i++)
		{
			//チェストに保管していたアイテム
			m_chest->AddAdvanceItemList(m_chestItem[i].GetItemNum(), m_chestItem[i].GetItemData());
		}
	}

	// マウスカーソルを表示する
	SetMouseDispFlag(true);
}

void SceneMenu::Finalize()
{
	// ノードの削除
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	DeleteGraph(m_clearMovieHandle);
}

SceneBase* SceneMenu::Update()
{
	// ノードの更新
	m_rootNode->TreeUpdate();

	bool isClear = false;

	if (m_moneyCount->GetRestDays() <= 0)
	{

		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
		{
			WaitTimer(200);

			if (m_moneyCount->GetClearCount() >= NeedClearCount)
			{
				//7週目をクリアした時のムービーのロード
				m_clearMovieHandle = LoadGraph("Resource/movie/gameClear.mp4");

				m_wallet->LostMoney(m_moneyCount->GetNeedMoney());
				PlayMovieToGraph(m_clearMovieHandle);

				isClear = true;
			}

			if (m_moneyCount->GetTaskClear() && !m_clearFlag && !isClear)
			{

				//週のノルマをクリアした時のムービーのロード
				m_clearMovieHandle = LoadGraph("Resource/movie/weekClear.mp4");

				m_wallet->LostMoney(m_moneyCount->GetNeedMoney());
				PlayMovieToGraph(m_clearMovieHandle);

				m_clearFlag = true;
			}

			if (!m_moneyCount->GetTaskClear())
			{
				return new SceneGameOver();
			}
		}

		if (GetMovieStateToGraph(m_clearMovieHandle) == 0 && m_clearFlag)
		{
			return new SceneGame(m_chest->GetItemList(), m_enhanceType, m_wallet->HaveMoney(), m_moneyCount);
		}

		if (GetMovieStateToGraph(m_clearMovieHandle) == 0)
		{
			if (Input::GetInstance()->IsAnyKeyDown())
			{
				return new SceneTitle();
			}
		}

	}
	else
	{
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
		{
			m_haveMoney = m_wallet->HaveMoney();
			return new SceneGame(m_chest->GetItemList(), m_enhanceType, m_haveMoney, m_moneyCount);
		}
	}

	return this;
}

void SceneMenu::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();

	DrawExtendGraph(0, 0, 1280, 960, m_clearMovieHandle, FALSE);
}