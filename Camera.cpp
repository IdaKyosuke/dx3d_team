#include"Camera.h"
#include"Math.h"
#include"LoadPlayer.h"
#include"Input.h"
#include"Screen.h"

Camera::Camera(LoadPlayer* player) :
	Actor("PlayerCam"),
	m_camPos(StartPos),
	m_loadPlayerNode(player),
	m_pastTerning(false),
	m_diffY(DiffY),
	m_sightMode(SightMode::First)
{
	Vector3 playerPos = Vector3(m_loadPlayerNode->PlayerPos());
	switch(m_sightMode)
	{
	case SightMode::First:
		// 自身の正面が注視点になる
		m_camTarget = Vector3(m_camPos.x * 2, m_camPos.y, m_camPos.z * 2);
		break;

	case SightMode::Third:
		// プレイヤーが注視点になる
		m_camTarget = Vector3(playerPos.x, m_camTarget.y, playerPos.z);
		break;
	}
	// プレイヤーの移動量
	m_pastPlayerPos = player->PlayerPos();
};

// カメラの場所と焦点を設定
void Camera::SetCamPosAndTag()
{
	switch (m_sightMode)
	{
	case SightMode::First:
		SetCameraPositionAndTargetAndUpVec(m_camPos, m_camTarget, Vector3(0, 1, 0));
		break;

	case SightMode::Third:
		if (Input::GetInstance()->IsKeyPress(KEY_INPUT_UP))
		{
			// カメラの移動（↑）
			float y = m_camTarget.y + DiffTagY;
			SetCameraPositionAndTargetAndUpVec(m_camPos, Vector3(m_camTarget.x, y, m_camTarget.z), Vector3(0, 1, 0));
		}
		else if (Input::GetInstance()->IsKeyPress(KEY_INPUT_DOWN))
		{
			// カメラの移動（↓）
			float y = m_camTarget.y - DiffTagY;
			SetCameraPositionAndTargetAndUpVec(m_camPos, Vector3(m_camTarget.x, y, m_camTarget.z), Vector3(0, 1, 0));
		}
		else
		{
			// 通常カメラ
			SetCameraPositionAndTargetAndUpVec(m_camPos, m_camTarget, Vector3(0, 1, 0));
		}
		break;
	}
	
}

// カメラの正面ベクトルを取得する(XYZ)
Vector3 Camera::CamFrontVec()
{
	Vector3 rot = Math::Normalized((m_camTarget - m_camPos));
	return rot;
}

// カメラの正面ベクトルを取得(XZ平面)
Vector3 Camera::CamFrontPlaneVec()
{
	Vector3 camRight = CamRight();
	Vector3 camUp = Vector3(0, -1.0f, 0);
	return Math::Normalized(camUp.CrossP(camRight));
}

// カメラの右方向ベクトルを取得
Vector3 Camera::CamRight()
{
	// カメラの正面を取得
	Vector3 camFront = CamFrontVec();
	// カメラの上ベクトルを取得
	Vector3 camUp = Vector3(0, -1.0f, 0);
	// 2つの外積を求める
	Vector3 camRight = camFront.CrossP(camUp);
	// 正規化して返す
	return Math::Normalized(camRight);
}

// プレイヤーを追いかける
void Camera::ChasePlayer(const Vector3& playerPos)
{
	switch (m_sightMode)
	{
	case SightMode::First:
		// 1人称視点
		FirstPerson(playerPos);
		break;

	case SightMode::Third:
		// 3人称視点
		ThirdPerson(playerPos);
		break;
	}
	
	// カメラの高さは常にプレイヤーの移動量と同じにする
	if (m_pastPlayerPos != playerPos)
	{
		m_camPos.y = playerPos.y + DiffY;
	}
	m_pastPlayerPos = playerPos;

	// カメラのスクリーン座標を中心に戻す
	Input::GetInstance()->SetMousePoint();
}


// 3人称視点
void Camera::ThirdPerson(const Vector3& playerPos)
{
	// 3人称視点
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// マウスを動かすと
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
		m_camPos = Math::PointRotate(m_camPos, m_loadPlayerNode->PlayerPos(), DX_PI_F / CamRot * diffX);
		// プレイヤーと一定距離を保つ
		Vector3 pos = Vector3(m_camTarget - CamFrontPlaneVec() * CamDiff);
		m_camPos = Vector3(pos.x, playerPos.y + DiffY, pos.z);
	}
	else if (m_pastPlayerPos != playerPos)
	{
		// マウスの移動量がないときはプレイヤーと同じ動きをする
		m_camPos += playerPos - m_pastPlayerPos;
	}
	m_camTarget = Vector3(playerPos.x, m_camPos.y, playerPos.z);
}

// 1人称視点
void Camera::FirstPerson(const Vector3& playerPos)
{
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// マウスを動かすとカメラの注視点が動く
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
		m_camTarget = Math::PointRotate(m_camTarget, playerPos, DX_PI_F / CamRot * diffX);
		// カメラと注視点を一定距離に保つ
		Vector3 pos = Vector3(playerPos + CamFrontPlaneVec() * CamDiff);
		m_camTarget = Vector3(pos.x, playerPos.y + DiffY, pos.z);
	}
	else
	{
		// マウスの移動量がないときはプレイヤーと同じ動きをする
		m_camTarget += playerPos - m_pastPlayerPos;
	}
	// カメラをプレイヤーと一緒に移動する
	m_camPos = Vector3(playerPos.x, playerPos.y + DiffY, playerPos.z);
}

// 反重力用カメラ
void Camera::AntiGravity(const Vector3& playerPos)
{
	// 重力の向きが変わったら角度を変えていく
	if (m_loadPlayerNode->IsTerning() != m_pastTerning)
	{
		// いったん角度をリセット
		m_diffY = 0;
	}

	if (m_diffY < DiffY)
	{
		m_diffY += AddAngleY;
		if (m_diffY >= DiffY)
		{
			// 値に上限を設ける
			m_diffY = DiffY;
		}
	}

	// 重力の向きによって見上げるか見下ろすかを変える
	if (!m_loadPlayerNode->IsTerning())
	{
		m_camPos.y = playerPos.y + m_diffY;
	}
	else
	{
		m_camPos.y = playerPos.y - m_diffY;
	}

	// 1フレーム前の重力の状態を保存
	m_pastTerning = m_loadPlayerNode->IsTerning();
}

void Camera::Update()
{	
	// プレイヤーの方を向く
	ChasePlayer(m_loadPlayerNode->PlayerPos());
}

void Camera::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, GetColor(255, 255, 255), 
		"CamTarget Vector3(%.0f, %.0f, %.0f)",
		m_camTarget.x, m_camTarget.y, m_camTarget.z);

	DrawFormatString(0, 20, GetColor(255, 255, 255),
		"CamPos Vector3(%.0f, %.0f, %.0f)",
		m_camPos.x, m_camPos.y, m_camPos.z);

	DrawFormatString(0, 120, GetColor(255, 255, 255),
		"CamAngle :%.0f",
		Math::ACos(CamFrontVec(), CamFrontPlaneVec()));

	DrawFormatString(0, 140, GetColor(255, 255, 255),
		"CamFrontVec Vector3(%.0f, %.0f, %.0f)",
		CamFrontVec().x, CamFrontVec().y, CamFrontVec().z);
#endif // _DEBUG

}