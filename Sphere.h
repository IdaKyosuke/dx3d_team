#pragma once
#include"Actor3D.h"
#include"Vector3.h"

class LoadPlayer;
class ItemFactory;

class Sphere : public Actor3D
{
private:
	static constexpr float RotateSpeed = 0.01f;	// 球の回転スピード
	static constexpr float FroatHeight = 40.0f;	// 球の上下に揺れる幅
	static constexpr float LerpTime = 2.0f;		// アイテムが動くのにかかる時間
	const char* PathName = "item.mv1";	// 球のパス

	ItemFactory* m_itemFactory;

	int m_model;
	Vector3 m_rotate;
	float m_dist;
	float m_froatHeight;
	float m_elapsedTime;
	float m_startY;			// 揺れ始めの高さ
	bool m_changeDir;		// 揺れる向きを反転させる

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	Sphere(const Vector3& pos, ItemFactory* itemFactory);
	virtual ~Sphere();

	// 球を動かす
	void MoveSphere();
};