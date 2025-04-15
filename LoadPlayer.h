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
	// ループするアニメーション
	enum class RoopAnim
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

	// ループしないアニメーション
	enum class NoRoopAnim
	{

	};

	// 現在の移動モード
	enum MoveMode
	{
		Normal,
		ZeroGravity,
	};

private:
	std::vector<Animation3D*> m_attachAnimList;

	static constexpr int RoopAnimNum = static_cast<int>(RoopAnim::Length);	// アニメーションの数

	static constexpr float WalkSpeed = 8.0f;	// 歩く速度
	static constexpr float RunSpeed = 12.0f;	// 走る速度
	static constexpr float RotateSpeed = 10.0f;	// 回転速度
	static constexpr float DurationTime = 1.0f;
	static constexpr float JumpPower = 4.0f;	// 初速度
	static constexpr float Gravity = 9.8f;		// 重力加速度
<<<<<<< HEAD
	static constexpr Vector3 SpawnPos = Vector3(950, 60, 90);	// ステージにスポーンする場所
=======
	static constexpr Vector3 SpawnPos = Vector3(1000, 1000, 1000);	// ステージにスポーンする場所
>>>>>>> navMesh
	static constexpr float RestartHeight = 3000.0f;	// リスポーンする高さの絶対値
	static constexpr float FloatHeight = 50.0f;		// 無重力状態の高さ
	static constexpr Vector3 ColOffset = Vector3(0, 90, 0);	// コライダーのオフセット
	static constexpr Vector3 ColSize = Vector3(70, 180, 70);	// コライダーのサイズ

	Vector3 AxisY = Vector3(0.0f, 1.0f, 0.0f);	// 回転軸(Y軸で上方向)

	static const char* AnimList[RoopAnimNum];

	int m_model;	// プレイヤーのモデル(mv1)

	int m_animIndex;	// 再生中のアニメーション

	int m_lightHandle;	// 光源を作成する
	bool m_isJump;
	bool m_isJumping;
	bool m_isFall;
	bool m_isFloating;
	bool m_isTerning;	// 重力が反転しているかどうか

	Camera* m_camNode;
	CollisionStage* m_collisionStage;

	Vector3 m_playerPastPos;
	Vector3 m_playerRotate;
	Vector3 m_moveDirection;


	// アニメーション切り替え用
	RoopAnim m_nowAnim;
	RoopAnim m_nextAnim;


	// 移動モード切り替え用
	MoveMode m_moveMode;

	bool m_moving;	// 移動アニメーションが再生中か
	float m_elapsedTime;	// 経過時間保持
	float m_fallTime;		// 自由落下を加速させる用
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
	void ChangeAnimQuick(const RoopAnim nextAnim);

	// アニメーションを再生する
	void PlayAnim();

	// モデル関係を削除
	void Finalize();

	// プレイヤーの通常移動
	void NormalMove();
	// 無重力状態の移動
	void ZeroGravityMove();

	// 移動モードの切り替え
	void ChangeMoveMode();

	bool IsJump()
	{
		return m_isJump;
	}

	bool IsTerning()
	{
		return m_isTerning;
	}
	virtual void OnCollision(const Actor3D* other) override;
};