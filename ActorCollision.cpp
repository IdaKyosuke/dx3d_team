#include "ActorCollision.h"
#include "Actor.h"
#include "Collider.h"

#include "BoxCollider2D.h"

// 更新
void ActorCollision::Update()
{
	// 途中でリストへの追加や削除が発生してもいいように、コピーで回す
	std::list<Actor*> actorList(m_actorList);

	// 総当たりで衝突判定
	for (auto it1 = actorList.begin(); it1 != actorList.end(); it1++)
	{
		for (auto it2 = it1; it2 != actorList.end(); it2++)
		{
			// イテレータからActor*を取り出す
			Actor* actor1 = *it1;
			Actor* actor2 = *it2;

			// 衝突判定の形状が無かったらスキップ
			if (!actor1->GetCollider()) continue;
			if (!actor2->GetCollider()) continue;

			// 自分自身はスキップ
			if (actor1 == actor2) continue;

			// 衝突判定
			if (actor1->GetCollider()->CheckCollision(
				actor1->GetTransform(),
				actor2->GetTransform(), actor2->GetCollider() ))
			{
				actor1->OnCollision(actor2);
				actor2->OnCollision(actor1);
			}
			/*
			const BoxCollider2D* box = dynamic_cast<const BoxCollider2D*>(actor1->GetCollider());
			if (box != nullptr)
			{
				// Collider*は本当にBoxCollider*だった
			}
			else
			{
				// Collider*はBoxCollider*ではなかった
			}
			*/
		}
	}
}

#ifdef _DEBUG
void ActorCollision::Draw() const
{
	for (auto actor : m_actorList)
	{
		actor->GetCollider()->Draw(actor->GetTransform());
	}
}
#endif
