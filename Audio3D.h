#pragma once
#include"Actor3D.h"

class Enemy;

class Audio3D : Actor3D
{
private:
	static constexpr float SoundRange = 800.0f;		// ���̕�������͈�
	static constexpr int MinPlayCoolTime = 10;		// �����Đ�����܂ł̎��Ԃ̍ŏ��l
	static constexpr int RangePlayCoolTime = 10;		// �����Đ�����܂ł̎��Ԃ͈̔�

	int m_sound;	// �����n���h��
	Enemy* m_enemy;	// ���g�̐e�i���W�擾�p�j

	float m_durationTime;	// ���݂̌o�ߎ���
	float m_playCoolTime;	// �Đ��܂ł̎���
	bool m_isSetCoolTime;	// �������Đ�����܂ł̃N�[���^�C��

protected:
	virtual void Update() override;

public:
	Audio3D(const char* audioHundle, Enemy* enemy);

};