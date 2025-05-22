#pragma once
#include"Node.h"
#include"Vector3.h"
#include"Item.h"
#include<vector>

class NavMesh;
class Inventory;
class LoadPlayer;

class ItemFactory : public Node
{
private:
	static constexpr int ItemNum = 40;
	static constexpr int ProbabilityEnhanceItem = 6;	// 強化アイテムが選ばれる割合
	static constexpr int ProbabilitySellItem = 4;		// 売却用アイテムが選ばれる割合

	bool m_isFinish;		// アイテムをすべて集め終えたか
	int m_getNum;		// 集め終えたアイテム数
	int m_itemType;	// アイテムの種類(1:強化用, 2:売却用)
	// アイテムのスポーン確率を変える用
	std::vector<Item::ItemNameList> m_forSellItemList;	// 売却用アイテム
	std::vector<Item::ItemNameList> m_forEnhanceItemList;	// 強化用アイテム

	ItemData m_itemData[static_cast<int>(Item::ItemNameList::Length)];

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemFactory(
		Inventory* inventory,
		NavMesh* navMesh
	);

	// クリアフラグを返す
	bool IsFinish()
	{
		return m_isFinish;
	}

	void LoadItemData();

	ItemData* GetItemData(int itemNum)
	{
		return &m_itemData[itemNum];
	}
};