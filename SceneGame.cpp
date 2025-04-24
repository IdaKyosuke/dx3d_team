#include "SceneGame.h"
#include"SceneTitle.h"
#include "SceneMenu.h"
#include "Screen.h"
#include "Input.h"
#include "Node.h"
#include "Actor.h"
#include "HitBox.h"
#include "Time.h"
#include"Fade.h"
#include"ImageLoader.h"
#include"LoadPlayer.h"
#include"Collision3D.h"
#include"ItemFactory.h"
#include"EnemyFactory.h"
#include"UiScore.h"
#include"UiResult.h"
#include"Inventory.h"
#include"CollisionStage.h"
#include"NavMesh.h"
#include"DrawStageView.h"
#include"EscapePoint.h"
#include"ScreenFilter.h"
#include "MoneyCount.h"
#include "EnhanceType.h"
#include "DxLib.h"

#include "Chest.h"
#include "KeepChest.h"

#include "Item.h"


// 初期化
void SceneGame::Initialize()
{
	// ルート
	m_rootNode = new Node();

	// アクターレイヤー
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UIレイヤー
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);
	/*
	// ステージの見た目を描画
	m_drawStageView = new DrawStageView("nav_stage_test_view.mv1");
	uiLayer->AddChild(m_drawStageView);
	*/
	// ステージの見た目を描画
	m_drawStageView = new DrawStageView("favorite_stage.mv1");
	uiLayer->AddChild(m_drawStageView);

	// ステージの当たり判定を作成
	m_collisionStage = new CollisionStage("Resource/nav_test_stage.mv1", Vector3(0, 0, 0));
	uiLayer->AddChild(m_collisionStage);

	// navMesh
	m_navMesh = new NavMesh(m_collisionStage);

	m_maxHaveInventory = m_enhanceType->GetMaxHaveInventory();
	m_theWorldTime = m_enhanceType->GetMaxTheWorldTime();
	m_useCountTheWorld = 0;

	m_enhanceType = new EnhanceType(m_maxHaveInventory,m_theWorldTime, m_useCountTheWorld);

	//インベントリ
	m_inventory = new Inventory(m_maxHaveInventory);
	uiLayer->AddChild(m_inventory);

	// プレイヤー
	m_loadPlayer = new LoadPlayer(m_collisionStage,m_inventory);
	actorLayer->AddChild(m_loadPlayer);

	/*
	//アイテム
	m_item = new Item(0, Vector3(100, 50, 100),m_inventory);
	actorLayer->AddChild(m_item);
	//アイテム
	m_item = new Item(0, Vector3(500, 50, 100),m_inventory);
	actorLayer->AddChild(m_item);
	//アイテム
	m_item = new Item(0, Vector3(500, 50, 100), m_inventory);
	actorLayer->AddChild(m_item);
	//アイテム
	m_item = new Item(0, Vector3(500, 50, 100), m_inventory);
	actorLayer->AddChild(m_item);

	//アイテム
	m_item = new Item(1, Vector3(400, 50, 100),m_inventory);
	actorLayer->AddChild(m_item);

	//アイテム
	m_item = new Item(1, Vector3(400, 50, 300), m_inventory);
	actorLayer->AddChild(m_item);
	*/

	//アイテム
	m_item = new Item(1, Vector3(300, 0, 0),m_inventory,m_loadPlayer);
	actorLayer->AddChild(m_item);//アイテム
	m_item = new Item(0, Vector3(300, 0, 0),m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);

	m_item = new Item(3, Vector3(300, 0, 0), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);
	m_item = new Item(4, Vector3(300, 0, 200), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);
	m_item = new Item(0, Vector3(300, 0, 500), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);

	// スコア
	m_uiScore = new UiScore();
	uiLayer->AddChild(m_uiScore);

	// アイテムの生成
	m_itemfactory = new ItemFactory(m_uiScore, m_inventory, m_navMesh, m_loadPlayer);
	actorLayer->AddChild(m_itemfactory);
	
	// 敵の生成
	m_enemyFactory = new EnemyFactory(actorLayer, m_navMesh, m_loadPlayer);
	
	// 脱出地点の作成
	m_escapePoint = new EscapePoint();
	actorLayer->AddChild(m_escapePoint);

	// リザルト画面
	m_uiResult = new UiResult(m_itemfactory);
	uiLayer->AddChild(m_uiResult);

	m_restDays = m_moneyCount->GetRestDays();
	m_moneyCount = new MoneyCount(m_wallet, m_restDays, 0);
	m_keepChest = new KeepChest;
	
	//チェストとインベントリ
	if (!m_chestItem.empty())
	{

		int haveItemCount = static_cast<int>(m_chestItem.size());

		for (int i = 0; i <= haveItemCount - 1; i++)
		{
			m_keepChest->SetItemList(std::next(m_chestItem.begin(), i)->GetItemNum());
		}
	}

	// スクリーンに掛けるフィルター
	m_screenFilter = new ScreenFilter(m_loadPlayer);
	uiLayer->AddChild(m_screenFilter);
}

// 終了
void SceneGame::Finalize()
{
	// プレイヤー情報の削除
	m_loadPlayer->Finalize();

	// ポリゴンの連結情報を破棄
	m_navMesh->RemovePolyLinkInfo();

	// ノードの削除
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

// 更新
SceneBase* SceneGame::Update()
{
	// ノードの更新
	m_rootNode->TreeUpdate();

	m_limitTime -= Time::GetInstance()->GetDeltaTime();
	if (m_limitTime <= 0)
	{
		return new SceneMenu(m_keepChest->TakeItMenu(), m_inventory, m_enhanceType, m_haveMoney, m_moneyCount);
	}
	
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_T))
	{
		return new SceneTitle();
	}
	

	if (m_escapePoint->IsEscape())
	{
		return new SceneMenu(m_keepChest->TakeItMenu(), m_inventory,m_enhanceType,m_haveMoney,m_moneyCount);
	}
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
	{
		return new SceneMenu(m_keepChest->TakeItMenu(), m_inventory, m_enhanceType, m_haveMoney, m_moneyCount);
	}

	return this;
}

// 描画
void SceneGame::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}
