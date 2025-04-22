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
#include"UiScore.h"
#include"UiResult.h"
#include"Inventory.h"
#include"CollisionStage.h"
#include"NavMesh.h"
#include"DrawStageView.h"
#include"EscapePoint.h"
#include"ScreenFilter.h"
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
	/*
	// �X�e�[�W�̌����ڂ�`��
	m_drawStageView = new DrawStageView("nav_stage_test_view.mv1");
	uiLayer->AddChild(m_drawStageView);
	*/
	

	// �X�e�[�W�̓����蔻����쐬
	m_collisionStage = new CollisionStage("Resource/nav_test_stage.mv1", Vector3(0, 0, 0));
	uiLayer->AddChild(m_collisionStage);

	// navMesh
	m_navMesh = new NavMesh(m_collisionStage);

	// �v���C���[
	m_loadPlayer = new LoadPlayer(m_collisionStage);
	actorLayer->AddChild(m_loadPlayer);

	//�C���x���g��
	m_inventory = new Inventory(m_loadPlayer);
	uiLayer->AddChild(m_inventory);
	m_inventory->SetMaxHaveItem(m_maxHaveInventory);
	/*
	//�A�C�e��
	m_item = new Item(0, Vector3(100, 50, 100),m_inventory);
	actorLayer->AddChild(m_item);
	//�A�C�e��
	m_item = new Item(0, Vector3(500, 50, 100),m_inventory);
	actorLayer->AddChild(m_item);
	//�A�C�e��
	m_item = new Item(0, Vector3(500, 50, 100), m_inventory);
	actorLayer->AddChild(m_item);
	//�A�C�e��
	m_item = new Item(0, Vector3(500, 50, 100), m_inventory);
	actorLayer->AddChild(m_item);

	//�A�C�e��
	m_item = new Item(1, Vector3(400, 50, 100),m_inventory);
	actorLayer->AddChild(m_item);

	//�A�C�e��
	m_item = new Item(1, Vector3(400, 50, 300), m_inventory);
	actorLayer->AddChild(m_item);

	//�A�C�e��
	m_item = new Item(2, Vector3(100, 50, 400),m_inventory);
	actorLayer->AddChild(m_item);
	*/

	// �X�R�A
	m_uiScore = new UiScore();
	uiLayer->AddChild(m_uiScore);

	// �A�C�e���̐���
	m_itemfactory = new ItemFactory(m_uiScore, m_inventory, m_navMesh);
	actorLayer->AddChild(m_itemfactory);
	
	// �G�̐���
	m_enemyFactory = new EnemyFactory(actorLayer, m_navMesh, m_loadPlayer);
	
	// �E�o�n�_�̍쐬
	m_escapePoint = new EscapePoint();
	actorLayer->AddChild(m_escapePoint);

	// ���U���g���
	m_uiResult = new UiResult(m_itemfactory);
	uiLayer->AddChild(m_uiResult);

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
	actorLayer->AddChild(m_screenFilter);
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

	
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_T))
	{
		return new SceneTitle();
	}
	

	if (m_escapePoint->IsEscape())
	{
		return new SceneMenu(m_keepChest->TakeItMenu(), m_inventory,m_inventory->GetMaxHaveItem(),m_haveMoney);
	}

	return this;
}

// �`��
void SceneGame::Draw()
{
	// �m�[�h�̕`��
	m_rootNode->TreeDraw();
}
