#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "MenuInventory.h"
#include "Chest.h"
#include "Shop.h"
#include "ShopButton.h"

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

	//�`�F�X�g�ƃC���x���g��
	m_menuInventory = new MenuInventory(m_chest);
	m_chest = new Chest(m_menuInventory);
	uiLayer->AddChild(m_chest);

	m_menuInventory = new MenuInventory(m_chest);
	uiLayer->AddChild(m_menuInventory);

	m_shop = new Shop();
	uiLayer->AddChild(m_shop);

	//�V���b�v�؂�ւ��{�^��
	m_shopButton = new ShopButton();
	uiLayer->AddChild(m_shopButton);

	if (!m_inventory->TakeItMenu().empty())
	{
		for (int i = 0; i <= m_inventory->TakeItMenu().size() - 1; i++)
		{
			//�����ċA�����A�C�e�����i�[����
			m_menuInventory->Change(std::next(m_inventory->TakeItMenu().begin(), i)->GetItemNum());
		}
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