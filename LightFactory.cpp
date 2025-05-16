#include"LightFactory.h"
#include"LoadPlayer.h"
#include"Camera.h"
LightFactory::LightFactory(Camera* camera, LoadPlayer* player) :
	m_cam(camera),
	m_player(player),
	m_lightHundle(0),
	m_hundlePos(Vector3(0, 0, 0))
{
	m_lightHundle = CreateSpotLightHandle(
		m_cam->GetPosition(),
		m_cam->CamFrontVec(),
		80 * DX_PI_F / 180.0f,
		40 * DX_PI_F / 180.0f,
		3000.0f,
		0.0f,
		0.001f,
		0.0f);

	// 効果がわかり易いように標準ライトを無効化
	SetLightEnable(false);
}

void LightFactory::Update()
{
	if (!m_player->IsFinsh())
	{
		// ライトの位置をカメラに合わせる
		SetLightPositionHandle(m_lightHundle, m_cam->GetPosition());

		// ライトの向きをカメラに合わせる
		SetLightDirectionHandle(m_lightHundle, m_cam->CamFrontVec());
#ifdef _DEBUG
		m_hundlePos = GetLightPositionHandle(m_lightHundle);
#endif // _DEBUG
	}
}

void LightFactory::Release()
{
	// ライトハンドルを削除
	DeleteLightHandle(m_lightHundle);
}

void LightFactory::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"LightPos Vector3(%.0f, %.0f, %.0f)",
		m_hundlePos.x, m_hundlePos.y, m_hundlePos.z
	);
#endif // _DEBUG

}