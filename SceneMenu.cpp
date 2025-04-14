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

	// �A�N�^�[���C���[
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UI���C���[
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);
	/*
	// �^�C�g�����S
	m_rootNode->AddChild(new Actor(
		"Logo",
		"game_title.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));
	*/

	//�`�F�X�g�ƃC���x���g��
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

			//�����ċA�����A�C�e�����X���b�v����
			m_menuInventory->BroughtItMenu(getItemNum,i);
		}
	}

	m_collisionStage = new CollisionStage("Resource/stage.mv1", Vector3(0, 0, 0));
	m_loadPlayer = new LoadPlayer(m_collisionStage);
	//m_inventory = new Inventory(m_loadPlayer);

}

void SceneMenu::Finalize()
{
	// �m�[�h�̍폜
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneMenu::Update()
{
	// �m�[�h�̍X�V
	m_rootNode->TreeUpdate();

	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
	{
		return new SceneGame();
	}

	return this;
}

void SceneMenu::Draw()
{
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();
}