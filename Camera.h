#pragma once
#include"Actor.h"
#include"Vector3.h"

class LoadPlayer;

class Camera : public Actor
{
private:
	static constexpr Vector3 StartPos = Vector3(1300, 200, 300);
	static constexpr float DiffY = 150;
	static constexpr float CamDiff = 400.0f;	// プレイヤーとカメラの距離
	static constexpr float DiffTagY = 200.0f;	// 上を向く時の差分
	static constexpr float AddAngleY = 2.0f;	// y軸の差分を加算していくための値
	static constexpr float DecMouseDiff = 10.0f;	// マウスの移動量を回転に使うために、値を減らす用
	static constexpr float DivAngle = 720.0f;	// マウスの移動量に対するカメラの回転量を決める

	Vector3 m_camPos;
	Vector3 m_camTarget;
	Vector3 m_pastPlayerPos;

	LoadPlayer* m_loadPlayerNode;
	bool m_pastTerning;		// 1フレーム前の重力の向き
	float m_diffY;		// カメラ角度を少しずつ変えるよう

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	Camera(LoadPlayer* player);

	// カメラの場所と焦点を設定
	void SetCamPosAndTag();

	// カメラの正面ベクトルを取得する(XYZ)
	Vector3 CamFrontVec();

	// カメラの正面ベクトルを取得(XZ平面)
	Vector3 CamFrontPlaneVec();

	// カメラの右方向ベクトルを取得
	Vector3 CamRight();

	// プレイヤーの方を向く
	void ChasePlayer(const Vector3& playerPos);

	// プレイヤーを中心に回転する3人称視点カメラ
	void ThirdPersonCam(const Vector3& playerPos);

	// プレイヤーと一緒に移動する1人称カメラ
	void FirstPersonCam(const Vector3& playerPos);

	// 反重力を実装する場合のカメラ
	void AntiGravityCam(const Vector3& playerPos);
};