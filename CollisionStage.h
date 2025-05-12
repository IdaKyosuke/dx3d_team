#pragma once
#pragma once
#include"Node.h"
#include"Vector3.h"
#include"DxLib.h"

class CollisionStage : public Node
{
private:
	static constexpr Vector3 DeffRay = Vector3(0, 100, 0);	// 足場確認用のレイを体内にめり込ませる長さ
	static constexpr Vector3 CheckLineLength = Vector3(0, 3000, 0);	// 足場があるかどうかを確認するレイの長さ
	static constexpr Vector3 DiffGetLine = Vector3(0, 60, 0);	// プレイヤーの足元までのレイの長さ
	static constexpr Vector3 ModelHeight = Vector3(0, 180, 0);		// 使用しているモデルの高さ
	static constexpr float Radius = 60.0f;		// カプセルに使う球の半径
	static constexpr float CapsuleLength = 180.0f;	// カプセルの長さ（球同士の距離）
	static constexpr float DiffCapsule = 10.0f;	// カプセルが足元と頭上の判定をとらないための差分

	int m_modelFloor;	// 床モデルハンドル
	int m_modelWall;	// 壁モデルハンドル
	MV1_COLL_RESULT_POLY m_stagePoly;	// レイの先にあるメッシュの情報
	MV1_COLL_RESULT_POLY m_polyHeight;	// レイの先にあるメッシュの情報
	MV1_COLL_RESULT_POLY_DIM m_camFrontPoly;	// レイの先にあるメッシュの情報（カメラ用）
	MV1_REF_POLYGONLIST m_refPolyFloor;	// ステージモデル全体の参照用メッシュ情報（床）
	MV1_REF_POLYGONLIST m_refPolyWall;	// ステージモデル全体の参照用メッシュ情報（壁）

	MV1_COLL_RESULT_POLY_DIM m_spherePoly;	// 球の当たり判定

	// 移動可能範囲のチェック用(プレイヤーの足元より下に長く)
	Vector3 m_checkMoveStart;
	Vector3 m_checkMoveEnd;

	// 今いるフィールドの高さ取得用（プレイヤーの足元までの長さ）
	Vector3 m_getHeightStart;
	Vector3 m_getHeightEnd;

	// デバッグ用
	Vector3 m_cap1;

protected:
	virtual void Release() override;	// リソースの解放
	virtual void Draw() override;

public:
	CollisionStage(const char* floorModelPath, const char* wallModelPath, const Vector3& pos);

	// 移動予定先に足場があるかどうか
	int CheckStage(const Vector3& pos);

	// プレイヤーの足元までのレイ(構造体を返す)
	MV1_COLL_RESULT_POLY GetHeight(const Vector3& pos);

	// カメラの描画距離を調整する用
	MV1_COLL_RESULT_POLY_DIM CheckCamHitWall(Vector3 camPos, Vector3 camTargetPos);

	// プレイヤーのカプセルとステージの当たり判定をとる
	int CapsuleCollider(const Vector3& pos);

	// 使用しているレイの長さを取得（反転状態のy座標取得用）
	Vector3 RayLength()
	{
		return DiffGetLine;
	}

	// 床のメッシュモデルを取得
	int GetMeshModel()
	{
		return m_modelFloor;
	}

	// 球とステージの壁の当たり判定情報
	MV1_COLL_RESULT_POLY_DIM GetColSphere()
	{
		return m_spherePoly;
	}

	// カメラ用線分と壁の当たり判定
	MV1_COLL_RESULT_POLY_DIM GetColCamLine()
	{
		return m_camFrontPoly;
	}

	// 指定の座標の直下、若しくは直上にあるポリゴンの番号を取得する
	int CheckOnPolyIndex(const Vector3& pos, const MV1_REF_POLYGONLIST& polyList);
};