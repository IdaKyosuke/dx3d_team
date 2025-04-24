#include "SceneMenu.h"
#include "SceneGame.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "MenuInventory.h"
#include "Chest.h"
#include "Shop.h"
#include "Wallet.h"
#include "SellButton.h"
#include "EnhanceType.h"
#include "EnhanceInventory.h"
#include "EnhanceTheWorldTime.h"
#include "MoneyCount.h"

#include"Inventory.h"

#include "SceneTitle.h"

void SceneMenu::Initialize()
{
	m_rootNode = new Node();

	// �A�N�^�[���C���[
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UI���C���[
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);

	//�����̎��
	m_maxHaveItem = m_inventory->GetMaxHaveItem();
	m_maxTheWorldTime = m_enhanceType->GetMaxTheWorldTime();
	m_enhanceType = new EnhanceType(m_maxHaveItem, m_maxTheWorldTime, 0);

	//�`�F�X�g
	m_chest = new Chest();
	uiLayer->AddChild(m_chest);
	//���j���[�̃C���x���g��
	m_menuInventory = new MenuInventory(m_chest,m_enhanceType);
	uiLayer->AddChild(m_menuInventory);
	
	//����{�^��
	m_sellButton = new SellButton();
	uiLayer->AddChild(m_sellButton);

	//���z
	m_wallet = new Wallet();
	uiLayer->AddChild(m_wallet);
	m_wallet->InWalletMoney(m_haveMoney);

	//�V���b�v
	m_shop = new Shop(m_chest,m_wallet,m_sellButton);
	uiLayer->AddChild(m_shop);
	
	//�����{�^��
	m_enhanceInventory = new EnhanceInventory(m_chest, m_wallet, m_enhanceType);
	uiLayer->AddChild(m_enhanceInventory);
	
	m_enhanceWorldTime = new EnhanceTheWorldTime(m_chest,m_wallet,m_enhanceType);
	uiLayer->AddChild(m_enhanceWorldTime);

	m_restDays = m_moneyCount->GetRestDays();
	m_clearCount = m_moneyCount->GetClearCount();

	m_moneyCount = new MoneyCount(m_wallet, m_restDays, m_clearCount);
	uiLayer->AddChild(m_moneyCount);
	m_moneyCount->BackMenu();

	if (!m_inventory->GetItemList().empty())
	{
		for (int i = 0; i <= m_inventory->GetItemList().size() - 1; i++)
		{
			//�����ċA�����A�C�e�����i�[����
			m_menuInventory->Change(std::next(m_inventory->GetItemList().begin(), i)->GetItemNum());
		}
	}

	if (!m_chestItem.empty())
	{
		for (int i = 0; i <= m_chestItem.size() - 1; i++)
		{
			//�`�F�X�g�ɕۊǂ��Ă����A�C�e��
			m_chest->SetItemList(std::next(m_chestItem.begin(), i)->GetItemNum());
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

	
	if (m_moneyCount->GetRestDays() <= 0)
	{
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
		{
			if (m_moneyCount->GetTaskClear())
			{
				m_wallet->LostMoney(m_moneyCount->GetNeedMoney());

				return new SceneGame(m_chest->GetItemList(),m_enhanceType, m_wallet->HaveMoney(), m_moneyCount);
			}
			else 
			{
				return new SceneTitle();
			}
		}
	}
	else
	{
		if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
		{
			m_haveMoney = m_wallet->HaveMoney();

			return new SceneGame(m_chest->GetItemList(), m_enhanceType, m_haveMoney, m_moneyCount);
		}
	}

	return this;
}

void SceneMenu::Draw()
{
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();
}