#include"ItemFactory.h"
#include"Sphere.h"
#include"UiScore.h"

ItemFactory::ItemFactory(UiScore* uiScore) :
	m_isFinish(false),
	m_getNum(0),
	m_uiScore(uiScore)
{
	// �A�C�e���𐶐�����
	for (int i = 0; i < ItemNum; i++)
	{
		AddChild(new Sphere(pos[i], this));
	}

	// �\���p�X�R�A�̐ݒ�
	m_uiScore->SetNum(ItemNum);
}

void ItemFactory::Update()
{
	if (m_getNum >= ItemNum)
	{
		m_isFinish = true;
	}
}

void ItemFactory::Draw()
{
	DrawFormatString(0, 100, GetColor(255, 255, 255), "HIT:%d", m_getNum);
}

void ItemFactory::ItemCount()
{
	m_getNum++;
	m_uiScore->GetItem(1);
}