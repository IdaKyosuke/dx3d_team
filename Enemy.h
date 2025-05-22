#pragma once
#include"Actor3D.h"
#include"Vector3.h"
#include"NavMesh.h"
#include"DxLib.h"
#include<vector>
#include <thread>
#include <atomic>

class Animation3D;
class LoadPlayer;
class CheckRoot;

class Enemy : public Actor3D
{
public:
	enum class Anim
	{
		Idle,
		Run,

		Attack,

		Length,
	};

private:
	static constexpr int AnimNum = static_cast<int>(Anim::Length);	// アニメーションの数
	static constexpr float MoveSpeedCombat = 5.5f;	// プレイヤー発見時の移動速度
	static constexpr float MoveSpeed = 2.0f;	// 徘徊時の移動速度
	static constexpr float Width = 300.0f;	// 敵の幅
	static const char* AnimList[AnimNum];
	static constexpr Vector3 FindColSize = Vector3(900, 200, 900);	// プレイヤーを見つけるための当たり判定のサイズ
	static constexpr Vector3 AttackColSize = Vector3(300, 200, 300);	// 攻撃判定のサイズ
	static constexpr Vector3 ColOffset = Vector3(0, 100, 0);	// コライダーのオフセット
	static constexpr int Power = 10;		// 攻撃力
	static constexpr float CoolTime = 1.0f;	// 攻撃後のクールタイム

	std::vector<Animation3D*> m_attachAnimList;
	int m_model;	// 敵のモデル
	int m_animIndex;	// 再生中のアニメーション
	Vector3 m_enemyPos;
	Vector3 m_enemyPastPos;

	// アニメーション切り替え用
	Anim m_nowAnim;
	Anim m_nextAnim;

	// モデルの回転用
	Vector3 m_moveDirection;

	//static const char* AnimList[AnimNum];	// アニメーションを指定する用
	NavMesh* m_navMesh;	// 経路探索用
	LoadPlayer* m_player;	// プレイヤー

	bool m_isSet;
	bool m_isMove;	// 徘徊中 && 目的地に向かっている途中か
	bool m_isFind;	// プレイヤーを見つけたか
	bool m_isAttack;	// 攻撃時に立ち止まるならこのフラグを使う
	bool m_countCoolTime;	// クールタイムカウント開始フラグ
	float m_durationCoolTime;	// クールタイムの経過時間
	CheckRoot* m_checkRoot;	// ステージ情報から経路探索を行う用
	int m_polyCount;	// 目的地までのポリゴン数
	bool m_isCheck;		// プレイヤー発見時、プレイヤーまでの探索が終わっているか
	bool m_pastAttackFlg;	// 攻撃時に再探索を行う用

	bool m_stoped;	// 前のフレームで動きを止められていたか

	std::thread* m_thread = nullptr;
	bool m_found = false;	// 探索が終わったかどうか

protected:
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Release() override;
	virtual void OnCollision(const Actor3D* other) override;

public:
	Enemy(NavMesh* navMesh, const Vector3& pos, LoadPlayer* loadPlayer);

	// アニメーションを切り替える(Lerp)
	void ChangeAnimLerp();

	// アニメーションを切り替える(即座)
	void ChangeAnimQuick(const Anim nextAnim);

	// アニメーションを再生する
	void PlayAnim();

	// 敵の移動（臨戦態勢）
	void MoveCombat();

	// 敵の移動（徘徊）
	void MoveWanderAround();

	// 敵の攻撃
	void Attack();

	// 敵の位置を返す
	Vector3 EnemyPos()
	{
		return m_enemyPos;
	}
};
