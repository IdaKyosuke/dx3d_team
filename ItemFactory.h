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

	bool m_isFinish;		// アイテムをすべて集め終えたか
	int m_getNum;		// 集め終えたアイテム数
	UiScore* m_uiScore;	// スコア表示用

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

	// 獲得されたアイテム数をカウントアップ
	void ItemCount();

	// クリアフラグを返す
	bool IsFinish()
	{
		return m_isFinish;
	}
};