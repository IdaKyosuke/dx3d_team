#include"Camera.h"
#include"Math.h"
#include"LoadPlayer.h"
#include"Input.h"
#include"Screen.h"

Camera::Camera(LoadPlayer* player) :
	Actor("PlayerCam"),
	m_camPos(StartPos),
	m_camTarget(player->PlayerPos()),
	m_loadPlayerNode(player),
	m_pastTerning(false),
	m_diffY(DiffY)
{
	Vector3 playerPos = Vector3(m_loadPlayerNode->PlayerPos());
	m_camTarget = Vector3(playerPos.x, m_camTarget.y, playerPos.z);
	// プレイヤーの移動量
	m_pastPlayerPos = player->PlayerPos();
};

// カメラの場所と焦点を設定
void Camera::SetCamPosAndTag()
{
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
	// 3人称視点
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// マウスを動かすと
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / 10;
		m_camPos = Math::PointRotate(m_camPos, m_loadPlayerNode->PlayerPos(), DX_PI_F / CamRot * diffX);
		// プレイヤーと一定距離を保つ
		m_camPos = m_camTarget - CamFrontPlaneVec() * CamDiff;
	}
	else if (m_pastPlayerPos != playerPos)
	{
		// マウスの移動量がないときはプレイヤーと同じ動きをする
		m_camPos += playerPos - m_pastPlayerPos;
	}

	// 1人称視点
	

	m_camTarget = Vector3(playerPos.x, m_camPos.y, playerPos.z);
	// カメラの高さは常にプレイヤーの移動量と同じにする
	if (m_pastPlayerPos != playerPos)
	{
		
	}
	m_camPos.y = m_camTarget.y - CamFrontPlaneVec().y * CamDiff;
	m_pastPlayerPos = playerPos;

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

	// カメラのスクリーン座標を中心に戻す
	Input::GetInstance()->SetMousePoint();
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
#endif // _DEBUG

}