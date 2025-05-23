#pragma once
#include"Actor3D.h"
#include"Vector3.h"
#include"DxLib.h"
#include<vector>

class Camera;
class Animation3D;
class CollisionStage;
class Inventory;
class EnhanceType;
class ItemFactory;

class LoadPlayer : public Actor3D
{
public:
	// アニメーション
	enum class Anim
	{
		Idle,
		Walk,
		Run,
		Death,

		Length,
	};

private:
	std::vector<Animation3D*> m_attachAnimList;

	static constexpr int AnimNum = static_cast<int>(Anim::Length);	// アニメーションの数

	static constexpr float WalkSpeed = 4.0f;	// 歩く速度
	static constexpr float RunSpeed = 12.0f;	// 走る速度
	static constexpr float WeightOverSpeed = 6.0f;	//容量オーバーしたときの走る速度
	static constexpr float RotateSpeed = 10.0f;	// 回転速度
	static constexpr float DurationTime = 1.0f;
	static constexpr float JumpPower = 4.0f;	// 初速度
	static constexpr float Gravity = 9.8f;		// 重力加速度
	static constexpr Vector3 SpawnPos = Vector3(850, 100, 850);	// ステージにスポーンする場所
	static constexpr Vector3 ColOffset = Vector3(0, 90, 0);	// コライダーのオフセット
	static constexpr Vector3 ColSize = Vector3(150, 180, 150);	// コライダーのサイズ
	static constexpr int MaxHp = 100;	// 体力の最大値
	static constexpr float TheWorldCoolDown = 30;		//スキルのクールダウン

	// スタミナ関連
	static constexpr float MaxStamina = 100;	// 走るのに必要なスタミナの最大値
	static constexpr float StaminaRecoveryAmount = 10;	// スタミナの回復量
	static constexpr float StaminaDecreaseAmount = 20;	// スタミナの消費量
	static constexpr float TimeToRecoverStamina = 1;	// スタミナが回復し始めるまでの時間

	Vector3 AxisY = Vector3(0.0f, 1.0f, 0.0f);	// 回転軸(Y軸で上方向)

	static const char* AnimList[AnimNum];

	int m_model;	// プレイヤーのモデル(mv1)

	int m_animIndex;	// 再生中のアニメーション

	float m_runSpeed;
	bool m_isJump;
	bool m_isJumping;
	bool m_isFall;	// 現在落下しているか
	bool m_isFloating;
	float m_fallStartY;	// 落下し始めの高さ

	int m_maxHaveWeight;	//持てる最大容量
	bool m_weightOver;		//持てる最大容量が超えたか

	//時間停止に関する変数
	int m_useTheWorldCount;	//何回使ったか
	float m_theWorldCoolDown;
	float m_nowStopTime;	//時間停止してから何秒経ったか
	float m_stopTime;		//時間停止できる時間
	bool m_isStop;			//時間停止してるか
	bool m_isCooldown;		// クールダウン中か

	bool m_isGetting;	//アイテムを拾ったか

	int m_seDamage;	// 攻撃を受けたときのSE

	bool m_isDeath;	// 死亡フラグ
	bool m_finish;	// 体力が0になったかどうか

	Camera* m_camNode;
	CollisionStage* m_collisionStage;

	Inventory* m_inventory;
	EnhanceType* m_enhanceType;
	ItemFactory* m_itemFactory;

	Vector3 m_playerPastPos;
	Vector3 m_playerRotate;
	Vector3 m_moveDirection;

	// アニメーション切り替え用
	Anim m_nowAnim;
	Anim m_nextAnim;

	bool m_moving;	// 移動アニメーションが再生中か
	float m_elapsedTime;	// 経過時間保持
	float m_fallTime;		// 自由落下を加速させる用

	// プレイヤーの体力に関する変数
	int m_hp;	// プレイヤーの体力

	float m_stamina;	// プレイヤーの現在のスタミナ	
	bool m_isDash;		// 走っているか
	float m_duration;	// 時間経過をカウントする用
	float m_staminaRecovery;	// スタミナの回復量
	float m_staminaDecrease;	// スタミナの消費量

	// プレイヤーのジャンプ処理
	void Jumping();		// 自分でジャンプする処理

	// ---- SE ----
	int m_seWalk;
	int m_seRun;

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Load() override;
	virtual void Release() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	LoadPlayer(
		CollisionStage* collisionStage, 
		Inventory* inventory, 
		EnhanceType* enhanceType,
		const Vector3& pos,
		ItemFactory* itemFactory
	);

	// アニメーションを切り替える(Lerp)
	void ChangeAnimLerp();

	// アニメーションを切り替える(即座)
	void ChangeAnimQuick(const Anim nextAnim);

	// アニメーションを再生する
	void PlayAnim();

	// プレイヤーの通常移動
	void NormalMove();

	// 移動先を決める
	void CheckMove();


	// 落下した高さを計算する
	void CountFallHeight();

	// プレイヤーの体力を減らす処理
	void DecreaseHP(int damage);

	//時間停止
	void TheWorld();

	bool IsJump()
	{
		return m_isJump;
	}

	// 時間が止まっているか
	bool IsTheWorld()
	{
		return m_isStop;
	}

	// クールダウン中か
	bool IsCoolDown()
	{
		return m_isCooldown;
	}

	// 停止できる時間を取得
	float GetStopTime()
	{
		return m_stopTime;
	}

	// 現在のHpの割合を返す
	float GetHpRatio()
	{
		return static_cast<float>(m_hp) / static_cast<float>(MaxHp);
	}

	float GetTheWorldTime()
	{
		return m_stopTime;
	}

	float SetTheWorldTime(float theWorldTime)
	{
		m_stopTime = theWorldTime;
	}

	bool GetIsGetting()
	{
		return m_isGetting;
	}

	void GetEnd()
	{
		m_isGetting = false;
	}

	// 死亡フラグを取得
	bool GetIsDeath()
	{
		return m_isDeath;
	}

	// スタミナ管理
	void StaminaManagement();

	// 現在のスタミナを取得
	float GetStamina()
	{
		return m_stamina;
	}

	// スタミナの最大値を取得
	float GetMaxStamina()
	{
		return MaxStamina;
	}

	// 体力が0になったか
	bool IsFinsh()
	{
		return m_finish;
	}

	//アイテムを落とした時
	void DropItem();
};