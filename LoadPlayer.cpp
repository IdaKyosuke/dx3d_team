#include"LoadPlayer.h"
#include"Time.h"
#include"Math.h"
#include"Camera.h"
#include"Animation3D.h"
#include"Input.h"
#include"Lerp.h"
#include"Quaternion.h"
#include"CollisionStage.h"
#include"BoxCollider3D.h"
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

LoadPlayer::LoadPlayer(CollisionStage* collisionStage) :
	Actor3D("Player", SpawnPos),
	m_model(MV1LoadModel("Resource/Man/Man.mv1")),
	m_animIndex(0),
	m_moving(false),
	m_nowAnim(RoopAnim::Idle),
	m_nextAnim(RoopAnim::Idle),
	m_moveMode(MoveMode::Normal),
	m_moveDirection(Vector3(0, 0, 0)),
	m_camNode(nullptr),
	m_collisionStage(collisionStage),
	m_elapsedTime(0),
	m_fallTime(0),
	m_isJump(false),
	m_isJumping(false),
	m_isFall(false),
	m_isFloating(false),
	m_isTerning(false),
	m_hit(false)
{
	//-----アニメーションの作成-----
	// アニメーションクラスをリスト化する
	for (int i = 0; i < RoopAnimNum; i++)
	{
		m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
		AddChild(m_attachAnimList[i]);
	}

	// アニメーション用の位置
	m_playerPastPos = m_transform.position;

	// プレイヤーの回転(x, y, z)
	m_playerRotate = Vector3(0, 0, 0);
	m_transform.angle = Vector3(0, 180, 0);

	// カメラの生成
	m_camNode = new Camera(this);
	AddChild(m_camNode);

	// ディレクショナルライトを作成
	m_lightHandle = CreateDirLightHandle(Vector3(0, 100.0f, 0));

	// 最初のアニメーションを指定
	m_attachAnimList[static_cast<int>(RoopAnim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(ColSize, ColOffset);
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
		Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);
	}
	else
	{
		Quaternion::RotateAxisYZ(m_model, m_transform.angle.y, m_transform.position);
	}

	// モデルの描画
	MV1DrawModel(m_model);
	
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"PlayerPos Vector3(%.0f, %.0f, %.0f)",
		m_transform.position.x, m_transform.position.y, m_transform.position.z
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
	if (m_transform.position.y <= -RestartHeight ||
		m_transform.position.y >= RestartHeight ||
		Input::GetInstance()->IsKeyDown(KEY_INPUT_R)
		)
	{
		m_transform.position = SpawnPos;
		m_isTerning = false;
	}
	/*

	// 移動状態の切り替え
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_Q))
	{
		ChangeMoveMode();
	}*/

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
	m_playerPastPos = m_transform.position;
}

// ジャンプ処理
void LoadPlayer::Jumping()
{
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();
	if (!m_isTerning)
	{
		m_transform.position.y += JumpPower - Gravity * m_elapsedTime;
	}
	else
	{
		m_transform.position.y += -JumpPower + Gravity * m_elapsedTime;
	}

	// 足元が床に触れたら
	if (m_isJumping && m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag)
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
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag == 0 && !m_isJump)
	{
		m_fallTime += Time::GetInstance()->GetDeltaTime();
		if (!m_isTerning)
		{
			// 通常時
			m_transform.position.y -= Gravity * m_fallTime;
		}
		else
		{
			m_transform.position.y += Gravity * m_fallTime;
		}
	}
	else if (m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag == 1)
	{
		// プレイヤーの高さを足場の高さに合わせる
		m_fallTime = 0;
		if (!m_isTerning)
		{
			// 通常時
			m_transform.position.y = m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitPosition.y;
		}
		else
		{
			// 重力反転時
			m_transform.position.y = m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitPosition.y;
		}
	}

	// 移動
	m_transform.position += Math::Normalized(m_moveDirection) * (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT) ? RunSpeed : WalkSpeed);

	if (m_collisionStage->CapsuleCollider(m_transform.position, m_isTerning) != 0)
	{
		// 進む予定先に壁があった時
		m_transform.position.x = m_playerPastPos.x;
		m_transform.position.z = m_playerPastPos.z;
	}

	// ジャンプ
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag != 0 && Input::GetInstance()->IsKeyDown(KEY_INPUT_SPACE))
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
		float afterAngle = 0;

		Math::MatchAngleSign(afterAngle, m_moveDirection, m_transform.angle);

		m_transform.angle.y = Lerp::Exec(m_transform.angle.y, afterAngle, 0.2f);
	}

	// ---- 移動アニメーション ----
	if (m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag == 0)
	{
		m_nextAnim = RoopAnim::JumpIdle;
	}
	else
	{
		if (!m_isJump)
		{
			if (m_playerPastPos.x != m_transform.position.x ||
				m_playerPastPos.z != m_transform.position.z)
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
		if (!m_isTerning) m_transform.position.y += FloatHeight;
		else m_transform.position.y -= FloatHeight;
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
		if (m_collisionStage->GetHeight(m_transform.position, m_isTerning).HitFlag != 0)
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

void LoadPlayer::OnCollision(const Actor3D* other)
{
	if (other->GetName() == "wall")
	{
		m_hit = true;
	}
}

