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
#include "Inventory.h"
#include "EnhanceType.h"
#include"ItemFactory.h"
#include<math.h>

// アニメーションリスト
const char* LoadPlayer::AnimList[AnimNum] =
{
	"Man/Idle_stand.mv1",
	"Man/Walking.mv1",
	"Man/Sprint.mv1",
	"Man/Die.mv1",
};

LoadPlayer::LoadPlayer(
	CollisionStage* collisionStage,
	Inventory* inventory,
	EnhanceType* enhanceType,
	const Vector3& pos,
	ItemFactory* itemFactory
) :
	Actor3D("Player", pos),
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
	m_stopTime(0),
	m_useTheWorldCount(0),
	m_isStop(false),
	m_theWorldCoolDown(0),
	m_nowStopTime(0),
	m_isGetting(false),
	m_inventory(inventory),
	m_isDeath(false),
	m_isDash(false),
	m_stamina(MaxStamina),
	m_enhanceType(enhanceType),
	m_staminaRecovery(0),
	m_staminaDecrease(0),
	m_maxHaveWeight(0),
	m_runSpeed(RunSpeed),
	m_weightOver(false),
	m_finish(false),
	m_isCooldown(false),
	m_itemFactory(itemFactory)
{
	//-----アニメーションの作成-----
	// アニメーションクラスをリスト化する
	for (int i = 0; i < AnimNum; i++)
	{
		if (i < AnimNum - 1)
		{
			m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i]));
			AddChild(m_attachAnimList[i]);
		}
		else
		{
			// ループしないアニメーション
			m_attachAnimList.push_back(new Animation3D(m_model, AnimList[i], false));
			AddChild(m_attachAnimList[i]);
		}
	}

	// アニメーション用の位置
	m_playerPastPos = m_transform.position;

	// プレイヤーの回転(x, y, z)
	m_playerRotate = Vector3(0, 0, 0);
	m_transform.angle = Vector3(0, 180, 0);

	// カメラの生成
	m_camNode = new Camera(this, collisionStage);
	AddChild(m_camNode);

	// 最初のアニメーションを指定
	m_attachAnimList[static_cast<int>(Anim::Idle)]->FadeIn();

	m_collider = new BoxCollider3D(ColSize, ColOffset);

	//持てる最大容量の設定
	m_maxHaveWeight = m_enhanceType->GetMaxHaveWeight();

	// 体力の初期値を設定
	m_hp = MaxHp;

	// 音を聞くポイントを更新
	Set3DSoundListenerPosAndFrontPos_UpVecY(this->GetPosition(), m_camNode->CamFrontPlaneVec());

	m_stopTime = m_enhanceType->GetMaxTheWorldTime();
}

void LoadPlayer::Load()
{
	// ダメージボイスを設定
	m_seDamage = LoadSoundMem("Resource/sound/damage.mp3");
	ChangeVolumeSoundMem(128, m_seDamage);

	// 走る音
	m_seRun = LoadSoundMem("Resource/sound/player_run.mp3");

	// 歩く音
	m_seWalk = LoadSoundMem("Resource/sound/player_walk.mp3");

	Actor3D::Load();
}

void LoadPlayer::Release()
{
	// seを削除
	DeleteSoundMem(m_seDamage);
	DeleteSoundMem(m_seRun);
	DeleteSoundMem(m_seWalk);
	// プレイヤーのモデルを削除
	MV1DeleteModel(m_model);

	Actor3D::Release();
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
	DrawFormatString(0, 160, GetColor(255, 255, 255),
		"PlayerPos Vector3(%.0f, %.0f, %.0f)",
		m_transform.position.x, m_transform.position.y, m_transform.position.z
	);

	DrawFormatString(0, 150, GetColor(255, 255, 255), "PlayerHP %d", m_hp);
#endif // _DEBUG
}

void LoadPlayer::Update()
{
#ifdef _DEBUG
	// press "r" => リスタート
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_R))
	{
		m_transform.position = SpawnPos;
		m_hp = MaxHp;
	}

	// press "0" => 自殺
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_0))
	{
		m_hp = 0;
	}
#endif // _DEBUG

	if (m_hp <= 0)
	{
		if(!m_finish) m_finish = true;

		if (m_nowAnim != Anim::Death)
		{
			// 死亡アニメーションを再生
			ChangeAnimQuick(Anim::Death);
			m_camNode->ModeChange();
		}

		if (m_attachAnimList[static_cast<int>(m_nowAnim)]->FinishAnim())
		{
			// 死亡アニメーションが終了したら死亡フラグを立てる
			m_isDeath = true;
		}
	}
	else
	{
		if (m_maxHaveWeight * 1.5 > m_inventory->GetHaveWeight())
		{
			// プレイヤーの移動
			NormalMove();
		}

		DropItem();
		TheWorld();
	}

	if (m_maxHaveWeight < m_inventory->GetHaveWeight())
	{
		m_weightOver = true;
	}
	else
	{
		m_weightOver = false;
	}

	// アニメーションの切り替え
	ChangeAnimLerp();

	// 1フレーム前の位置を更新
	m_playerPastPos = m_transform.position;

	// 音を聞くポイントを更新
	Set3DSoundListenerPosAndFrontPos_UpVecY(this->GetPosition(), m_camNode->CamFrontPlaneVec());
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

		CountFallHeight();
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

	// 実際の移動先を決める
	CheckMove();

	// スタミナ管理
	StaminaManagement();

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
	if (m_collisionStage->GetHeight(m_transform.position).HitFlag != 0)
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

// 移動先を決める
void LoadPlayer::CheckMove()
{
	// 移動
	if (m_stamina > 0)
	{

		// ダッシュフラグの管理
		m_isDash = Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT);
		
		if (m_moveDirection.IsZero())
		{
			m_isDash = false;
		}

		

		//重さオーバーしたら移動スピードを下げる。
		if (m_weightOver)
		{
			m_runSpeed = WeightOverSpeed;
		}
		else
		{
			m_runSpeed = RunSpeed;

			if (m_maxHaveWeight * 0.9f <= m_inventory->GetHaveWeight())
			{
				m_runSpeed = RunSpeed * 0.7f;
			}
			if (m_maxHaveWeight * 0.7f <= m_inventory->GetHaveWeight())
			{
				m_runSpeed = RunSpeed * 0.8f;
			}
			if (m_maxHaveWeight * 0.5f <= m_inventory->GetHaveWeight())
			{
				m_runSpeed = RunSpeed * 0.9f;
			}
		}

		Vector3 nextPos = m_transform.position + m_moveDirection * (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT) ? m_runSpeed : WalkSpeed);

		// 球が壁に当たっていたら
		if (m_collisionStage->CapsuleCollider(nextPos) != 0)
		{
			// 移動できる方向を調整
			Vector3 dir = Vector3(0, 0, 0);

			// 壁の方向だけ進めないようにする
			for (int i = 0; i < m_collisionStage->CapsuleCollider(nextPos); i++)
			{
				// 当たったポリゴン分だけ回す
				Vector3 hitPos = m_collisionStage->GetColSphere().Dim[i].HitPosition;
				Vector3 wallDir = Math::Normalized(this->GetPosition() - hitPos);

				if (std::abs(dir.x) < std::abs(wallDir.x))
				{
					dir.x = wallDir.x;
				}
				if (std::abs(dir.z) < std::abs(wallDir.z))
				{
					dir.z = wallDir.z;
				}
			}

			m_moveDirection.x = dir.x - m_moveDirection.x;
			m_moveDirection.z = dir.z - m_moveDirection.z;
		}

		m_transform.position += m_moveDirection * (Input::GetInstance()->IsKeyPress(KEY_INPUT_LSHIFT) ? m_runSpeed : WalkSpeed);

		if (!m_moveDirection.IsZero())
		{
			if (m_isDash)
			{
				if (CheckSoundMem(m_seRun) == 0)
				{
					// 移動速度に応じてseを変える
					if (CheckSoundMem(m_seWalk) == 1)
					{
						StopSoundMem(m_seWalk);
					}
					PlaySoundMem(m_seRun, DX_PLAYTYPE_BACK);
				}
			}
			else
			{
				if (CheckSoundMem(m_seWalk) == 0)
				{
					// 移動速度に応じてseを変える
					if (CheckSoundMem(m_seRun) == 1)
					{
						StopSoundMem(m_seRun);
					}
					PlaySoundMem(m_seWalk, DX_PLAYTYPE_BACK);
				}
			}
		}
		else
		{
			// 移動終了時にSEを止める
			if (CheckSoundMem(m_seRun) == 1)
			{
				StopSoundMem(m_seRun);
			}
			if (CheckSoundMem(m_seWalk) == 1)
			{
				StopSoundMem(m_seWalk);
			}
		}
	}
	else
	{
		// スタミナがなくなった時点で走る用SEを止める
		if (CheckSoundMem(m_seRun) == 1)
		{
			StopSoundMem(m_seRun);
		}

		Vector3 nextPos = m_transform.position + m_moveDirection * WalkSpeed;
		m_isDash = false;

		// 進む予定先に壁があった時
		// 球が壁に当たっていたら
		if (m_collisionStage->CapsuleCollider(nextPos) != 0)
		{
			// 移動できる方向を調整
			Vector3 dir = Vector3(0, 0, 0);

			// 壁の方向だけ進めないようにする
			for (int i = 0; i < m_collisionStage->CapsuleCollider(nextPos); i++)
			{
				// 当たったポリゴン分だけ回す
				Vector3 hitPos = m_collisionStage->GetColSphere().Dim[i].HitPosition;
				Vector3 wallDir = Math::Normalized(this->GetPosition() - hitPos);

				if (std::abs(dir.x) < std::abs(wallDir.x))
				{
					dir.x = wallDir.x;
				}
				if (std::abs(dir.z) < std::abs(wallDir.z))
				{
					dir.z = wallDir.z;
				}
			}

			m_moveDirection.x = dir.x - m_moveDirection.x;
			m_moveDirection.z = dir.z - m_moveDirection.z;
		}

		m_transform.position += m_moveDirection * WalkSpeed;

		if (!m_moveDirection.IsZero())
		{
			if (CheckSoundMem(m_seWalk) == 0)
			{
				// 移動速度に応じてseを変える
				PlaySoundMem(m_seWalk, DX_PLAYTYPE_BACK);
			}
		}
		else
		{
			if (CheckSoundMem(m_seWalk) == 1)
			{
				// 移動速度に応じてseを変える
				StopSoundMem(m_seWalk);
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
	//プレイヤーが拾える範囲に入ったら拾える
	if (other->GetName() == "Item")
	{
		if (!m_isGetting)
		{
			if (Input::GetInstance()->IsKeyPress(KEY_INPUT_F) )
			{
				if (m_inventory->CanGetItem())
				{
					m_isGetting = true;
				}
			}
		}
		else if (m_isGetting)
		{
			m_isGetting = false;
		}
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

// プレイヤーの体力を減らす処理
void LoadPlayer::DecreaseHP(int damage)
{
	m_hp -= damage;

	// SE:被ダメージ
	PlaySoundMem(m_seDamage, DX_PLAYTYPE_BACK);

	if (m_hp <= 0)
	{
		m_hp = 0;
	}
}

//時間停止
void LoadPlayer::TheWorld()
{
	if (m_useTheWorldCount < m_enhanceType->GetMaxUseTheWorldCount() && m_theWorldCoolDown > 0)
	{
		m_theWorldCoolDown -= Time::GetInstance()->GetDeltaTime();
		if (m_theWorldCoolDown <= 0)
		{
			m_theWorldCoolDown = 0;
			// クールダウン状態解除
			m_isCooldown = false;
		}
	}

	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_C) && m_theWorldCoolDown <= 0)
	{
		m_isStop = true;
		// 使用したらクールダウン中扱い
		m_isCooldown = true;
	}

	if (m_isStop)
	{
		//m_stopTime = m_enhanceType->GetMaxTheWorldTime();

		m_nowStopTime += Time::GetInstance()->GetDeltaTime();

		if (m_nowStopTime >= m_stopTime)
		{
			m_theWorldCoolDown = TheWorldCoolDown;
			m_useTheWorldCount += 1;
			m_nowStopTime = 0;
			m_isStop = false;
		}
	}
}

// スタミナ管理
void LoadPlayer::StaminaManagement()
{
	if (m_isDash)
	{
		m_staminaDecrease =  m_enhanceType->GetStaminaDecrease();

		//重量オーバーしたらスタミナ消費量を上げる
		if (m_weightOver)
		{
			m_staminaDecrease = m_staminaDecrease * 2;
		}

		// 走っている間はスタミナを減らす
		m_stamina -= m_staminaDecrease * Time::GetInstance()->GetDeltaTime();
		m_duration = 0;
		if (m_stamina <= 0)
		{
			m_stamina = 0;
		}
	}
	else
	{
		m_staminaRecovery = m_enhanceType->GetStaminaRecovery();

		//重量オーバーしたら回復量を下げる
		if (m_weightOver)
		{
			m_staminaRecovery = m_staminaRecovery / 2;
		}

		// すでにスタミナが最大の時
		if (m_stamina == MaxStamina) return;

		// 走っていないとき
		m_duration += Time::GetInstance()->GetDeltaTime();

		if (m_duration >= TimeToRecoverStamina)
		{
			// スタミナ回復までの時間を超えたら回復を始める
			m_stamina += m_staminaRecovery * Time::GetInstance()->GetDeltaTime();
		}

		if (m_stamina >= MaxStamina)
		{
			// スタミナの最大値を超えたら調整
			m_stamina = MaxStamina;
			m_duration = 0;
		}
	}
}

void LoadPlayer::DropItem()
{
	if (m_inventory->GetDropItem())
	{
		//捨てたオブジェクトを生成

		int num = std::next(m_inventory->GetItemList().begin(), m_inventory->GetTakeItem())->GetItemNum();

		GetParent()->AddChild(
			new Item(
				num,
				m_itemFactory->GetItemData(num),
				GetPosition(),
				m_inventory
			));

		m_inventory->GetDropItemCompletion();
	}
}