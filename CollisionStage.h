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
	static constexpr Vector3 DiffGetLine = Vector3(0, 20, 0);	// プレイヤーの足元までのレイの長さ
	static constexpr Vector3 ModelHeight = Vector3(0, 180, 0);		// 使用しているモデルの高さ
	static constexpr float Radius = 30.0f;		// カプセルに使う球の半径
	static constexpr float CapsuleLength = 180.0f;	// カプセルの長さ（球同士の距離）
	static constexpr float DiffCapsule = 10.0f;	// カプセルが足元と頭上の判定をとらないための差分

	int m_model;	// 読み込まれたモデルハンドル
	MV1_COLL_RESULT_POLY m_stagePoly;	// レイの先にあるメッシュの情報
	MV1_COLL_RESULT_POLY m_polyHeight;	// レイの先にあるメッシュの情報
	MV1_REF_POLYGONLIST m_refPoly;	// ステージモデル全体の参照用メッシュ情報

	// 移動可能範囲のチェック用(プレイヤーの足元より下に長く)
	Vector3 m_checkMoveStart;
	Vector3 m_checkMoveEnd;

	// 今いるフィールドの高さ取得用（プレイヤーの足元までの長さ）
	Vector3 m_getHeightStart;
	Vector3 m_getHeightEnd;

	Vector3 m_cap1;
	Vector3 m_cap2;

protected:
	virtual void Release() override;	// リソースの解放
	virtual void Draw() override;

public:
	CollisionStage(const char* modelPath, const Vector3& pos);

	// 移動予定先に足場があるかどうか
	int CheckStage(const Vector3& pos);

	// プレイヤーの足元までのレイ(構造体を返す)
	MV1_COLL_RESULT_POLY GetHeight(const Vector3& pos);

	// プレイヤーのカプセルとステージの当たり判定をとる
	int CapsuleCollider(const Vector3& pos, bool terning);

	// 使用しているレイの長さを取得（反転状態のy座標取得用）
	Vector3 RayLength()
	{
		return DiffGetLine;
	}

	// ステージのメッシュ情報を取得する
	MV1_REF_POLYGONLIST GetStageMesh()
	{
		return m_refPoly;
	}
	// メッシュモデルを取得
	int GetMeshModel()
	{
		return m_model;
	}

	// 指定の座標の直下、若しくは直上にあるポリゴンの番号を取得する
	int CheckOnPolyIndex(const Vector3& pos, const MV1_REF_POLYGONLIST& polyList);
};