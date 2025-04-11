#include"NavMesh.h"
#include"CollisionStage.h"
#include"Math.h"

NavMesh::NavMesh(CollisionStage* collisionStage) :
	m_collisionStage(collisionStage),
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
	m_targetPathPlan(nullptr)
{
	// 最初にポリゴンの連結情報を構築しておく
	SetPolyLinkInfo();
}

// 指定座標の直下、直上のポリゴン番号を取得（ない場合は-1）
int NavMesh::CheckPolyIndex(Vector3 pos)
{
	return m_collisionStage->CheckOnPolyIndex(pos, m_polyList);
}

// ポリゴン同士の連結情報を作成
void NavMesh::SetPolyLinkInfo()
{
	// ステージのメッシュ情報を取得
	m_polyList = m_collisionStage->GetStageMesh();

	// メモリ領域を確保
	m_polyLink = new PolyLinkInfo [sizeof(PolyLinkInfo) * m_polyList.PolygonNum];

	MV1_REF_POLYGON* refPoly = m_polyList.Polygons;
	// 全ポリゴンの中心座標を算出
	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++)
	{
		m_polyLink[i].centerPos = VScale(
			VAdd(m_polyList.Vertexs[refPoly->VIndex[0]].Position,
			VAdd(m_polyList.Vertexs[refPoly->VIndex[1]].Position, 
				 m_polyList.Vertexs[refPoly->VIndex[2]].Position)),
			1.0f / 3.0f
		);
	}

	// ポリゴン同士の隣接情報を作成
	refPoly = m_polyList.Polygons;
	MV1_REF_POLYGON* refPolySub;

	if (m_polyLink)
	{
		for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++)
		{
			// 最初に隣接情報をリセットする
			for (int j = 0; j < 3; j++)
			{
				m_polyLink[i].linkPolyIndex[j] = -1;
			}

			// 隣接するポリゴンを探すためにポリゴンの数だけ繰り返す
			refPolySub = m_polyList.Polygons;

			for (int j = 0; j < m_polyList.PolygonNum; j++, refPolySub++)
			{
				// 自分自身を無視
				if (i == j) continue;

				if (j == m_polyList.PolygonNum - 2)
				{
					// デバッグ用
					j = m_polyList.PolygonNum - 2;
				}

				// ポリゴン頂点番号(0,1)で形成される辺と隣接していたら隣接情報に追加
				if (
					m_polyLink[i].linkPolyIndex[0] == -1 &&
					((refPoly->VIndex[0] == refPolySub->VIndex[0] && refPoly->VIndex[1] == refPolySub->VIndex[2]) ||
						(refPoly->VIndex[0] == refPolySub->VIndex[1] && refPoly->VIndex[1] == refPolySub->VIndex[0]) ||
						(refPoly->VIndex[0] == refPolySub->VIndex[2] && refPoly->VIndex[1] == refPolySub->VIndex[1]))
				)
				{
					m_polyLink[i].linkPolyIndex[0] = j;
					m_polyLink[i].linkPolyDistance[0] = VSize(VSub(m_polyLink[j].centerPos, m_polyLink[i].centerPos));
				}
				// ポリゴン頂点番号(1,2)で形成される辺と隣接していたら隣接情報に追加
				else if(
					m_polyLink[i].linkPolyIndex[1] == -1 &&
					((refPoly->VIndex[1] == refPolySub->VIndex[0] && refPoly->VIndex[2] == refPolySub->VIndex[2]) ||
						(refPoly->VIndex[1] == refPolySub->VIndex[1] && refPoly->VIndex[2] == refPolySub->VIndex[0]) ||
						(refPoly->VIndex[1] == refPolySub->VIndex[2] && refPoly->VIndex[2] == refPolySub->VIndex[1]))
					)
				{
					m_polyLink[i].linkPolyIndex[1] = j;
					m_polyLink[i].linkPolyDistance[1] = VSize(VSub(m_polyLink[j].centerPos, m_polyLink[i].centerPos));
				}
				// ポリゴン頂点番号(2,0)で形成される辺と隣接していたら隣接情報に追加
				else if (
					m_polyLink[i].linkPolyIndex[2] == -1 &&
					((refPoly->VIndex[2] == refPolySub->VIndex[0] && refPoly->VIndex[0] == refPolySub->VIndex[2]) ||
						(refPoly->VIndex[2] == refPolySub->VIndex[1] && refPoly->VIndex[0] == refPolySub->VIndex[0]) ||
						(refPoly->VIndex[2] == refPolySub->VIndex[2] && refPoly->VIndex[0] == refPolySub->VIndex[1]))
					)
				{
					m_polyLink[i].linkPolyIndex[2] = j;
					m_polyLink[i].linkPolyDistance[2] = VSize(VSub(m_polyLink[j].centerPos, m_polyLink[i].centerPos));
				}
			}
		}
	}
}

// ポリゴン同士の連結情報の削除
void NavMesh::RemovePolyLinkInfo()
{
	delete[] m_polyLink;
}

// 指定の２点間を直線的に移動できるか（連結情報を使用する）
bool NavMesh::CheckPolyMove(Vector3 startPos, Vector3 goalPos)
{
	int startPoly;
	int goalPoly;
	PolyLinkInfo* polyInfoStart;
	PolyLinkInfo* polyInfoGoal;
	Vector3 firstPos;
	Vector3 targetPos;
	Vector3 polyPos[3];
	int checkPoly[3];
	int checkPolyPrev[3];
	int checkPolyNum;
	int checkPolyPrevNum;
	int nextCheckPoly[3];
	int nextCheckPolyPrev[3];
	int nextCheckPolyNum;
	int nextCheckPolyPrevNum;
	int num;

	// 開始座標と目標座標のYを0にして平面上の判定を行う
	startPos.y = 0;
	goalPos.y = 0;

	// 開始座標と目標座標の真下のポリゴンを調べる
	startPoly = CheckPolyIndex(startPos);
	goalPoly = CheckPolyIndex(goalPos);

	// ポリゴンが存在しない場合
	if (startPoly == -1 || goalPoly == -1) return false;

	// 開始座標と目標座標の直上 or 直下のポリゴンを登録
	polyInfoStart = &m_polyLink[startPoly];
	polyInfoGoal = &m_polyLink[goalPoly];


	// 指定した線分上にあるかを判断するためのポリゴンを登録（直上 or 直下）
	checkPolyNum = 1;
	checkPoly[0] = startPoly;
	checkPolyPrevNum = 0;
	checkPolyPrev[0] = -1;

	// 結果が出るまでループ
	while (true)
	{
		// 次のループでのチェック対象のポリゴン数をリセット
		nextCheckPolyNum = 0;

		// 次のループでチェック対象から外すポリゴン数をリセット
		nextCheckPolyPrevNum = 0;

		// チェック対象のポリゴン数だけループ
		for (int i = 0; i < checkPolyNum; i++)
		{
			// チェック対象のポリゴンの3座標を取得
			polyPos[0] = VConvFtoD(m_polyList.Vertexs[m_polyList.Polygons[checkPoly[i]].VIndex[0]].Position);
			polyPos[1] = VConvFtoD(m_polyList.Vertexs[m_polyList.Polygons[checkPoly[i]].VIndex[0]].Position);
			polyPos[2] = VConvFtoD(m_polyList.Vertexs[m_polyList.Polygons[checkPoly[i]].VIndex[0]].Position);

			// y座標を0にして平面判定を行う
			polyPos[0].y = 0.0f;
			polyPos[1].y = 0.0f;
			polyPos[2].y = 0.0f;

			// ポリゴンの頂点(0, 1)に隣接するポリゴンが存在する && 辺の線分と移動開始点、終了点で形成する線分が接している
			if (m_polyLink[checkPoly[i]].linkPolyIndex[0] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[0], polyPos[1]) < 0.001
				)
			{
				if (m_polyLink[checkPoly[i]].linkPolyIndex[0] == goalPoly) return true;

				// 辺と接しているポリゴンを次のチェック対象に追加
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// すでに追加されている場合追加しない
					if (nextCheckPoly[num] == m_polyLink[checkPoly[i]].linkPolyIndex[0]) break;
				}
				if (num == nextCheckPolyNum)
				{
					// 次のループで除外するポリゴンの対象に加える
					for (num = 0; num < nextCheckPolyPrevNum; num++)
					{
						// すでに追加されている場合追加しない
						if (nextCheckPolyPrev[num] == checkPoly[i]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						nextCheckPolyPrev[nextCheckPolyPrevNum] = checkPoly[i];
						nextCheckPolyPrevNum++;
					}

					// １つ前のループでチェック対象になったポリゴンも加えない
					for (num = 0; num < checkPolyPrevNum; num++)
					{
						if (checkPolyPrev[num] == m_polyLink[checkPoly[i]].linkPolyIndex[0]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						// チェック対象に追加
						nextCheckPoly[nextCheckPolyNum] = m_polyLink[checkPoly[i]].linkPolyIndex[0];
						nextCheckPolyNum++;
					}
				}
			}

			// ポリゴンの頂点(1, 2)に隣接するポリゴンが存在する && 辺の線分と移動開始点、終了点で形成する線分が接している
			if (m_polyLink[checkPoly[i]].linkPolyIndex[1] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[1], polyPos[2]) < 0.001
				)
			{
				if (m_polyLink[checkPoly[i]].linkPolyIndex[1] == goalPoly) return true;

				// 辺と接しているポリゴンを次のチェック対象に追加
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// すでに追加されている場合追加しない
					if (nextCheckPoly[num] == m_polyLink[checkPoly[i]].linkPolyIndex[1]) break;
				}
				if (num == nextCheckPolyNum)
				{
					// 次のループで除外するポリゴンの対象に加える
					for (num = 0; num < nextCheckPolyPrevNum; num++)
					{
						// すでに追加されている場合追加しない
						if (nextCheckPolyPrev[num] == checkPoly[i]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						nextCheckPolyPrev[nextCheckPolyPrevNum] = checkPoly[i];
						nextCheckPolyPrevNum++;
					}

					// １つ前のループでチェック対象になったポリゴンも加えない
					for (num = 0; num < checkPolyPrevNum; num++)
					{
						if (checkPolyPrev[num] == m_polyLink[checkPoly[i]].linkPolyIndex[1]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						// チェック対象に追加
						nextCheckPoly[nextCheckPolyNum] = m_polyLink[checkPoly[i]].linkPolyIndex[1];
						nextCheckPolyNum++;
					}
				}
			}

			// ポリゴンの頂点(2, 0)に隣接するポリゴンが存在する && 辺の線分と移動開始点、終了点で形成する線分が接している
			if (m_polyLink[checkPoly[i]].linkPolyIndex[2] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[2], polyPos[0]) < 0.001
				)
			{
				if (m_polyLink[checkPoly[i]].linkPolyIndex[2] == goalPoly) return true;

				// 辺と接しているポリゴンを次のチェック対象に追加
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// すでに追加されている場合追加しない
					if (nextCheckPoly[num] == m_polyLink[checkPoly[i]].linkPolyIndex[2]) break;
				}
				if (num == nextCheckPolyNum)
				{
					// 次のループで除外するポリゴンの対象に加える
					for (num = 0; num < nextCheckPolyPrevNum; num++)
					{
						// すでに追加されている場合追加しない
						if (nextCheckPolyPrev[num] == checkPoly[i]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						nextCheckPolyPrev[nextCheckPolyPrevNum] = checkPoly[i];
						nextCheckPolyPrevNum++;
					}

					// １つ前のループでチェック対象になったポリゴンも加えない
					for (num = 0; num < checkPolyPrevNum; num++)
					{
						if (checkPolyPrev[num] == m_polyLink[checkPoly[i]].linkPolyIndex[2]) break;
					}
					if (num == nextCheckPolyPrevNum)
					{
						// チェック対象に追加
						nextCheckPoly[nextCheckPolyNum] = m_polyLink[checkPoly[i]].linkPolyIndex[2];
						nextCheckPolyNum++;
					}
				}
			}
		}

		// 次のループのチェック対象がないとき、直線的な動きができない
		if (nextCheckPolyNum == 0) return false;

		// 次のチェック対象のポリゴン情報をコピー
		for (int i = 0; i < nextCheckPolyNum; i++)
		{
			checkPoly[i] = nextCheckPoly[i];
		}

		checkPolyNum = nextCheckPolyNum;

		// 次のチェック対象外になるポリゴンをコピー
		for (int i = 0; i < nextCheckPolyPrevNum; i++)
		{
			checkPolyPrev[i] = nextCheckPolyPrev[i];
		}

		checkPolyPrevNum = nextCheckPolyPrevNum;
	}

	return false;
}

// 指定の２点間を直線的に移動できるか（幅指定版）
bool NavMesh::CheckPolyMoveWidth(Vector3 startPos, Vector3 goalPos, float width)
{
	Vector3 direction;
	Vector3 sideDirection;
	Vector3 tempVec;

	// 開始座標から目標座標に直線的に移動できるかチェック
	if (!CheckPolyMove(startPos, goalPos)) return false;

	// 開始座標から目標座標に向かうベクトルを算出
	direction = goalPos - startPos;

	// 平面なベクトルに変換
	direction.y = 0;

	// 開始座標から目標座標へのベクトルに直角な正規化ベクトル
	sideDirection = direction.CrossP(Vector3(0, 1, 0));
	sideDirection = Math::Normalized(sideDirection);

	// 開始座標と目標座標を垂直方向にwidthの半分だけずらして確認
	tempVec = sideDirection / (width / 2);
	if (!CheckPolyMove(startPos + tempVec, goalPos + tempVec)) return false;
	if (!CheckPolyMove(startPos - tempVec, goalPos - tempVec)) return false;

	// 指定幅があっても直線的な移動が可能
	return true;
}

// 指定の２点間を経路探索
bool NavMesh::SetPathPlan(Vector3 startPos, Vector3 goalPos)
{
	int polyIndex;
	PathPlanUnit* pathUnit;
	PathPlanUnit* pathUnit_sub;
	PathPlanUnit* pathUnit_sub2;
	bool isGoal = false;

	// スタート位置とゴール位置を保存
	m_startPos = startPos;
	m_endPos = goalPos;

	// 現在地を設定
	m_nowPos = startPos;

	// 経路探索用のメモリ確保
	m_unitArray = new PathPlanUnit[sizeof(PathPlanUnit) * m_polyList.PolygonNum];

	// 経路探索用のポリゴンの初期化
	for (int i = 0; i < m_polyList.PolygonNum; i++)
	{
		m_unitArray[i].polyIndex = i;
		m_unitArray[i].totalDistance = 0;
		m_unitArray[i].prevPolyIndex = -1;
		m_unitArray[i].nextPolyIndex = -1;
		m_unitArray[i].activeNext = nullptr;
	}

	// スタート地点のポリゴン番号を取得 -> 経路探索用の構造体のアドレスを保存
	polyIndex = CheckPolyIndex(startPos);
	if (polyIndex == -1) return false;
	m_start = &m_unitArray[polyIndex];
	// 経路探索用のポリゴンとしてスタート地点のポリゴンを登録
	m_activeFirst = &m_unitArray[polyIndex];

	// ゴール地点にあるポリゴン番号を取得 -> 経路探索用の構造体のアドレスを保存
	polyIndex = CheckPolyIndex(goalPos);
	if (polyIndex == -1) return false;
	m_goal = &m_unitArray[polyIndex];

	// スタート地点とゴール地点のポリゴンが同じとき
	if (m_start == m_goal) return false;

	// ゴールにたどり着くまで探索
	while (!isGoal)
	{
		// 探索対象のポリゴンを処理
		pathUnit = m_activeFirst;
		m_activeFirst = nullptr;
		for (; pathUnit != nullptr; pathUnit = pathUnit->activeNext)
		{
			// ポリゴンの辺の数分繰り返す
			for (int i = 0; i < 3; i++)
			{
				// 隣接するポリゴンがないとき
				if (m_polyLink[pathUnit->polyIndex].linkPolyIndex[i] == -1) continue;

				// 隣接するポリゴンが経路探索処理が行われている && より距離の長い経路  もしくは　スタート地点のポリゴンの時
				pathUnit_sub = &m_unitArray[m_polyLink[pathUnit->polyIndex].linkPolyIndex[i]];
				if (
					(pathUnit_sub->prevPolyIndex != -1 &&
					pathUnit_sub->totalDistance <= pathUnit->totalDistance + m_polyLink[pathUnit->polyIndex].linkPolyDistance[i]) ||
					pathUnit_sub->polyIndex == m_start->polyIndex
					)
				{
					continue;
				}

				// 隣接するポリゴンがゴール地点の時、ゴールフラグを立てる
				if (pathUnit_sub->polyIndex == m_goal->polyIndex)
				{
					isGoal = true;
				}

				// 隣接するポリゴンに自分のポリゴン情報を代入
				pathUnit_sub->prevPolyIndex = pathUnit->polyIndex;

				// 隣接するポリゴンにここまでの距離を代入
				pathUnit_sub->totalDistance = pathUnit->totalDistance + m_polyLink[pathUnit->polyIndex].linkPolyDistance[i];

				// 次のループの探索対象を追加
				for (pathUnit_sub2 = m_activeFirst; pathUnit_sub2 != nullptr; pathUnit_sub2 = pathUnit_sub2->activeNext)
				{
					// すでに追加されていたら追加しない
					if (pathUnit_sub2 == pathUnit_sub) break;
				}
				if (pathUnit_sub2 == nullptr)
				{
					pathUnit_sub->activeNext = m_activeFirst;
					m_activeFirst = pathUnit_sub;
				}
			}
		}

		// m_activeFirst == nullptr の時、ゴールにたどり着けない
		if (!m_activeFirst) return false;
	}

	// ゴールからスタートまでたどって、経路上の次に移動するべきポリゴン番号を代入
	pathUnit = m_goal;
	do
	{
		pathUnit_sub = pathUnit;
		pathUnit = &m_unitArray[pathUnit_sub->prevPolyIndex];
		pathUnit->nextPolyIndex = pathUnit_sub->polyIndex;
	} while (pathUnit != m_start);

	// 探索終了
	return true;
}

// 経路探索情報を削除
void NavMesh::RemovePathPlan()
{
	// 経路探索の為に確保したメモリ領域を解放
	delete[] m_unitArray;
}

// 探索経路の移動処理の初期化
void NavMesh::MoveInitialize()
{
	// 移動開始時に乗っているポリゴンをスタート地点に設定
	m_nowPolyIndex = m_start->polyIndex;

	/*
	// 移動開始時点の座標はスタート地点にあるポリゴンの中心座標
	m_nowPos = m_polyLink[m_nowPolyIndex].centerPos;
	*/

	// 移動開始時の経路探索情報をスタート地点のポリゴン情報にする
	m_nowPathPlan = m_start;

	// 移動開始時の移動中間地点の経路探索情報もスタート地点のポリゴン
	m_targetPathPlan = m_start;
}

// 探索経路の移動処理
Vector3 NavMesh::Move(const Vector3& pos, const float speed)
{
	// y座標だけ保存
	int y = pos.y;

	// 移動方向の更新、ゴールにたどり着いていたら終了
	if (RefreshMoveDirection(speed)) return Vector3(m_nowPos.x, y, m_nowPos.z);

	// 移動方向の座標に移動
	m_nowPos += m_moveDirection * speed;

	// 現在の座標で乗っているポリゴンを検索
	m_nowPolyIndex = CheckPolyIndex(m_nowPos);

	// 乗っているポリゴンの経路探索情報をアドレスに代入
	m_nowPathPlan = &m_unitArray[m_nowPolyIndex];

	return Vector3(m_nowPos.x, y, m_nowPos.z);
}

// 探索経路の移動方向を更新（true:目標地点に到達, false:目標地点に未到達）
bool NavMesh::RefreshMoveDirection(const float speed)
{
	PathPlanUnit* tempPathUnit;

	// 現在乗っているポリゴンがゴール地点のポリゴンの場合、処理を分岐
	if (m_nowPathPlan == m_goal)
	{
		// 方向は目標座標
		m_moveDirection = m_endPos - m_nowPos;
		m_moveDirection.y = 0.0f;

		// 目標座標までの距離が移動速度以下なら到達したとする
		if (m_moveDirection.Magnitude() <= speed)
		{
			return true;
		}

		m_moveDirection = Math::Normalized(m_moveDirection);

		return false;
	}

	// 現在乗っているポリゴンが移動中間地点の場合、次の中間地点を決定する処理をする
	if (m_nowPathPlan == m_targetPathPlan)
	{
		// 中間地点が決まるまでループ
		while (true)
		{
			tempPathUnit = &m_unitArray[m_targetPathPlan->nextPolyIndex];

			// 経路上の次のポリゴンの中心座標に直線移動できないとき、ループから抜ける
			if (!CheckPolyMoveWidth(m_nowPos, m_polyLink[tempPathUnit->polyIndex].centerPos, Width)) break;

			// チェック対象を経路上の更に１つ先のポリゴンに変更
			m_targetPathPlan = tempPathUnit;

			// ゴール地点のポリゴンの時ループを抜ける
			if (m_targetPathPlan == m_goal) break;
		}
	}

	// 移動方向を決定する（移動方向は現在の座標から中間地点のポリゴンの中心座標に向かう）
	m_moveDirection = m_polyLink[m_targetPathPlan->polyIndex].centerPos - m_nowPos;
	m_moveDirection.y = 0;
	m_moveDirection = Math::Normalized(m_moveDirection);

	// ゴールにたどり着いていない
	return false;
}