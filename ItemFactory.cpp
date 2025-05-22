#include"ItemFactory.h"
#include"Item.h"
#include"NavMesh.h"
#include"LoadPlayer.h"
#include <cstdlib>
#include <ctime>

ItemFactory::ItemFactory(
	Inventory* inventory,
	NavMesh* navMesh
) :
	m_isFinish(false),
	m_getNum(0),
	m_itemType(0)
{
	// アイテムの情報を読み込む
	LoadItemData();

	srand(static_cast<unsigned int>(time(NULL)));
	// アイテムを生成する
	for (int i = 0; i < ItemNum; i++)
	{
		// 強化素材か売却用アイテムかを抽選
		int itemType = rand() % (ProbabilityEnhanceItem + ProbabilitySellItem);

		if (itemType <= ProbabilityEnhanceItem)
		{
			// 強化素材
			int itemNum = rand() % m_forEnhanceItemList.size();			
			int number = static_cast<int>(m_forEnhanceItemList[itemNum]);
			AddChild(new Item(number, &m_itemData[number], navMesh->GetPos(), inventory));
		}
		else
		{
			// 売却用
			int itemNum = rand() % m_forSellItemList.size();
			int number = static_cast<int>(m_forSellItemList[itemNum]);
			AddChild(new Item(number, &m_itemData[number], navMesh->GetPos(), inventory));
		}
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
#ifdef _DEBUG
	DrawFormatString(0, 100, GetColor(255, 255, 255), "HIT:%d", m_getNum);

#endif // _DEBUG

}

void ItemFactory::LoadItemData()
{
	int fileHandle = FileRead_open("Item.csv");

	char dontUseLine[9][256];

	for (int i = 0; i < 1; i++) FileRead_gets(dontUseLine[0], 256, fileHandle);

	for (int i = 0; i < static_cast<int>(Item::ItemNameList::Length); i++)
	{
		FileRead_scanf(fileHandle, "%[^,],%[^,],%[^,],%[^,],%d,%d,%d",
			m_itemData[i].m_itemName, m_itemData[i].m_modelName, m_itemData[i].m_iconName,
			m_itemData[i].m_flavorText, &m_itemData[i].m_sellMoney, &m_itemData[i].m_weight, &m_itemType);//読み込み

		switch (m_itemType)
		{
		case 1:
			// 強化用アイテムリストに追加
			m_forEnhanceItemList.push_back(static_cast<Item::ItemNameList>(i));
			break;

		case 2:
			// 売却用アイテムリストに追加
			m_forSellItemList.push_back(static_cast<Item::ItemNameList>(i));
			break;
		}
	}

	//ファイルを閉じる
	FileRead_close(fileHandle);
}