#include"LoadPlayer.h"
#include"Time.h"
#include"Math.h"
#include"Camera.h"
#include"Animation3D.h"
#include"Input.h"
#include"Lerp.h"
#include"Quaternion.h"
#include"Collision3D.h"
#include<math.h>


// アニメーションリスト
const char* LoadPlayer::AnimList[RoopAnimNum] =
{
	"Man/Idle_stand.mv1", 
	"Man/Walking.mv1",
	"Man/Sprint.mv1",
	"Man/Jumping_trimed.mv1",
	"Man/jumpUp.mv1",
	"Man/fall.mv1",
	"Man/landing.mv1",
	"Man/Floating.mv1",
};

LoadPlayer::LoadPlayer(Collision3D* collision3D) :
	Actor("Player"),
	m_model(MV1LoadModel("Resource/Man/Man.mv1")),
	m_animIndex(0),
	m_moving(false),
	m_nowAnim(RoopAnim::Idle),
	m_nextAnim(RoopAnim::Idle),
	m_moveMode(MoveMode::Normal),
	m_angle(0),
	m_moveDirection(Vector3(0,0,0)),
	m_camNode(nullptr),
	m_collision3D(collision3D),
	m_elapsedTime(0),
	m_fallTime(0),
	m_isJump(false),
	m_isJumping(false),
	m_isFall(false),
	m_isFloating(false),
	m_isTerning(false)
{
	//-----アニメーションの作成-----
	// アニメーションクラスをリスト化する
	for (int i = 0; i < RoopAnimNum; i++)
	{
		m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
		AddChild(m_attachAnimList[i]);
	}

	// プレイヤーの位置(x, y, z)
	m_playerPos = SpawnPos;
	// アニメーション用の位置
	m_playerPastPos = m_playerPos;
	
	// プレイヤーの回転(x, y, z)
	m_playerRotate = Vector3(0, 0, 0);

	// カメラの生成
	m_camNode = new Camera(this);
	AddChild(m_camNode);

	// ディレクショナルライトを作成
	m_lightHandle = CreateDirLightHandle(Vector3(0, 100.0f, 0));

	// 最初のアニメーションを指定
	m_attachAnimList[static_cast<int>(RoopAnim::Idle)]->FadeIn();
}

// アニメーションを切り替える(Lerp)
void LoadPlayer::ChangeAnimLerp()
{
	if (m_nowAnim == m_nextAnim) return;

	m_attachAnimList[static_cast<int>(m_nowAnim)]->FadeOut();
	m_attachAnimList[static_cast<int>(m_nextAnim)]->FadeIn();
	
	m_nowAnim = m_nextAnim;
}

// アニメーションを切り替える(即座)
void LoadPlayer::ChangeAnimQuick(const RoopAnim nextAnim)
{
	m_attachAnimList[static_cast<int>(m_nowAnim)]->ChangeOut();
	m_attachAnimList[static_cast<int>(nextAnim)]->ChangeIn();

	m_nowAnim = nextAnim;
	m_nextAnim = nextAnim;
}

// アニメーションを再生する
void LoadPlayer::PlayAnim()
{
	// カメラの位置と向き
	m_camNode->SetCamPosAndTag();

	// モデルの回転
	if (!m_isTerning)
	{
		Quaternion::RotateAxisY(m_model, m_angle, m_playerPos);
	}
	else
	{
		Quaternion::RotateAxisYZ(m_model, m_angle, m_playerPos);
	}

	// モデルの描画
	MV1DrawModel(m_model);

	DrawFormatString(0, 40, GetColor(255, 255, 255),
		"PlayerPos Vector3(%.0f, %.0f, %.0f)",
		m_playerPos.x, m_playerPos.y, m_playerPos.z
	);
}

// モデル関係を削除
void LoadPlayer::Finalize()
{
	// ライトハンドルを削除
	DeleteLightHandle(m_lightHandle);
	// プレイヤーのモデルを削除
	MV1DeleteModel(m_model);
}

void LoadPlayer::Update()
{
	// 落下死, 天井到達 -> リスポーン or press "r" => リスタート
	if(m_playerPos.y <= -RestartHeight || 
		m_playerPos.y >= RestartHeight || 
		Input::GetInstance()->IsKeyDown(KEY_INPUT_R)
	)
	{
		m_playerPos = SpawnPos;
		m_isTerning = false;
	}

	// 移動状態の切り替え
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
	{
		ChangeMoveMode();
	}

	// プレイヤーの移動
	switch (m_moveMode)
	{
	case MoveMode::Normal:
		NormalMove();
		break;

	case MoveMode::ZeroGravity:
		ZeroGravityMove();
		break;
	}

	// アニメーションの切り替え
	ChangeAnimLerp();

	// 1フレーム前の位置を更新
	m_playerPastPos = m_playerPos;
}

// ジャンプ処理
void LoadPlayer::Jumping()
{
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();
	if (!m_isTerning)
	{
		m_playerPos.y += JumpPower - Gravity * m_elapsedTime;
	}
	else
	{
		m_playerPos.y += -JumpPower + Gravity * m_elapsedTime;
	}

	// 足元が床に触れたら
	if (m_isJumping && m_collision3D->GetHeight(m_playerPos, m_isTerning).HitFlag)
	{
		m_isJump = false;
		m_isFall = false;
		m_isJumping = false;
		m_nextAnim = RoopAnim::Landing;
	}
	else
	{
		m_isJumping = true;
	}
}

// プレイヤーの通常移動
void LoadPlayer::NormalMove()
{
	// ----プレイヤーの移動----
	float inputX = 0;
	float inputZ = 0;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_S)) inputZ = -1;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_W)) inputZ = 1;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_A)) inputX = -1;
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_D)) inputX = 1;

	// 重力の向きを確認
	float dir = m_isTerning ? -1.0f : 1.0f;
	// 移動方向を決定（重力に応じて左右移動の向きが変わる）
	m_moveDirection = m_camNode->CamFrontPlaneVec() * inputZ + m_camNode->CamRight() * inputX;

	// 重力を加算（ジャンプ中でない && 着地していないときのみ）
	if (!m_isJump && m_collision3D->GetHeight(m_playerPos, m_isTerning).HitFlag == 0 && !m_isJump)
	{
		m_fallTime += Time::GetInstance()->GetDeltaTime();
		if (!m_isTerning)
		{
			// 通常時
			m_playerPos.y -= Gravity * m_fallTime;
		}
		else
		{
			m_playerPos.y += Gravity * m_fallTime;
		}
	}
	else if (m_collision3D->GetHeight(m_playerPos, m_isTerning).HitFlag == 1)
	{
		// プレイヤーの高さを足場の高さに合わせる
		m_fallTime = 0;
		if (!m_isTerning)
		{
			// 通常時
			m_playerPos.y = m_collision3D->GetHeight(m_playerPos, m_isTerning).HitPosition.y;
		}
		else
		{
			// 重力反転時
			m_playerPos.y = m_collision3D->GetHeight(m_playerPos, m_isTerning).HitPosition.y;
		}
	}

	// 移動
	m_playerPos += Math::Normalized(m_moveDirection) * (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT) ? RunSpeed : WalkSpeed);

	if (m_collision3D->CapsuleCollider(m_playerPos, m_isTerning) != 0)
	{
		// 進む予定先に壁があった時
m_playerPos.x = m_playerPastPos.x;
m_playerPos.z = m_playerPastPos.z;
	}

	// ジャンプ
	if (!m_isJump && m_collision3D->GetHeight(m_playerPos, m_isTerning).HitFlag != 0 && Input::GetInstance()->IsKeyDown(KEY_INPUT_SPACE))
	{
		m_nextAnim = RoopAnim::JumpUp;
		m_isJump = true;
		m_elapsedTime = 0;
	}
	else if (m_isJump)
	{
		Jumping();
	}

	// プレイヤーの回転で現在の向きと回転予定の向きの符号が違うときに符号を合わせる（180 ～ -180でしか取れないため）
	if (!m_moveDirection.IsZero())
	{
		float afterAngle = atan2f(m_moveDirection.z, m_moveDirection.x);
		// 現在の角度の絶対値が符号を変えた後の数の絶対値より小さかったら何もしない(0 => 180)
		if (std::abs(m_angle) > Math::GetSign(Math::Pi / 2))
		{
			if (Math::GetSign(m_angle) < Math::GetSign(afterAngle))
			{
				// - => +
				m_angle += Math::DegtoRad(Math::Pi * 2);
			}
			else if (Math::GetSign(m_angle) > Math::GetSign(afterAngle))
			{
				// + => -
				m_angle -= Math::DegtoRad(Math::Pi * 2);
			}
		}
		m_angle = Lerp::Exec(m_angle, afterAngle, 0.2f);
	}

	// ---- 移動アニメーション ----
	if (m_collision3D->GetHeight(m_playerPos, m_isTerning).HitFlag == 0)
	{
		m_nextAnim = RoopAnim::JumpIdle;
	}
	else
	{
		if (!m_isJump)
		{
			if (m_playerPastPos.x != m_playerPos.x ||
				m_playerPastPos.z != m_playerPos.z)
			{
				if (!Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT))
				{
					// walk
					m_nextAnim = RoopAnim::Walk;
					m_moving = true;
				}
				else
				{
					// run
					m_nextAnim = RoopAnim::Run;
					m_moving = true;
				}
			}
			else
			{
				// idle
				m_nextAnim = RoopAnim::Idle;
				m_moving = false;
			}
		}
	}
}

// 無重力状態の移動
void LoadPlayer::ZeroGravityMove()
{
	if (!m_isFloating)
	{
		if (!m_isTerning) m_playerPos.y += FloatHeight;
		else m_playerPos.y -= FloatHeight;
		m_isFloating = true;
	}

	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_SPACE))
	{
		// 重力の反転
		m_isTerning = !m_isTerning;
		MV1SetRotationXYZ(m_model, Vector3(0, Math::DegtoRad(180), 0));
		m_moveMode = MoveMode::Normal;
		m_isFloating = false;
	}
}

// 移動モードの切り替え
void LoadPlayer::ChangeMoveMode()
{
	switch (m_moveMode)
	{
	case MoveMode::Normal:
		// 地面に足がついているときだけ移行できる
		if (m_collision3D->GetHeight(m_playerPos, m_isTerning).HitFlag != 0)
		{
			// 通常移動 -> 無重力移動
			m_nextAnim = RoopAnim::Floating;
			m_moveMode = MoveMode::ZeroGravity;
			// 重力をリセット
			m_fallTime = 0;
		}
		break;

	case MoveMode::ZeroGravity:
		// 無重力移動 -> 通常移動
		m_nextAnim = RoopAnim::Idle;
		m_moveMode = MoveMode::Normal;
		m_isFloating = false;
		// 重力をリセット
		m_fallTime = 0;
		break;
	}
}

void LoadPlayer::Draw()
{
	// アニメーション再生
	PlayAnim();
}

