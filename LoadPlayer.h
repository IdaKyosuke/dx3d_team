#pragma once
#include"Actor3D.h"
#include"Vector3.h"
#include"DxLib.h"
#include<vector>

class Camera;
class Animation3D;
class CollisionStage;

class LoadPlayer : public Actor3D
{
public:
	// アニメーション
	enum class Anim
	{
		Idle,
		Walk,
		Run,
		Jump,
		JumpUp,
		JumpIdle,
		Landing,
		Floating,

		Length,
	};

private:
	std::vector<Animation3D*> m_attachAnimList;

	static constexpr int AnimNum = static_cast<int>(Anim::Length);	// アニメーションの数

	static constexpr float WalkSpeed = 8.0f;	// 歩く速度
	static constexpr float RunSpeed = 12.0f;	// 走る速度
	static constexpr float RotateSpeed = 10.0f;	// 回転速度
	static constexpr float DurationTime = 1.0f;
	static constexpr float JumpPower = 4.0f;	// 初速度
	static constexpr float Gravity = 9.8f;		// 重力加速度
	static constexpr Vector3 SpawnPos = Vector3(850, 100, 850);	// ステージにスポーンする場所
	static constexpr Vector3 ColOffset = Vector3(0, 90, 0);	// コライダーのオフセット
	static constexpr Vector3 ColSize = Vector3(70, 180, 70);	// コライダーのサイズ

	Vector3 AxisY = Vector3(0.0f, 1.0f, 0.0f);	// 回転軸(Y軸で上方向)

	static const char* AnimList[AnimNum];

	int m_model;	// プレイヤーのモデル(mv1)

	int m_animIndex;	// 再生中のアニメーション

	int m_lightHandle;	// 光源を作成する
	bool m_isJump;
	bool m_isJumping;
	bool m_isFall;	// 現在落下しているか
	bool m_isFloating;
	float m_fallStartY;	// 落下し始めの高さ

	Camera* m_camNode;
	CollisionStage* m_collisionStage;

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
	float m_duration;	// 時間経過をカウントする用
	static constexpr float m_time = 1.0f;	// 無敵時間

	// プレイヤーのジャンプ処理
	void Jumping();		// 自分でジャンプする処理


	bool m_hit;	// テスト用

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	LoadPlayer(CollisionStage* collisionStage);

	// アニメーションを切り替える(Lerp)
	void ChangeAnimLerp();

	// アニメーションを切り替える(即座)
	void ChangeAnimQuick(const Anim nextAnim);

	// アニメーションを再生する
	void PlayAnim();

	// モデル関係を削除
	void Finalize();

	// プレイヤーの通常移動
	void NormalMove();


	// 無敵時間
	void InvincibleTime();

	// 落下した高さを計算する
	void CountFallHeight();

	// プレイヤーの体力を減らす処理
	void DecreaseHP(int damage)
	{
		m_hp -= damage;
		if (m_hp <= 0)
		{
			m_hp = 0;
		}
	}

	bool IsJump()
	{
		return m_isJump;
	}
};