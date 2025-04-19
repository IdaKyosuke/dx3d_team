#pragma once
#include"Node.h"
#include"Vector3.h"

class UiScore;
class NavMesh;
class Inventory;
class LoadPlayer;

class ItemFactory : public Node
{
private:
	static constexpr int ItemNum = 6;

	bool m_isFinish;		// �A�C�e�������ׂďW�ߏI������
	int m_getNum;		// �W�ߏI�����A�C�e����
	UiScore* m_uiScore;	// �X�R�A�\���p

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemFactory(
		UiScore* uiScore,
		LoadPlayer* player,
		Inventory* inventory,
		NavMesh* navMesh
	);

	// �l�����ꂽ�A�C�e�������J�E���g�A�b�v
	void ItemCount();

	// �N���A�t���O��Ԃ�
	bool IsFinish()
	{
		return m_isFinish;
	}
};