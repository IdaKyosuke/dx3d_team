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

	// Press Any Button
	m_rootNode->AddChild(new PressAny());

	//�`�F�X�g�ƃC���x���g��
	m_menuInventory = new MenuInventory(m_chest);
	m_chest = new Chest(m_menuInventory);
	uiLayer->AddChild(m_chest);

	m_menuInventory = new MenuInventory(m_chest);
	uiLayer->AddChild(m_menuInventory);

	auto getItemCount = std::distance(m_inventory->TakeItMenu().begin(), m_inventory->TakeItMenu().end());

	if (getItemCount >= 0)
	{
	//�����ċA�����A�C�e�����X���b�v���Ă�
	m_menuInventory->BroughtItMenu(m_inventory->TakeItMenu());
	}
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