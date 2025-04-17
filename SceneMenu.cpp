#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "MenuInventory.h"
#include "Chest.h"
#include "Shop.h"
#include "ShopButton.h"
#include "Wallet.h"
#include "SellButton.h"
#include "EnhanceShopButton.h"
#include "EnhanceShop.h"

#include "EnhanceType.h"
#include "EnhanceInventory.h"

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

	//�`�F�X�g
	m_chest = new Chest();
	uiLayer->AddChild(m_chest);
	//���j���[�̃C���x���g��
	m_menuInventory = new MenuInventory(m_chest);
	uiLayer->AddChild(m_menuInventory);
	
	//����{�^��
	m_sellButton = new SellButton();
	uiLayer->AddChild(m_sellButton);

	//�V���b�v�؂�ւ��{�^��
	m_shopButton = new ShopButton();
	uiLayer->AddChild(m_shopButton);
	//�����V���b�v�؂�ւ��{�^��
	m_enhanceShopButton = new EnhanceShopButton();
	uiLayer->AddChild(m_enhanceShopButton);

	//���z
	m_wallet = new Wallet();
	uiLayer->AddChild(m_wallet);

	//�V���b�v
	m_shop = new Shop(m_chest,m_wallet,m_sellButton, m_shopButton);
	uiLayer->AddChild(m_shop);
	

	m_enhanceType = new EnhanceType(m_inventory,m_maxHaveItem);

	//�����X�g�A
	m_enhanceShop = new EnhanceShop(m_chest);
	uiLayer->AddChild(m_enhanceShop);
	
	//�����{�^��
	uiLayer->AddChild(new EnhanceInventory(m_chest,m_wallet,m_enhanceType));


	if (!m_inventory->TakeItMenu().empty())
	{
		for (int i = 0; i <= m_inventory->TakeItMenu().size() - 1; i++)
		{
			//�����ċA�����A�C�e�����i�[����
			m_menuInventory->Change(std::next(m_inventory->TakeItMenu().begin(), i)->GetItemNum());
		}
	}

	if (!m_chestItem.empty())
	{
		for (int i = 0; i <= m_chestItem.size() - 1; i++)
		{
			//�`�F�X�g�ɕۊǂ��Ă����A�C�e��
			m_chest->Change(std::next(m_chestItem.begin(), i)->GetItemNum());
			m_chest->CreateIcon(i);
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
		return new SceneGame(m_chest->GetItemList(), m_enhanceType->GetMaxHaveInventory());
	}

	return this;
}

void SceneMenu::Draw()
{
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();
}