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

	bool m_isFinish;		// アイテムをすべて集め終えたか
	int m_getNum;		// 集め終えたアイテム数

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemFactory(
		Inventory* inventory,
		NavMesh* navMesh,
		LoadPlayer* player
	);

	// クリアフラグを返す
	bool IsFinish()
	{
		return m_isFinish;
	}
};