#include "ActorCollision3D.h"
#include "Actor3D.h"
#include "Collider3D.h"

#include "BoxCollider3D.h"

// 更新
void ActorCollision3D::Update()
{
	// 途中でリストへの追加や削除が発生してもいいように、コピーで回す
	std::list<Actor3D*> actorList(m_actorList);

	// 総当たりで衝突判定
	for (auto it1 = actorList.begin(); it1 != actorList.end(); it1++)
	{
		for (auto it2 = it1; it2 != actorList.end(); it2++)
		{
			// イテレータからActor*を取り出す
			Actor3D* actor1 = *it1;
			Actor3D* actor2 = *it2;

			// 衝突判定の形状が無かったらスキップ
			if (!actor1->GetCollider()) continue;
			if (!actor2->GetCollider()) continue;

			// 自分自身はスキップ
			if (actor1 == actor2) continue;

			// 衝突判定
			if (actor1->GetCollider()->CheckCollision(
				actor1->GetTransform(),
				actor2->GetTransform(), actor2->GetCollider()))
			{
				actor1->OnCollision(actor2);
				actor2->OnCollision(actor1);
			}
		}
	}
}

#ifdef _DEBUG
void ActorCollision3D::Draw() const
{
	for (auto actor : m_actorList)
	{
		actor->GetCollider()->Draw(actor->GetTransform());
	}
}
#endif
