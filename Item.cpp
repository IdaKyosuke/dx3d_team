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
	m_player(player)
{
	//�|�W�V�����̐ݒ�
	m_itemPos = spownPos;

	//�E����͈͂̐ݒ�
	m_collider = new BoxCollider3D(CanGetRange, Vector3(0,0,0));

	ItemNameList itemList = static_cast<ItemNameList>(m_itemNumber);

	//�A�C�e���̃��f���̃��[�h
	m_model = MV1LoadModel(itemDate[static_cast<int>(itemList)].m_modelName);

	//�|�W�V�����̐ݒ�
	MV1SetPosition(m_model, m_itemPos);

	//���O�̐ݒ�
	m_itemName = itemDate[static_cast<int>(itemList)].m_itemName;
	//���������̋��z�ݒ�
	m_sellMoney = itemDate[static_cast<int>(itemList)].m_sellMoney;
	//�A�C�e���̏d���ݒ�
	m_itemWeight = itemDate[static_cast<int>(itemList)].m_weight;
	//�A�C�R���̃e�N�X�`��ID
	m_iconName = IconName[static_cast<int>(itemList)];
	//�t���[�o�[�e�L�X�g�̐ݒ�
	m_flavorText = itemDate[static_cast<int>(itemList)].m_flavorText;
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