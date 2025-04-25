#include"Audio3D.h"
#include"Time.h"
#include"Enemy.h"
#include "DxLib.h"

Audio3D::Audio3D(const char* audioHundle, Enemy* enemy) :
	Actor3D("Audio"),
	m_durationTime(0),
	m_enemy(enemy),
	m_isSetCoolTime(true)
{
	// 3D������ݒ�
	SetCreate3DSoundFlag(true);
	m_sound = LoadSoundMem(audioHundle);
	SetCreate3DSoundFlag(false);

	// �T�E���h���͂�( ������������ )�͈͂�ݒ�
	Set3DRadiusSoundMem(SoundRange, m_sound);

	// ���̍Đ�����|�C���g��ݒ�
	Set3DPositionSoundMem(m_enemy->GetPosition(), m_sound);

	m_playCoolTime = static_cast<float>(rand() % MinPlayCoolTime + RangePlayCoolTime);
}

void Audio3D::Update()
{
	if (!m_isSetCoolTime)
	{
		// �Đ�����܂ł̎��Ԃ�ݒ�
		m_playCoolTime = static_cast<float>(rand() % MinPlayCoolTime + RangePlayCoolTime);
	}
	else
	{
		// ���Ԃ��v��
		m_durationTime += Time::GetInstance()->GetDeltaTime();

		if (m_durationTime >= m_playCoolTime)
		{
			// �Đ����Ԃ̃N�[���^�C�����I�������Đ�
			PlaySoundMem(m_sound, DX_PLAYTYPE_BACK);
			m_durationTime = 0;
			m_isSetCoolTime = false;
		}
	}

}