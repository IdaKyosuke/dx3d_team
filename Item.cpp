#include "Item.h"
#include "ModelLoader.h"
#include "LoadPlayer.h"
#include "Input.h"
#include "Inventory.h"
#include "DxLib.h"
#include <cmath>
#include "BoxCollider3D.h"

Item::Item(int itemNumber,Vector3 spownPos,LoadPlayer* player,Inventory* inventory) : Actor3D("Item", spownPos),
	m_itemNumber(itemNumber),
	m_itemName(),
	m_player(player),
	m_inventory(inventory),
	m_canGetItem(false),
	m_playerToDistance(0)
{
	//ポジションの設定
	m_itemPos = spownPos;

	//拾える範囲の設定
	m_collider = new BoxCollider3D(CanGetRange, Vector3(0,0,0));

	ItemNameList itemList = static_cast<ItemNameList>(m_itemNumber);
	//アイテムのモデルのロード
	m_model = MV1LoadModel(itemDate[static_cast<int>(itemList)].m_modelName);

	//ポジションの設定
	MV1SetPosition(m_model, m_itemPos);

	//名前の設定
	m_itemName = itemDate[static_cast<int>(itemList)].m_itemName;
	//売った時の金額設定
	m_sellMoney = itemDate[static_cast<int>(itemList)].m_sellMoney;
}

void Item::Release()
{
	Actor3D::Release();
	//モデルの削除
	MV1DeleteModel(m_model);
}

void Item::Update()
{
	float num = m_itemPos.x - m_player->GetPosition().x;
	float num2 = m_itemPos.y - m_player->GetPosition().y;
	float num3 = m_itemPos.z - m_player->GetPosition().z;
	//アイテムとプレイヤーの距離
	m_playerToDistance = std::sqrt(num * num + num2 * num2 + num3 * num3);
	
	m_canGetItem = m_inventory->CanGetItem();

	//距離が近くなってFを押したら獲得
	if (m_playerToDistance <= 50 && m_canGetItem)
	{
		if (Input::GetInstance()->IsKeyPress(KEY_INPUT_F))
			m_canGetItem = m_inventory->CanGetItem();

		Actor3D::Update();
	}
}

void Item::OnCollision(const Actor3D* other)
{
	//プレイヤーが拾える範囲に入ったら拾える
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

	Actor3D::Update();
}

void Item::Draw()
{
	// モデルの描画
	MV1DrawModel(m_model);
	Actor3D::Draw();
}