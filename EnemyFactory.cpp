#include"EnemyFactory.h"
#include"Enemy.h"
#include"NavMesh.h"

EnemyFactory::EnemyFactory(
	Node* m_actorNode,
	NavMesh* navMesh,
	LoadPlayer* loadPlayer
)
{
	for (int i = 0; i < EnemyNum; i++)
	{
		// ランダムで座標を決めて生成
		Vector3 pos = navMesh->GetPos();
		m_actorNode->AddChild(new Enemy(navMesh, pos, loadPlayer));
	}
}