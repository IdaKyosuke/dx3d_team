#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "PressAny.h"
#include "MenuInventory.h"
#include "Chest.h"

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
	/*
	// タイトルロゴ
	m_rootNode->AddChild(new Actor(
		"Logo",
		"game_title.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));
	*/

	// Press Any Button
	m_rootNode->AddChild(new PressAny());

	//チェストとインベントリ
	m_menuInventory = new MenuInventory(m_chest);
	m_chest = new Chest(m_menuInventory);
	uiLayer->AddChild(m_chest);

	m_menuInventory = new MenuInventory(m_chest);
	uiLayer->AddChild(m_menuInventory);

	auto getItemCount = std::distance(m_inventory->TakeItMenu().begin(), m_inventory->TakeItMenu().end());

	if (getItemCount >= 0)
	{
	//持って帰ったアイテムをスワップしてる
	m_menuInventory->BroughtItMenu(m_inventory->TakeItMenu());
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
		return new SceneGame();
	}

	return this;
}

void SceneMenu::Draw()
{
	// ノードの描画
	m_rootNode->TreeDraw();
}