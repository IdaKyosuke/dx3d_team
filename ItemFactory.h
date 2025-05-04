#pragma once
#include"Node.h"
#include"Vector3.h"

class NavMesh;
class Inventory;
class LoadPlayer;

class ItemFactory : public Node
{
private:
	static constexpr int ItemNum = 10;

	bool m_isFinish;		// �A�C�e�������ׂďW�ߏI������
	int m_getNum;		// �W�ߏI�����A�C�e����

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemFactory(
		Inventory* inventory,
		NavMesh* navMesh,
		LoadPlayer* player
	);

	// �N���A�t���O��Ԃ�
	bool IsFinish()
	{
		return m_isFinish;
	}
};