#pragma once
#include"Actor3D.h"

class NavMesh;
class Node;
class LoadPlayer;

class EnemyFactory
{
private:
	static constexpr int EnemyNum = 15;

public:
	EnemyFactory(
		Node* m_actorNode,
		NavMesh* navMesh,
		LoadPlayer* loadPlayer);

};