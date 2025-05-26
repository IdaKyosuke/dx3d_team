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
#include"UiResult.h"
#include"UiTime.h"
#include "UiStamina.h"
#include"Inventory.h"
#include "Map.h"
#include"CollisionStage.h"
#include"NavMesh.h"
#include"DrawStageView.h"
#include"EscapePoint.h"
#include"ScreenFilter.h"
#include "MoneyCount.h"
#include "EnhanceType.h"
#include"LightFactory.h"
#include"Camera.h"
#include"UiTheWorld.h"
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
	
	// ステージの見た目を描画
	m_drawStageView = new DrawStageView("stage_view_2.mv1");
	uiLayer->AddChild(m_drawStageView);

	// ステージの当たり判定を作成
	m_collisionStage = new CollisionStage("Resource/nav_test_stage.mv1", "Resource/stage_wall.mv1", Vector3(0, 0, 0));
	uiLayer->AddChild(m_collisionStage);

	// navMesh
	m_navMesh = new NavMesh(m_collisionStage);

	//強化の種類＋強化した内容の保持
	m_maxHaveInventory = m_enhanceType->GetMaxHaveInventory();
	m_maxHaveWeight = m_enhanceType->GetMaxHaveWeight();
	m_theWorldTime = m_enhanceType->GetMaxTheWorldTime();
	m_useCountTheWorld = m_enhanceType->GetMaxUseTheWorldCount();
	m_staminaRecovery = m_enhanceType->GetStaminaRecovery();
	m_staminaDecrease = m_enhanceType->GetStaminaDecrease();
	m_enhanceType = new EnhanceType(m_maxHaveInventory, m_maxHaveWeight, m_theWorldTime, m_useCountTheWorld,m_staminaRecovery,m_staminaDecrease);

	//インベントリ
	m_inventory = new Inventory(m_enhanceType);
	uiLayer->AddChild(m_inventory);

	// アイテムの生成
	m_itemfactory = new ItemFactory(m_inventory, m_navMesh);

	// マップ
	m_map = new Map();
	uiLayer->AddChild(m_map);

	// プレイヤーのスポーン地点を作成
	int index = rand() % PointNum;
	m_escapePointIndex = index;
	
	// プレイヤー
	m_loadPlayer = new LoadPlayer(m_collisionStage,m_inventory, m_enhanceType, pos[index], m_itemfactory);
	actorLayer->AddChild(m_loadPlayer);
	
	// カメラ
	m_cam = new Camera(m_loadPlayer, m_collisionStage);
	actorLayer->AddChild(m_cam);

	// ライトを作成
	m_lightFactory = new LightFactory(m_cam, m_loadPlayer);
	actorLayer->AddChild(m_lightFactory);
	/*
#ifdef _DEBUG
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
	m_item = new Item(2, Vector3(500, 0, 200), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);
	m_item = new Item(6, Vector3(500, 0, 500), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);
	m_item = new Item(7, Vector3(400, 0, 500), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);

#endif // _DEBUG
	*/

	// 制限時間
	m_uiTime = new UiTime();
	uiLayer->AddChild(m_uiTime);

	actorLayer->AddChild(m_itemfactory);
	// ザ・ワールドのアイコン
	m_uiTheWorld = new UiTheWorld(m_loadPlayer);
	uiLayer->AddChild(m_uiTheWorld);

	//スタミナ
	m_uiStamina = new UiStamina(m_loadPlayer);
	uiLayer->AddChild(m_uiStamina);
	
	// 敵の生成
	m_enemyFactory = new EnemyFactory(actorLayer, m_navMesh, m_loadPlayer);
	

	// 脱出地点を作成
	do
	{
		index = rand() % PointNum;
	} while (m_escapePointIndex == index);

	// 脱出地点の作成
	m_escapePoint = new EscapePoint(pos[index], Rotate[index]);
	actorLayer->AddChild(m_escapePoint);

	// リザルト画面
	m_uiResult = new UiResult(m_itemfactory);
	uiLayer->AddChild(m_uiResult);

	m_restDays = m_moneyCount->GetRestDays();
	m_clearcount = m_moneyCount->GetClearCount();
	m_moneyCount = new MoneyCount(m_wallet, m_restDays, m_clearcount);
	m_keepChest = new KeepChest;
	
	//チェストとインベントリ
	if (!m_chestItem.empty())
	{
		int haveItemCount = static_cast<int>(m_chestItem.size());

		for (int i = 0; i <= haveItemCount - 1; i++)
		{
			m_keepChest->SetItemList(new Item(
				m_chestItem[i].GetItemNum(),
				m_itemfactory->GetItemData(m_chestItem[i].GetItemNum()))
			);
		}
	}

	// スクリーンに掛けるフィルター
	m_screenFilter = new ScreenFilter(m_loadPlayer);
	uiLayer->AddChild(m_screenFilter);

	// マウスカーソルを非表示にする
	SetMouseDispFlag(false);
}

// 終了
void SceneGame::Finalize()
{
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

	if (m_uiTime->IsFinsh() || m_loadPlayer->GetIsDeath())
	{
		m_inventory->ItemClear();
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
#ifdef _DEBUG
#endif // _DEBUG
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_TAB))
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
