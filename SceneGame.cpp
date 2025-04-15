#include "SceneGame.h"
#include"SceneTitle.h"
#include "Screen.h"
#include "Input.h"
#include "Node.h"
#include "Actor.h"
#include "Time.h"
#include "Fade.h"
#include "ImageLoader.h"
#include"LoadPlayer.h"
#include "Enemy.h"
#include"NavMesh.h"
#include"CollisionStage.h"
#include"ItemFactory.h"
#include"UiScore.h"
#include"UiResult.h"
#include "DxLib.h"

#include "Item.h"
#include "Inventory.h"

// 初期化
void SceneGame::Initialize()
{
	// 画像の事前読み込み
	for (auto image : ImagePreload)
	{
		ImageLoader::GetInstance()->Load(image);
	}

	// ルート
	m_rootNode = new Node();

	// アクターレイヤー
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UIレイヤー
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);

	// ステージの当たり判定を作成
	m_collisionStage = new CollisionStage("Resource/PathPlanning.mqo", Vector3(0, 0, 0));

	uiLayer->AddChild(m_collisionStage);

	// プレイヤー
	m_loadPlayer = new LoadPlayer(m_collisionStage);

	actorLayer->AddChild(m_loadPlayer);

	//インベントリ
	m_inventory = new Inventory(m_loadPlayer);
	uiLayer->AddChild(m_inventory);

	//アイテム
	m_item = new Item(0, Vector3(100, 50, 100), m_loadPlayer, m_inventory);
	actorLayer->AddChild(m_item);

	//アイテム
	m_item = new Item(1, Vector3(400, 50, 100), m_loadPlayer, m_inventory);
	actorLayer->AddChild(m_item);

	//アイテム
	m_item = new Item(2, Vector3(100, 50, 400), m_loadPlayer, m_inventory);
	actorLayer->AddChild(m_item);

	// ナビメッシュ
	m_navMesh = new NavMesh(m_collisionStage);
	
	// 敵
	m_enemy = new Enemy(m_navMesh, Vector3(800, 110, 10), m_loadPlayer);
	actorLayer->AddChild(m_enemy);
	
	// スコア
	m_uiScore = new UiScore();
	uiLayer->AddChild(m_uiScore);
	
	// アイテムの生成
	m_itemfactory = new ItemFactory(m_uiScore);
	actorLayer->AddChild(m_itemfactory);

	// リザルト画面
	m_uiResult = new UiResult(m_itemfactory);
	uiLayer->AddChild(m_uiResult);
}

// 終了
void SceneGame::Finalize()
{
	// 事前読み込みした画像の破棄
	for (auto image : ImagePreload)
	{
		ImageLoader::GetInstance()->Delete(image);
	}

	// プレイヤー情報の削除
	m_loadPlayer->Finalize();

	// ノードの削除
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;

	// navMesh情報の破棄
	m_navMesh->RemovePathPlan();
	m_navMesh->RemovePolyLinkInfo();
}

// 更新
SceneBase* SceneGame::Update()
{
	// ノードの更新
	m_rootNode->TreeUpdate();
	
	if (m_uiResult->IsShowedPress())
	{
		if (Input::GetInstance()->IsAnyKeyDown())
		{
			return new SceneTitle();
		}
	}

	return this;
}

// 描画
void SceneGame::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}
