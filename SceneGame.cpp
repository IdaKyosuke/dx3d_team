#include "SceneGame.h"
#include"SceneTitle.h"
#include "SceneMenu.h"
#include "Screen.h"
#include "Input.h"
#include "Node.h"
#include "Actor.h"
#include "HitBox.h"
#include "Time.h"
#include"Fade.h"
#include"ImageLoader.h"
#include"LoadPlayer.h"
#include"Collision3D.h"
#include"ItemFactory.h"
#include"EnemyFactory.h"
#include"UiResult.h"
#include"UiTime.h"
#include "UiStamina.h"
#include"Inventory.h"
#include"CollisionStage.h"
#include"NavMesh.h"
#include"DrawStageView.h"
#include"EscapePoint.h"
#include"ScreenFilter.h"
#include "MoneyCount.h"
#include "EnhanceType.h"
#include"LightFactory.h"
#include "DxLib.h"

#include "Chest.h"
#include "KeepChest.h"

#include "Item.h"


// ������
void SceneGame::Initialize()
{
	// ���[�g
	m_rootNode = new Node();

	// �A�N�^�[���C���[
	Node* actorLayer = new Node();
	m_rootNode->AddChild(actorLayer);

	// UI���C���[
	Node* uiLayer = new Node();
	m_rootNode->AddChild(uiLayer);
	
	// �X�e�[�W�̌����ڂ�`��
	m_drawStageView = new DrawStageView("favorite_stage.mv1");
	uiLayer->AddChild(m_drawStageView);

	// �X�e�[�W�̓����蔻����쐬
	m_collisionStage = new CollisionStage("Resource/nav_test_stage.mv1", Vector3(0, 0, 0));
	uiLayer->AddChild(m_collisionStage);

	// navMesh
	m_navMesh = new NavMesh(m_collisionStage);

	//�����̎�ށ{�����������e�̕ێ�
	m_maxHaveInventory = m_enhanceType->GetMaxHaveInventory();
	m_theWorldTime = m_enhanceType->GetMaxTheWorldTime();
	m_useCountTheWorld = m_enhanceType->GetMaxUseTheWorldCount();
	m_staminaRecovery = m_enhanceType->GetAddStaminaRecovery();
	m_staminaDecrease = m_enhanceType->GetAlleviationStaminaDecrease();
	m_enhanceType = new EnhanceType(m_maxHaveInventory,m_theWorldTime, m_useCountTheWorld,m_staminaRecovery,m_staminaDecrease);

	//�C���x���g��
	m_inventory = new Inventory(m_maxHaveInventory);
	uiLayer->AddChild(m_inventory);


	// �v���C���[�̃X�|�[���n�_���쐬
	int index = rand() % PointNum;
	m_escapePointIndex = index;

	// �v���C���[
	m_loadPlayer = new LoadPlayer(m_collisionStage,m_inventory, m_enhanceType, pos[index]);
	actorLayer->AddChild(m_loadPlayer);

	// ���C�g���쐬
	m_lightFactory = new LightFactory(m_loadPlayer);
	actorLayer->AddChild(m_lightFactory);
#ifdef _DEBUG
	//�A�C�e��
	m_item = new Item(1, Vector3(300, 0, 0),m_inventory,m_loadPlayer);
	actorLayer->AddChild(m_item);//�A�C�e��
	m_item = new Item(0, Vector3(300, 0, 0),m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);

	m_item = new Item(3, Vector3(300, 0, 0), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);
	m_item = new Item(4, Vector3(300, 0, 200), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);
	m_item = new Item(0, Vector3(300, 0, 500), m_inventory, m_loadPlayer);
	actorLayer->AddChild(m_item);
#endif // _DEBUG

	// ��������
	m_uiTime = new UiTime();
	uiLayer->AddChild(m_uiTime);

	//�X�^�~�i
	m_uiStamina = new UiStamina(m_loadPlayer);
	uiLayer->AddChild(m_uiStamina);

	// �A�C�e���̐���
	m_itemfactory = new ItemFactory(m_inventory, m_navMesh, m_loadPlayer);
	actorLayer->AddChild(m_itemfactory);
	
	// �G�̐���
	m_enemyFactory = new EnemyFactory(actorLayer, m_navMesh, m_loadPlayer);
	

	// �E�o�n�_���쐬
	do
	{
		index = rand() % PointNum;
	} while (m_escapePointIndex == index);

	// �E�o�n�_�̍쐬
	m_escapePoint = new EscapePoint(pos[index], Rotate[index]);
	actorLayer->AddChild(m_escapePoint);

	// ���U���g���
	m_uiResult = new UiResult(m_itemfactory);
	uiLayer->AddChild(m_uiResult);

	m_restDays = m_moneyCount->GetRestDays();
	m_moneyCount = new MoneyCount(m_wallet, m_restDays, 0);
	m_keepChest = new KeepChest;
	
	//�`�F�X�g�ƃC���x���g��
	if (!m_chestItem.empty())
	{

		int haveItemCount = static_cast<int>(m_chestItem.size());

		for (int i = 0; i <= haveItemCount - 1; i++)
		{
			m_keepChest->SetItemList(std::next(m_chestItem.begin(), i)->GetItemNum());
		}
	}

	// �X�N���[���Ɋ|����t�B���^�[
	m_screenFilter = new ScreenFilter(m_loadPlayer);
	uiLayer->AddChild(m_screenFilter);
}

// �I��
void SceneGame::Finalize()
{
	// �v���C���[���̍폜
	m_loadPlayer->Finalize();

	// �|���S���̘A������j��
	m_navMesh->RemovePolyLinkInfo();

	// �m�[�h�̍폜
	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

// �X�V
SceneBase* SceneGame::Update()
{
	// �m�[�h�̍X�V
	m_rootNode->TreeUpdate();

	if (m_uiTime->IsFinsh() || m_loadPlayer->GetIsDeath())
	{
		return new SceneMenu(m_keepChest->TakeItMenu(), m_inventory, m_enhanceType, m_haveMoney, m_moneyCount);
	}
	
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_T))
	{
		return new SceneTitle();
	}
	

	if (m_escapePoint->IsEscape())
	{
		return new SceneMenu(m_keepChest->TakeItMenu(), m_inventory,m_enhanceType,m_haveMoney,m_moneyCount);
	}
#ifdef _DEBUG
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_M))
	{
		return new SceneMenu(m_keepChest->TakeItMenu(), m_inventory, m_enhanceType, m_haveMoney, m_moneyCount);
	}
#endif // _DEBUG


	return this;
}

// �`��
void SceneGame::Draw()
{
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();
}
