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

	// �A�N�^�[���C���[
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UI���C���[
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);

	// �^�C�g�����S
	m_rootNode->AddChild(new Actor(
		"Logo",
		"game_title.png",
		Screen::TopCenter + Vector2(0, Screen::Height / 3)
	));

	// Press Any Button
	m_rootNode->AddChild(new PressAny());

	/*
	// �X�e�[�W�̓����蔻����쐬
	m_collisionStage = new CollisionStage("Resource/stage.mv1", Vector3(0, 0, 0));
	uiLayer->AddChild(m_collisionStage);

	// �v���C���[
	m_loadPlayer = new LoadPlayer(m_collisionStage);
	actorLayer->AddChild(m_loadPlayer);

	//�C���x���g��
	m_inventory = new Inventory(m_loadPlayer);
	uiLayer->AddChild(m_inventory);
	*/
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

	if (Input::GetInstance()->IsAnyKeyDown())
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