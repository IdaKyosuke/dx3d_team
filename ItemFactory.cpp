#include"ItemFactory.h"
#include"Item.h"
#include"NavMesh.h"
#include"LoadPlayer.h"
#include <cstdlib>
#include <ctime>

ItemFactory::ItemFactory(
	Inventory* inventory,
	NavMesh* navMesh,
	LoadPlayer* player
) :
	m_isFinish(false),
	m_getNum(0)
{
	srand(static_cast<unsigned int>(time(NULL)));
	// ÉAÉCÉeÉÄÇê∂ê¨Ç∑ÇÈ
	for (int i = 0; i < ItemNum; i++)
	{
		int itemNum = rand() % Item::GetItemTypeNum();
		AddChild(new Item(itemNum, navMesh->GetPos(), inventory, player));
	}
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