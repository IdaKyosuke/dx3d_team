#include"LightFactory.h"
#include"LoadPlayer.h"
LightFactory::LightFactory(LoadPlayer* loadPlayer) :
	m_player(loadPlayer),
	m_lightHundle(0)
{
	m_lightHundle = CreatePointLightHandle(m_player->GetPosition(), 2500.0f, 0.0f, 0.002f, 0.0f);
	// ���ʂ��킩��Ղ��悤�ɕW�����C�g�𖳌���
	SetLightEnable(false);
}

void LightFactory::Update()
{
	// ���C�g�̈ʒu���v���C���[�ɍ��킹��
	SetLightPositionHandle(m_lightHundle, m_player->GetPosition());
}

void LightFactory::Release()
{
	// ���C�g�n���h�����폜
	DeleteLightHandle(m_lightHundle);
}