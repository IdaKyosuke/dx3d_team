#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "PressAny.h"

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

	// タイトルロゴ
	m_rootNode->AddChild(new Actor(
		"Logo",
		"game_title.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));

	// Press Any Button
	m_rootNode->AddChild(new PressAny());

	/*
	// ステージの当たり判定を作成
	m_collisionStage = new CollisionStage("Resource/stage.mv1", Vector3(0, 0, 0));
	uiLayer->AddChild(m_collisionStage);

	// プレイヤー
	m_loadPlayer = new LoadPlayer(m_collisionStage);
	actorLayer->AddChild(m_loadPlayer);

	//インベントリ
	m_inventory = new Inventory(m_loadPlayer);
	uiLayer->AddChild(m_inventory);
	*/
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

	if (Input::GetInstance()->IsAnyKeyDown())
	{
		return new SceneGame();
	}

	return this;
}

void SceneMenu::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}