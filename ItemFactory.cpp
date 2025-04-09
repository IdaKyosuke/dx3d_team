#include"ItemFactory.h"
#include"Sphere.h"
#include"UiScore.h"

ItemFactory::ItemFactory(UiScore* uiScore) :
	m_isFinish(false),
	m_getNum(0),
	m_uiScore(uiScore)
{
	// アイテムを生成する
	for (int i = 0; i < ItemNum; i++)
	{
		AddChild(new Sphere(pos[i], this));
	}

	// 表示用スコアの設定
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