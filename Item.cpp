#include "Item.h"
#include "ModelLoader.h"
#include "LoadPlayer.h"
#include "Input.h"
#include "Inventory.h"
#include "BoxCollider3D.h"
#include "ActorCollision3D.h"
#include "DxLib.h"
#include <cmath>

Item::Item(int itemNumber, Vector3 spownPos, Inventory* inventory, LoadPlayer* player) :
	Actor3D("Item", spownPos, itemNumber),
	m_itemNumber(itemNumber),
	m_itemName(),
	m_inventory(inventory),
	m_canGetItem(false),
	m_playerToDistance(0),
	m_player(player),
	m_iconName(),
	m_itemData()
{
	//�|�W�V�����̐ݒ�
	m_itemPos = spownPos;

	//�E����͈͂̐ݒ�
	m_collider = new BoxCollider3D(CanGetRange, Vector3(0,0,0));

	//�A�C�e������csv����̃��[�h
	LoadItemData();

	//�A�C�e���̃��f���̃��[�h
	m_model = MV1LoadModel(m_itemData[m_itemNumber].m_modelName);

	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_model, m_itemPos);

	//���O�̐ݒ�
	m_itemName = m_itemData[m_itemNumber].m_itemName;
	//���������̋��z�ݒ�
	m_sellMoney = m_itemData[m_itemNumber].m_sellMoney;
	//�A�C�e���̏d���ݒ�
	m_itemWeight = m_itemData[m_itemNumber].m_weight;
	//�A�C�R���̃e�N�X�`��ID
	m_iconName = m_itemData[m_itemNumber].m_iconName;
	//�t���[�o�[�e�L�X�g�̐ݒ�
	m_flavorText = m_itemData[m_itemNumber].m_flavorText;
}

void Item::Release()
{
	Actor3D::Release();
	//���f���̍폜
	MV1DeleteModel(m_model);
}

void Item::Update()
{
	m_canGetItem = m_inventory->CanGetItem();

	Actor3D::Update();
}

void Item::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);
	Actor3D::Draw();
}

void Item::OnCollision(const Actor3D* other)
{
	//�v���C���[���E����͈͂ɓ�������E����
	if (other->GetName() == "Player")
	{
		if (m_player->GetIsGetting())
		{
			m_inventory->AddAdvanceItemList(this);
		}
	}
}

void Item::DestroyMine()
{
	ActorCollision3D::GetInstance()->Remove(this);
	Destroy();
}

void Item::LoadItemData()
{
	int fileHandle = FileRead_open("Item.csv");

	char dontUseLine[9][256];

	for (int i = 0; i < 1; i++) FileRead_gets(dontUseLine[0], 256, fileHandle);

	for (int i = 0; i < static_cast<int>(ItemNameList::Length); i++) 
	{
		FileRead_scanf(fileHandle, "%[^,],%[^,],%[^,],%[^,],%d,%d",
			m_itemData[i].m_itemName, m_itemData[i].m_modelName, m_itemData[i].m_iconName,
			m_itemData[i].m_flavorText, &m_itemData[i].m_sellMoney, &m_itemData[i].m_weight);//�ǂݍ���
	}

	//�t�@�C�������
	FileRead_close(fileHandle);
}