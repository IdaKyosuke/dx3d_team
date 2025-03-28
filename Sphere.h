#pragma once
#include"Actor.h"
#include"Vector3.h"

class LoadPlayer;
class ItemFactory;

class Sphere : public Actor
{
private:
	static constexpr float RotateSpeed = 0.01f;	// ���̉�]�X�s�[�h
	static constexpr float FroatHeight = 40.0f;	// ���̏㉺�ɗh��镝
	static constexpr float LerpTime = 2.0f;		// �A�C�e���������̂ɂ����鎞��
	static constexpr float DeleteDistance = 180.0f;	// �v���C���[���A�C�e�����l���ł��鋗��
	const char* PathName = "item.mv1";	// ���̃p�X

	LoadPlayer* m_loadPlayer;	// �v���C���[�̃|�C���^
	ItemFactory* m_itemFactory;

	int m_model;
	Vector3 m_pos;
	Vector3 m_rotate;
	float m_dist;
	float m_froatHeight;
	float m_elapsedTime;
	float m_startY;			// �h��n�߂̍���
	bool m_changeDir;		// �h�������𔽓]������

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	Sphere(LoadPlayer* loadPlayer, const Vector3& pos, ItemFactory* itemFactory);
	virtual ~Sphere();

	// ���𓮂���
	void MoveSphere();

	// �v���C���[�Ƃ̋����𔻒肷��
	void CheckDistance();
};