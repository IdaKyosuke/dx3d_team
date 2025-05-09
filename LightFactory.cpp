#include"LightFactory.h"
#include"LoadPlayer.h"
LightFactory::LightFactory(LoadPlayer* loadPlayer) :
	m_player(loadPlayer),
	m_lightHundle(0),
	m_hundlePos(Vector3(0, 0, 0))
{
	//m_lightHundle = CreatePointLightHandle(m_player->GetPosition(), 2500.0f, 0.0f, 0.002f, 0.0f);

	m_lightHundle = CreateSpotLightHandle(
		m_player->GetPosition(),
		m_player->GetPlayerFront(),
		80 * DX_PI_F / 180.0f,
		40 * DX_PI_F / 180.0f,
		3000.0f,
		0.0f,
		0.001f,
		0.0f);

	// ���ʂ��킩��Ղ��悤�ɕW�����C�g�𖳌���
	SetLightEnable(false);
}

void LightFactory::Update()
{
	Vector3 playerPos = m_player->GetPosition();
	// ���C�g�̈ʒu���v���C���[�ɍ��킹��
	SetLightPositionHandle(m_lightHundle, Vector3(playerPos.x, playerPos.y + 150, playerPos.z));

	// ���C�g�̌������v���C���[�ɍ��킹��
	SetLightDirectionHandle(m_lightHundle, m_player->GetPlayerFront());

	m_hundlePos = GetLightPositionHandle(m_lightHundle);
}

void LightFactory::Release()
{
	// ���C�g�n���h�����폜
	DeleteLightHandle(m_lightHundle);
}

void LightFactory::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"PlayerPos Vector3(%.0f, %.0f, %.0f)",
		m_hundlePos.x, m_hundlePos.y, m_hundlePos.z
	);
#endif // _DEBUG

}