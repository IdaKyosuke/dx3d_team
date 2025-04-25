#include"Audio3D.h"
#include"Time.h"
#include"Enemy.h"
#include "DxLib.h"

Audio3D::Audio3D(const char* audioHundle, Enemy* enemy) :
	Actor3D("Audio"),
	m_durationTime(0),
	m_enemy(enemy),
	m_isSetCoolTime(true),
	m_audioHundle(audioHundle)
{
	m_playCoolTime = static_cast<float>(rand() % MinPlayCoolTime + RangePlayCoolTime);
}

void Audio3D::Load()
{
	// 3D音源を設定
	SetCreate3DSoundFlag(true);
	m_sound = LoadSoundMem(m_audioHundle);
	SetCreate3DSoundFlag(false);

	// サウンドが届く( 音が聞こえる )範囲を設定
	Set3DRadiusSoundMem(SoundRange, m_sound);

	// 音の再生するポイントを設定
	Set3DPositionSoundMem(m_enemy->GetPosition(), m_sound);

}

void Audio3D::Update()
{
	// 再生位置を更新
	Set3DPositionSoundMem(m_enemy->GetPosition(), m_sound);

	if (!m_isSetCoolTime)
	{
		// 再生するまでの時間を設定
		m_playCoolTime = static_cast<float>(rand() % MinPlayCoolTime + RangePlayCoolTime);
		m_isSetCoolTime = true;
	}
	else
	{
		// 時間を計測
		m_durationTime += Time::GetInstance()->GetDeltaTime();

		if (m_durationTime >= m_playCoolTime)
		{
			// 再生時間のクールタイムが終わったら再生
			PlaySoundMem(m_sound, DX_PLAYTYPE_BACK);
			m_durationTime = 0;
			m_isSetCoolTime = false;
		}
	}
}

void Audio3D::Release()
{
	Actor3D::Release();

	DeleteSoundMem(m_sound);
}