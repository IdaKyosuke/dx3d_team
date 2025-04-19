#pragma once
#include"Vector3.h"
#include"DxLib.h"

// ポリゴン同士の連結情報
struct PolyLinkInfo
{
	int linkPolyIndex[3];	// ポリゴンの3つの辺と隣接しているポリゴン番号(-1:隣接なし, それ以外:ポリゴン番号)
	float linkPolyDistance[3];	// 隣接しているポリゴンとの距離
	Vector3 centerPos;	// ポリゴンの中心座標
};

// １ポリゴンの情報
struct PathPlanUnit
{
	int polyIndex;	// ポリゴン番号
	float totalDistance;	// ポリゴンに到達するまでに通過したポリゴン間の距離の合計
	int prevPolyIndex;	// 確定している１つ前のポリゴン（ポリゴンがないときは-1）
	int nextPolyIndex;	// 確定している１つ先のポリゴン（ポリゴンがないときは-1）
	PathPlanUnit* activeNext;	// 経路探索対象の次のポリゴンのアドレスを格納
};

class CollisionStage;

class NavMesh
{
private:
	MV1_REF_POLYGONLIST m_polyList;	// ステージのポリゴン情報
	PolyLinkInfo* m_polyLink;	// ステージモデルの全ポリゴン分の連結情報が格納された配列

	CollisionStage* m_collisionStage;

public:
	NavMesh(CollisionStage* collisionStage);

	// 指定座標の直下、直上のポリゴン番号を取得（ない場合は-1）
	int CheckPolyIndex(Vector3 pos);

	// ポリゴン同士の連結情報を作成
	void SetPolyLinkInfo();

	// ポリゴン同士の連結情報の削除
	void RemovePolyLinkInfo();

	// ポリゴンの情報を使って、ポジションを返す
	Vector3 GetPos(const int polygonNum = 0);

	// ポリゴンの連結情報を取得
	PolyLinkInfo* GetPolyLinkInfo()
	{
		return m_polyLink;
	}

	// ポリゴンの情報を取得
	MV1_REF_POLYGONLIST GetPolyList()
	{
		return m_polyList;
	}
};
