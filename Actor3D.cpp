#include "Actor3D.h"
#include "ActorCollision3D.h"
#include "Collider3D.h"

// コンストラクタ
Actor3D::Actor3D(const char* name, const Vector3& position, int itemNum) :
	m_name(name),
	m_itemNum(itemNum),
	m_collider(nullptr)
{
	m_transform.position = position;
}

// リソースの読み込み
void Actor3D::Load()
{
	// 衝突判定システムに登録
	if (m_collider)
	{
		ActorCollision3D::GetInstance()->Register(this);
	}
}

// リソースの解放
void Actor3D::Release()
{
	// 衝突判定システムから除外
	if (m_collider)
	{
		ActorCollision3D::GetInstance()->Remove(this);
		delete m_collider;
		m_collider = nullptr;
	}
}

// 描画
void Actor3D::Draw()
{
	Node::Draw();
}

// 子ノードを含む更新
void Actor3D::TreeUpdate()
{
	// Node本来のTreeUpdateを実行
	Node::TreeUpdate();
}
