#pragma once
#include"Vector3.h"
#include"NavMesh.h"
#include"DxLib.h"

class CheckRoot
{
private:
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
	PathPlanUnit* m_pastPoly;	// 1つ前のポリゴン情報
	NavMesh* m_navMesh;

public:
	CheckRoot(NavMesh* navMesh) :
		m_navMesh(navMesh),
		m_startPos(Vector3(0, 0, 0)),
		m_endPos(Vector3(0, 0, 0)),
		m_unitArray(nullptr),
		m_activeFirst(nullptr),
		m_start(nullptr),
		m_goal(nullptr),
		m_nowPolyIndex(0),
		m_nowPos(Vector3(0, 0, 0)),
		m_moveDirection(Vector3(0, 0, 0)),
		m_nowPathPlan(nullptr),
		m_targetPathPlan(nullptr),
		m_pastPoly(nullptr) {}

	// 指定の２点間を直線的に移動できるか
	bool CheckPolyMove(Vector3 startPos, Vector3 goalPos);

	// 指定の２点間を直線的に移動できるか（幅指定版）
	bool CheckPolyMoveWidth(Vector3 startPos, Vector3 goalPos, float width);

	// 指定の２点間を経路探索
	bool SetPathPlan(Vector3 startPos, Vector3 goalPos, int* polyCount, bool* found);

	// 経路探索情報を削除
	void RemovePathPlan();

	// 探索経路の移動処理の初期化
	void MoveInitialize(const Vector3& pos);

	// 探索経路の移動処理
	Vector3 Move(const Vector3& pos, const float speed, const float width, int* polyCount);

	// 探索経路の移動方向を更新（true:目標地点に到達, false:目標地点に未到達）
	bool RefreshMoveDirection(const float speed, const float width, int* polyCount);

	// 現在のポリゴンにプレイヤーがいるかどうか
	bool CheckPlayerPoly(const Vector3& enemyPos, const Vector3& playerPos);
};