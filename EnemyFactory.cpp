#include"EnemyFactory.h"
#include"Enemy.h"
#include"NavMesh.h"

EnemyFactory::EnemyFactory(
	Node* m_actorNode,
	NavMesh* navMesh,
	LoadPlayer* loadPlayer, 
	const int enemyNum
)
{
	for (int i = 0; i < enemyNum; i++)
	{
		// �����_���ō��W�����߂Đ���
		Vector3 pos = navMesh->GetPos();
		m_actorNode->AddChild(new Enemy(navMesh, pos, loadPlayer));
	}
}