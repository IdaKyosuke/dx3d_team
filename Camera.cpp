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
	// 3人称カメラ
	ThirdPersonCam(playerPos);

	// 1人称カメラ
	//FirstPersonCam(playerPos);

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

// プレイヤーを中心に回転する3人称視点カメラ
void Camera::ThirdPersonCam(const Vector3& playerPos)
{
	if (Input::GetInstance()->GetMousePoint().x != Screen::Center.x)
	{
		// マウスが左右に動いている（最後に割る数はちょうどいい数を探す必要あり）
		float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
		// マウスの移動量に応じてカメラの回転速度を決める
		m_camPos = Math::PointRotate(m_camPos, m_loadPlayerNode->PlayerPos(), (DX_PI_F / DivAngle) * diffX);
		// プレイヤーと一定距離を保つ（高さはプレイヤーの移動量を加算する）
		Vector3 diff = m_camTarget - CamFrontPlaneVec() * CamDiff;
		m_camPos = Vector3(diff.x, m_camPos.y + (playerPos.y - m_pastPlayerPos.y), diff.z);
	}
	else if (m_pastPlayerPos != playerPos)
	{
		// マウスが動いていないときはプレイヤーの動きについていく
		m_camPos += playerPos - m_pastPlayerPos;
	}
	// カメラの注視点
	m_camTarget = Vector3(playerPos.x, m_camPos.y, playerPos.z);
	// 1フレーム前のプレイヤーの位置を更新
	m_pastPlayerPos = playerPos;
}

// プレイヤーと一緒に移動する1人称カメラ
void Camera::FirstPersonCam(const Vector3& playerPos)
{
	// カメラの座標はプレイヤーの座標（高さは目線ぐらいに調整）
	m_camPos = Vector3(playerPos.x, playerPos.y + 200, playerPos.z);

	// 垂直回転
	float diffY = (Input::GetInstance()->GetMousePoint().y - Screen::Center.y) / DecMouseDiff;
	float vRot = Math::DegtoRad(5) * diffY;
	// 水平回転
	float diffX = (Input::GetInstance()->GetMousePoint().x - Screen::Center.x) / DecMouseDiff;
	float hRot = Math::DegtoRad(5) * diffX;

	// カメラの座標(m_camPos) 垂直回転(vRot) 水平回転(hRot) ひねり回転(tRot)
	SetCameraPositionAndAngle(m_camPos, diffY, diffX, 0);
}

// 反重力を実装する場合のカメラ
void Camera::AntiGravityCam(const Vector3& playerPos)
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