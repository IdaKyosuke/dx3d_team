#pragma once
#include"Actor3D.h"
#include"Vector3.h"

class LoadPlayer;
class ItemFactory;

class Sphere : public Actor3D
{
private:
	static constexpr float RotateSpeed = 0.01f;	// ���̉�]�X�s�[�h
	static constexpr float FroatHeight = 40.0f;	// ���̏㉺�ɗh��镝
	static constexpr float LerpTime = 2.0f;		// �A�C�e���������̂ɂ����鎞��
	const char* PathName = "item.mv1";	// ���̃p�X

	ItemFactory* m_itemFactory;

	int m_model;
	Vector3 m_rotate;
	float m_dist;
	float m_froatHeight;
	float m_elapsedTime;
	float m_startY;			// �h��n�߂̍���
	bool m_changeDir;		// �h�������𔽓]������

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	Sphere(const Vector3& pos, ItemFactory* itemFactory);
	virtual ~Sphere();

	// ���𓮂���
	void MoveSphere();
};