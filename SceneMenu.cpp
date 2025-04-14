#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "MenuInventory.h"
#include "Chest.h"

#include"Inventory.h"
#include "LoadPlayer.h"
#include "CollisionStage.h"

void SceneMenu::Initialize()
{
	m_rootNode = new Node();

	// アクターレイヤー
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UIレイヤー
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);
	/*
	// タイトルロゴ
	m_rootNode->AddChild(new Actor(
		"Logo",
		"game_title.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));
	*/

	//チェストとインベントリ
	m_menuInventory = new MenuInventory(m_chest);
	m_chest = new Chest(m_menuInventory);
	uiLayer->AddChild(m_chest);

	m_menuInventory = new MenuInventory(m_chest);
	uiLayer->AddChild(m_menuInventory);


	if (!m_inventory->TakeItMenu().empty())
	{
		for (int i = 0; i <= m_inventory->TakeItMenu().size() - 1; i++)
		{
			int getItemNum = std::next(m_inventory->TakeItMenu().begin(),i)->GetItemNum();

			//持って帰ったアイテムをスワップする
			m_menuInventory->BroughtItMenu(getItemNum,i);
		}
	}

	m_collisionStage = new CollisionStage("Resource/stage.mv1", Vector3(0, 0, 0));
	m_loadPlayer = new LoadPlayer(m_collisionStage);
	//m_inventory = new Inventory(m_loadPlayer);

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
		return new SceneGame();
	}

	return this;
}

void SceneMenu::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}