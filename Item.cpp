#include "Item.h"
#include "ModelLoader.h"
#include "LoadPlayer.h"
#include "Input.h"
#include "DxLib.h"

#include <cmath>

Item::Item(int itemNumber,Vector3 spownPos,LoadPlayer* player) : Actor("Item"),
	m_itemNumber(itemNumber),
	m_player(player),
	m_playerToDistance(0)
{
	//�|�W�V�����̐ݒ�
	m_itemPos = spownPos;

	ItemNameList itemList = static_cast<ItemNameList>(m_itemNumber);
	//�A�C�e���̃��f���̃��[�h
	m_model = MV1LoadModel(itemDate[static_cast<int>(itemList)].m_modelName);

	//���������̋��z�ݒ�
	m_sellMoney = itemDate[static_cast<int>(itemList)].m_sellMoney;
}

void Item::Load()
{
	//ModelLoader::LoadModel(itemDate[m_itemNumber].m_modelName);
}

void Item::Release()
{
	//���f���̍폜
	MV1DeleteModel(m_model);
}

void Item::Update()
{
	MV1SetPosition(m_model,m_itemPos);
	 
	float num = m_itemPos.x - m_player->PlayerPos().x;
	float num2 = m_itemPos.y - m_player->PlayerPos().y;
	float num3 = m_itemPos.z - m_player->PlayerPos().z;

	m_playerToDistance = std::sqrt(num * num + num2 * num2 + num3 * num3);;
	
	if (m_playerToDistance <= 5)
	{
		if (Input::GetInstance()->IsKeyPress(KEY_INPUT_F))
		{
			Destroy();
		}
	}

	Actor::Update();
}

void Item::Draw()
{
	// ���f���̕`��
	MV1DrawModel(m_model);

	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"ItemPos Vector3(%.0f, %.0f, %.0f)",
		m_itemPos.x, m_itemPos.y, m_itemPos.z
	);

	DrawFormatString(0, 150, GetColor(255, 255, 255),
		"playerToDistance(% .0f)",
		m_playerToDistance
	);

	Actor::Draw();
}