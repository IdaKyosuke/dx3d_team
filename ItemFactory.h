#pragma once
#include"Node.h"
#include"Vector3.h"

class UiScore;

class ItemFactory : public Node
{
private:
	static constexpr int ItemNum = 8;

	// アイテム生成場所のリスト
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

	bool m_isFinish;		// アイテムをすべて集め終えたか
	int m_getNum;		// 集め終えたアイテム数
	UiScore* m_uiScore;	// スコア表示用

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	ItemFactory(UiScore* uiScore);

	// 獲得されたアイテム数をカウントアップ
	void ItemCount();


	// クリアフラグを返す
	bool IsFinish()
	{
		return m_isFinish;
	}
};