#pragma once
#include"Actor3D.h"
#include"Vector3.h"

class LoadPlayer;
class CollisionStage;

class Camera : public Actor3D
{
public:
	enum SightMode
	{
		First,
		Third,
	};

private:
	static constexpr Vector3 StartPos = Vector3(1300, 250, 300);
	static constexpr float DiffY = 150;
	static constexpr float CamDiff = 400.0f;	// プレイヤーとカメラの距離
	static constexpr float DiffTagY = 200.0f;	// 上を向く時の差分
	static constexpr float AddAngleY = 2.0f;	// y軸の差分を加算していくための値
	static constexpr float DecMouseDiff = 10;	// マウスの移動量を回転に使うために、値を小さくする割合
	static constexpr float CamRot = 720.0f;	// マウスの移動量に対するカメラの回転の分母
	static constexpr float CamNearDist = 10.0f;	// カメラに映る最も近い距離
	static constexpr float CamFarDist = 20000.0f;// カメラに映る最も遠い距離
	// １人称用
	static constexpr float MaxCamHeight = 500.0f;	// カメラの注視点の最大の高さ差分
	static constexpr float MinCamHeight = -100.0f;	// カメラの注視点の最低の高さ差分

	Vector3 m_camTarget;
	Vector3 m_pastPlayerPos;

	LoadPlayer* m_loadPlayerNode;
	CollisionStage* m_collisionStage;
	bool m_pastTerning;		// 1フレーム前の重力の向き
	float m_diffY;		// カメラ角度を少しずつ変えるよう
	SightMode m_sightMode;	// 今の視点モード

	bool m_isLocked;
	bool m_isChange;	// カメラのモード切り替え中か

protected:
	virtual void Update() override;
	virtual void Draw() override;

public:
	Camera(LoadPlayer* player, CollisionStage* collisionStage);

	// カメラの場所と焦点を設定
	void SetCamPosAndTag();

	// カメラの正面ベクトルを取得する(XYZ)
	Vector3 CamFrontVec();

	// カメラの正面ベクトルを取得(XZ平面)
	Vector3 CamFrontPlaneVec();

	// カメラの右方向ベクトルを取得
	Vector3 CamRight();

	// カメラを動かす
	void MoveCam(const Vector3& playerPos);

	// 3人称視点
	void ThirdPerson(const Vector3& playerPos);

	// 1人称視点
	void FirstPerson(const Vector3& playerPos);

	// カメラのモードを変える
	void ChangeSightMode(const Vector3& playerPos);

	// カメラのモードを切り替える予約
	void ModeChange()
	{
		m_isChange = true;
	}

	// カメラの描画距離を調整
	void SetCamClipRange();
};