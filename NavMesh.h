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
	const float MoveSpeed = 1.0f;
	const float Width = 1.0f;

	// 経路探索で使用する情報を保存
	Vector3 m_startPos;	// 探索開始位置
	Vector3 m_endPos;	// 探索目標位置
	PathPlanUnit* m_unitArray;	// 全ポリゴンの情報配列が格納されたメモリ領域の先頭アドレス
	PathPlanUnit* m_activeFirst;	// ポリゴン群の最初のポリゴン情報へのアドレス
	PathPlanUnit* m_start;	// 探索のスタート地点にあるポリゴン情報へのアドレス
	PathPlanUnit* m_goal;	// 探索の目標地点にあるポリゴン情報へのアドレス

	// 移動処理用
	int m_nowPolyIndex;	// 現在乗っているポリゴン番号
	Vector3 m_nowPos;	// 現在位置
	Vector3 m_moveDirection;	// 移動方向
	PathPlanUnit* m_nowPathPlan;	// 現在乗っているポリゴンの経路探索情報が格納されているアドレス
	PathPlanUnit* m_targetPathPlan;	// 次の中間地点になる経路上のポリゴン情報へのアドレス

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

	// 指定の２点間を直線的に移動できるか
	bool CheckPolyMove(Vector3 startPos, Vector3 goalPos);

	// 指定の２点間を直線的に移動できるか（幅指定版）
	bool CheckPolyMoveWidth(Vector3 startPos, Vector3 goalPos, float width);

	// 指定の２点間を経路探索
	bool SetPathPlan(Vector3 startPos, Vector3 goalPos);
	
	// 経路探索情報を削除
	void RemovePathPlan();

	// 探索経路の移動処理の初期化
	void MoveInitialize();

	// 探索経路の移動処理
	Vector3 Move(const Vector3& pos, const float speed);

	// 探索経路の移動方向を更新（true:目標地点に到達, false:目標地点に未到達）
	bool RefreshMoveDirection(const float speed);
};
