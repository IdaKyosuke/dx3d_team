#include "Item.h"
#include "ModelLoader.h"
#include "LoadPlayer.h"
#include "Input.h"
#include "Inventory.h"
#include "DxLib.h"

#include <cmath>

Item::Item(int itemNumber,Vector3 spownPos,LoadPlayer* player,Inventory* inventory) : Actor("Item"),
	m_itemNumber(itemNumber),
	m_itemName(),
	m_player(player),
	m_inventory(inventory),
	m_canGetItem(false),
	m_playerToDistance(0)
{
	//�|�W�V�����̐ݒ�
	m_itemPos = spownPos;

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
	//���f���̍폜
	MV1DeleteModel(m_model);
}

void Item::Update()
{
	float num = m_itemPos.x - m_player->PlayerPos().x;
	float num2 = m_itemPos.y - m_player->PlayerPos().y;
	float num3 = m_itemPos.z - m_player->PlayerPos().z;
	//�A�C�e���ƃv���C���[�̋���
	m_playerToDistance = std::sqrt(num * num + num2 * num2 + num3 * num3);
	
	m_canGetItem = m_inventory->CanGetItem();

	//�������߂��Ȃ���F����������l��
	if (m_playerToDistance <= 50 && m_canGetItem)
	{
		if (Input::GetInstance()->IsKeyPress(KEY_INPUT_F))
		{
			m_inventory->SetItemList(this);
			m_inventory->OnInventory();
			m_inventory->GettingItem();

			Destroy();
		}
	}

	Actor::Update();
}

void Item::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);
	Actor::Draw();
}