#pragma once
#include"Node.h"
#include"Vector3.h"

class UiScore;

class ItemFactory : public Node
{
private:
	static constexpr int ItemNum = 8;

	// �A�C�e�������ꏊ�̃��X�g
	const Vector3 pos[ItemNum] =
	{
		Vector3(-660, 440, 640),
		Vector3(-12, 120, 2700),
		Vector3(-600, 120, -920),
		Vector3(30, 420, 0),
		Vector3(-920, 980, -690),
		Vector3(500, 1580, -680),
		Vector3(880, 720, 570),
		Vector3(850, 720, -670),
	};

	bool m_isFinish;		// �A�C�e�������ׂďW�ߏI������
	int m_getNum;		// �W�ߏI�����A�C�e����
	UiScore* m_uiScore;	// �X�R�A�\���p

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemFactory(UiScore* uiScore);

	// �l�����ꂽ�A�C�e�������J�E���g�A�b�v
	void ItemCount();


	// �N���A�t���O��Ԃ�
	bool IsFinish()
	{
		return m_isFinish;
	}
};