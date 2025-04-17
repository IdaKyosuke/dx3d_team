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
const char* LoadPlayer::AnimList[AnimNum] =
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
	m_nowAnim(Anim::Idle),
	m_nextAnim(Anim::Idle),
	m_moveDirection(Vector3(0, 0, 0)),
	m_camNode(nullptr),
	m_collisionStage(collisionStage),
	m_elapsedTime(0),
	m_fallTime(0),
	m_isJump(false),
	m_isJumping(false),
	m_isFloating(false),
	m_duration(0),
	m_isFall(false),
	m_fallStartY(0),
	m_hit(false)
{
	//-----アニメーションの作成-----
	// アニメーションクラスをリスト化する
	for (int i = 0; i < AnimNum; i++)
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
	m_attachAnimList[static_cast<int>(Anim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	// 体力の初期値を設定
	m_hp = 100;
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
void LoadPlayer::ChangeAnimQuick(const Anim nextAnim)
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
	Quaternion::RotateAxisY(m_model, m_transform.angle.y, m_transform.position);

	// モデルの描画
	MV1DrawModel(m_model);
	
#ifdef _DEBUG
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"PlayerPos Vector3(%.0f, %.0f, %.0f)",
		m_transform.position.x, m_transform.position.y, m_transform.position.z
	);

	DrawFormatString(0, 150, GetColor(255, 255, 255), "PlayerHP %.0f", m_hp);
#endif // _DEBUG
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
	// 死 -> リスポーン or press "r" => リスタート
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R) || m_hp <= 0)
	{
		m_transform.position = SpawnPos;
		m_hp = 100;
	}

	// 無敵時間
	if (m_hit)
	{
		InvincibleTime();
	}

	// プレイヤーの移動
	NormalMove();

	// アニメーションの切り替え
	ChangeAnimLerp();

	// 1フレーム前の位置を更新
	m_playerPastPos = m_transform.position;
}

// ジャンプ処理
void LoadPlayer::Jumping()
{
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();

	m_transform.position.y += JumpPower - Gravity * m_elapsedTime;

	// 足元が床に触れたら
	if (m_isJumping && m_collisionStage->GetHeight(m_transform.position).HitFlag)
	{
		m_isJump = false;
		m_isJumping = false;
		m_nextAnim = Anim::Landing;
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

	// 移動方向を決定（重力に応じて左右移動の向きが変わる）
	m_moveDirection = m_camNode->CamFrontPlaneVec() * inputZ + m_camNode->CamRight() * inputX;

	// 重力を加算（ジャンプ中でない && 着地していないときのみ）
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position).HitFlag == 0)
	{
		m_fallTime += Time::GetInstance()->GetDeltaTime();
		// 通常時
		m_transform.position.y -= Gravity * m_fallTime;

		if (!m_isFall)
		{
			// 落下開始時の高さを保持
			m_fallStartY = m_transform.position.y;
			m_isFall = true;
		}
	}
	else if (m_collisionStage->GetHeight(m_transform.position).HitFlag == 1)
	{
		// プレイヤーの高さを足場の高さに合わせる
		m_fallTime = 0;
		// 通常時
		m_transform.position.y = m_collisionStage->GetHeight(m_transform.position).HitPosition.y;

		if (m_isFall)
		{
			CountFallHeight();
			m_isFall = false;
		}
	}

	// 移動
	m_transform.position += Math::Normalized(m_moveDirection) * (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT) ? RunSpeed : WalkSpeed);
	
	// 進む予定先に足場があるか
	if (!m_collisionStage->CheckStage(this->GetPosition()))
	{
		// ないとき
		m_transform.position.x = m_playerPastPos.x;
		m_transform.position.z = m_playerPastPos.z;
	}

	// ジャンプ
	if (!m_isJump && m_collisionStage->GetHeight(m_transform.position).HitFlag != 0 && Input::GetInstance()->IsKeyDown(KEY_INPUT_SPACE))
	{
		m_nextAnim = Anim::JumpUp;
		m_isJump = true;
		m_elapsedTime = 0;
		// ジャンプのスタート地点を記録
		m_fallStartY = m_transform.position.y;
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
	if (m_collisionStage->GetHeight(m_transform.position).HitFlag == 0)
	{
		m_nextAnim = Anim::JumpIdle;
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
					m_nextAnim = Anim::Walk;
					m_moving = true;
				}
				else
				{
					// run
					m_nextAnim = Anim::Run;
					m_moving = true;
				}
			}
			else
			{
				// idle
				m_nextAnim = Anim::Idle;
				m_moving = false;
			}
		}
	}
}

void LoadPlayer::Draw()
{
	// アニメーション再生
	PlayAnim();
}

void LoadPlayer::OnCollision(const Actor3D* other)
{
	if (!m_hit && other->GetName() == "Enemy")
	{
		m_hit = true;
		DecreaseHP(10);
	}
}

// 無敵時間
void LoadPlayer::InvincibleTime()
{
	m_duration += Time::GetInstance()->GetDeltaTime();

	if (m_duration >= m_time)
	{
		// 無敵時間の終了
		m_duration = 0;
		m_hit = false;
	}
}

// 落下した高さを計算する
void LoadPlayer::CountFallHeight()
{
	if ((abs(m_transform.position.y) - abs(m_fallStartY)) >= 1300.0f)
	{
		// 志望する高さから落下したとき
		m_hp = 0;
	}
	m_fallStartY = 0;
}

