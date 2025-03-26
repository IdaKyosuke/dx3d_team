#pragma once
#include"Actor.h"
#include"Vector3.h"

class LoadPlayer;
class ItemFactory;

class Sphere : public Actor
{
private:
	static constexpr float RotateSpeed = 0.01f;	// 球の回転スピード
	static constexpr float FroatHeight = 40.0f;	// 球の上下に揺れる幅
	static constexpr float LerpTime = 2.0f;		// アイテムが動くのにかかる時間
	static constexpr float DeleteDistance = 180.0f;	// プレイヤーがアイテムを獲得できる距離
	const char* PathName = "item.mv1";	// 球のパス

	LoadPlayer* m_loadPlayer;	// プレイヤーのポインタ
	ItemFactory* m_itemFactory;

	int m_model;
	Vector3 m_pos;
	Vector3 m_rotate;
	float m_dist;
	float m_froatHeight;
	float m_elapsedTime;
	float m_startY;			// 揺れ始めの高さ
	bool m_changeDir;		// 揺れる向きを反転させる

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	Sphere(LoadPlayer* loadPlayer, const Vector3& pos, ItemFactory* itemFactory);
	virtual ~Sphere();

	// 球を動かす
	void MoveSphere();

	// プレイヤーとの距離を判定する
	void CheckDistance();
};