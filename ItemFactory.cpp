#include"ItemFactory.h"
#include"Item.h"
#include"NavMesh.h"
#include"UiScore.h"
#include <cstdlib>
#include <ctime>

ItemFactory::ItemFactory(
	UiScore* uiScore,
	Inventory* inventory,
	NavMesh* navMesh
) :
	m_isFinish(false),
	m_getNum(0),
	m_uiScore(uiScore)
{
	srand(static_cast<unsigned int>(time(NULL)));
	// アイテムを生成する
	for (int i = 0; i < ItemNum; i++)
	{
		int itemNum = rand() % Item::GetItemTypeNum();
		AddChild(new Item(itemNum, navMesh->GetPos(), inventory));
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