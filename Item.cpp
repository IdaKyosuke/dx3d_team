#include "Item.h"
#include "ModelLoader.h"
#include "LoadPlayer.h"
#include "Input.h"
#include "Inventory.h"
#include "BoxCollider3D.h"
#include "DxLib.h"
#include <cmath>

Item::Item(int itemNumber,Vector3 spownPos,LoadPlayer* player,Inventory* inventory) : Actor3D("Item", spownPos),
	m_itemNumber(itemNumber),
	m_itemName(),
	m_player(player),
	m_inventory(inventory),
	m_canGetItem(false),
	m_playerToDistance(0)
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
		if (Input::GetInstance()->IsKeyPress(KEY_INPUT_F) && m_canGetItem)
		{
			m_inventory->SetItemList(this);
			m_inventory->OnInventory();
			m_inventory->GettingItem();

			Destroy();
		}
	}
}