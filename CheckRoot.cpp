#include"CheckRoot.h"
#include"Math.h"

// 指定の２点間を直線的に移動できるか（連結情報を使用する）
bool CheckRoot::CheckPolyMove(Vector3 startPos, Vector3 goalPos)
{
	int startPoly;
	int goalPoly;
	PolyLinkInfo* polyInfoStart;
	PolyLinkInfo* polyInfoGoal;
	Vector3 firstPos;
	Vector3 targetPos;
	Vector3 polyPos[3] = { Vector3(0, 0, 0) };
	int checkPoly[3] = { 0 };
	int checkPolyPrev[3] = { 0 };
	int checkPolyNum;
	int checkPolyPrevNum;
	int nextCheckPoly[3] = { 0 };
	int nextCheckPolyPrev[3] = { 0 };
	int nextCheckPolyNum;
	int nextCheckPolyPrevNum;
	int num;

	// 開始座標と目標座標のYを0にして平面上の判定を行う
	startPos.y = 0;
	goalPos.y = 0;

	firstPos = startPos;
	targetPos = goalPos;

	// 開始座標と目標座標の真下のポリゴンを調べる
	startPoly = m_navMesh->CheckPolyIndex(startPos);
	goalPoly = m_navMesh->CheckPolyIndex(goalPos);

	// ポリゴンが存在しない場合
	if (startPoly == -1 || goalPoly == -1) return false;

	// 開始座標と目標座標の直上 or 直下のポリゴンを登録
	polyInfoStart = &m_navMesh->GetPolyLinkInfo()[startPoly];
	polyInfoGoal = &m_navMesh->GetPolyLinkInfo()[goalPoly];


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

		if (checkPolyNum >= 4) checkPolyNum = 3;

		// チェック対象のポリゴン数だけループ
		for (int i = 0; i < checkPolyNum; i++)
		{
			// チェック対象のポリゴンの3座標を取得
			polyPos[0] = VConvFtoD(m_navMesh->GetPolyList().Vertexs[m_navMesh->GetPolyList().Polygons[checkPoly[i]].VIndex[0]].Position);
			polyPos[1] = VConvFtoD(m_navMesh->GetPolyList().Vertexs[m_navMesh->GetPolyList().Polygons[checkPoly[i]].VIndex[1]].Position);
			polyPos[2] = VConvFtoD(m_navMesh->GetPolyList().Vertexs[m_navMesh->GetPolyList().Polygons[checkPoly[i]].VIndex[2]].Position);

			// y座標を0にして平面判定を行う
			polyPos[0].y = 0.0f;
			polyPos[1].y = 0.0f;
			polyPos[2].y = 0.0f;

			// ポリゴンの頂点(0, 1)に隣接するポリゴンが存在する && 辺の線分と移動開始点、終了点で形成する線分が接している
			if (m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[0] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[0], polyPos[1]) < 0.01
				)
			{
				if (m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[0] == goalPoly) return true;

				// 辺と接しているポリゴンを次のチェック対象に追加
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// すでに追加されている場合追加しない
					if (nextCheckPoly[num] == m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[0]) break;
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
						if (checkPolyPrev[num] == m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[0]) break;
					}
					if (num == checkPolyPrevNum)
					{
						// チェック対象に追加
						nextCheckPoly[nextCheckPolyNum] = m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[0];
						nextCheckPolyNum++;
					}
				}
			}

			// ポリゴンの頂点(1, 2)に隣接するポリゴンが存在する && 辺の線分と移動開始点、終了点で形成する線分が接している
			if (m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[1] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[1], polyPos[2]) < 0.01
				)
			{
				if (m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[1] == goalPoly) return true;

				// 辺と接しているポリゴンを次のチェック対象に追加
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// すでに追加されている場合追加しない
					if (nextCheckPoly[num] == m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[1]) break;
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
						if (checkPolyPrev[num] == m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[1]) break;
					}
					if (num == checkPolyPrevNum)
					{
						// チェック対象に追加
						nextCheckPoly[nextCheckPolyNum] = m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[1];
						nextCheckPolyNum++;
					}
				}
			}

			// ポリゴンの頂点(2, 0)に隣接するポリゴンが存在する && 辺の線分と移動開始点、終了点で形成する線分が接している
			if (m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[2] != -1 &&
				Segment_Segment_MinLength_SquareD(firstPos, targetPos, polyPos[2], polyPos[0]) < 0.01
				)
			{
				if (m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[2] == goalPoly) return true;

				// 辺と接しているポリゴンを次のチェック対象に追加
				for (num = 0; num < nextCheckPolyNum; num++)
				{
					// すでに追加されている場合追加しない
					if (nextCheckPoly[num] == m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[2]) break;
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
						if (checkPolyPrev[num] == m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[2]) break;
					}
					if (num == checkPolyPrevNum)
					{
						// チェック対象に追加
						nextCheckPoly[nextCheckPolyNum] = m_navMesh->GetPolyLinkInfo()[checkPoly[i]].linkPolyIndex[2];
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
			if (i >= 3) break;
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
bool CheckRoot::CheckPolyMoveWidth(Vector3 startPos, Vector3 goalPos, float width)
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
bool CheckRoot::SetPathPlan(Vector3 startPos, Vector3 goalPos, int* polyCount, bool* found)
{
	int num;
	int polyIndex;
	PathPlanUnit* pathUnit;
	PathPlanUnit* pathUnit_sub;
	PathPlanUnit* pathUnit_sub2;
	bool isGoal;
	// ゴールまでのポリゴン数
	int count = 0;

	// スタート位置とゴール位置を保存
	m_startPos = startPos;
	m_endPos = goalPos;

	// 経路探索用のメモリ確保
	m_unitArray = new PathPlanUnit[sizeof(PathPlanUnit) * m_navMesh->GetPolyList().PolygonNum];

	// 経路探索用のポリゴンの初期化
	for (num = 0; num < m_navMesh->GetPolyList().PolygonNum; num++)
	{
		m_unitArray[num].polyIndex = num;
		m_unitArray[num].totalDistance = 0;
		m_unitArray[num].prevPolyIndex = -1;
		m_unitArray[num].nextPolyIndex = -1;
		m_unitArray[num].activeNext = nullptr;
	}

	// スタート地点のポリゴン番号を取得 -> 経路探索用の構造体のアドレスを保存
	polyIndex = m_navMesh->CheckPolyIndex(startPos);
	if (polyIndex == -1) return false;

	m_start = &m_unitArray[polyIndex];
	if (!m_start) return false;

	// デバッグ用
	if (!m_start)
	{
		m_start = &m_unitArray[polyIndex];
	}

	// 経路探索用のポリゴンとしてスタート地点のポリゴンを登録
	m_activeFirst = &m_unitArray[polyIndex];

	// ゴール地点にあるポリゴン番号を取得 -> 経路探索用の構造体のアドレスを保存
	polyIndex = m_navMesh->CheckPolyIndex(goalPos);
	if (polyIndex == -1) return false;

	m_goal = &m_unitArray[polyIndex];
	if (!m_goal) return false;

	// スタート地点とゴール地点のポリゴンが同じとき
	if (m_start == m_goal) return false;

	// ゴールにたどり着くまで探索
	isGoal = false;
	while (!isGoal)
	{
		// 探索対象のポリゴンを処理
		pathUnit = m_activeFirst;
		m_activeFirst = nullptr;
		for (; pathUnit != nullptr; pathUnit = pathUnit->activeNext)
		{
			// ポリゴンの辺の数分繰り返す
			for (num = 0; num < 3; num++)
			{
				// 隣接するポリゴンがないとき
				if (m_navMesh->GetPolyLinkInfo()[pathUnit->polyIndex].linkPolyIndex[num] == -1) continue;

				// 隣接するポリゴンが経路探索処理が行われている && より距離の長い経路  もしくは　スタート地点のポリゴンの時
				pathUnit_sub = &m_unitArray[m_navMesh->GetPolyLinkInfo()[pathUnit->polyIndex].linkPolyIndex[num]];
				if ((pathUnit_sub->prevPolyIndex != -1 && pathUnit_sub->totalDistance <= pathUnit->totalDistance + m_navMesh->GetPolyLinkInfo()[pathUnit->polyIndex].linkPolyDistance[num]) ||
					pathUnit_sub->polyIndex == m_start->polyIndex)
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
				pathUnit_sub->totalDistance = pathUnit->totalDistance + m_navMesh->GetPolyLinkInfo()[pathUnit->polyIndex].linkPolyDistance[num];

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
		if (m_activeFirst == nullptr) return false;

		count++;
	}

	// ゴールからスタートまでたどって、経路上の次に移動するべきポリゴン番号を代入
	pathUnit = m_goal;
	do
	{
		pathUnit_sub = pathUnit;
		pathUnit = &m_unitArray[pathUnit_sub->prevPolyIndex];
		pathUnit->nextPolyIndex = pathUnit_sub->polyIndex;
	} while (pathUnit != m_start);

	if (count >= 15)
	{
		*polyCount = count / 2;
	}
	else if (count >= 2)
	{
		*polyCount = count;
	}
	else
	{
		*polyCount = 0;
	}

	// 探索終了フラグを立てる
	*found = true;

	// 探索終了
	return true;
}

// 経路探索情報を削除
void CheckRoot::RemovePathPlan()
{
	// 経路探索の為に確保したメモリ領域を解放
	delete[] m_unitArray;
	m_unitArray = nullptr;
}

// 探索経路の移動処理の初期化
void CheckRoot::MoveInitialize(const Vector3& pos)
{
	// 移動開始時に乗っているポリゴンをスタート地点に設定
	m_nowPolyIndex = m_start->polyIndex;

	// 移動開始時点の座標は現在の座標
	m_nowPos = pos;

	// 移動開始時の経路探索情報をスタート地点のポリゴン情報にする
	m_nowPathPlan = m_start;

	// 移動開始時の移動中間地点の経路探索情報もスタート地点のポリゴン
	m_targetPathPlan = m_start;

	// どれだけポリゴンを移動したかをカウントする用
	m_pastPoly = m_start;
}

// 探索経路の移動処理
Vector3 CheckRoot::Move(const Vector3& pos, const float speed, const float width, int* polyCount)
{
	// 移動方向の更新、ゴールにたどり着いていたら終了
	if (RefreshMoveDirection(speed, width, polyCount)) return pos;

	// 移動方向の座標に移動
	m_nowPos += m_moveDirection * speed;

	// 現在の座標で乗っているポリゴンを検索
	m_nowPolyIndex = m_navMesh->CheckPolyIndex(m_nowPos);

	// 乗っているポリゴンの経路探索情報をアドレスに代入
	m_nowPathPlan = &m_unitArray[m_nowPolyIndex];

	if (*polyCount != 0 && m_pastPoly != m_nowPathPlan)
	{
		// 位置の更新
		m_pastPoly = m_nowPathPlan;
		*polyCount = *polyCount - 1;
	}

	return m_nowPos;
}

// 探索経路の移動方向を更新（true:目標地点に到達, false:目標地点に未到達）
bool CheckRoot::RefreshMoveDirection(const float speed, const float width, int* polyCount)
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
			*polyCount = 0;
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
			// プレイヤーが落下中なので追わない
			if (m_targetPathPlan->nextPolyIndex == 0) break;

			tempPathUnit = &m_unitArray[m_targetPathPlan->nextPolyIndex];

			// 経路上の次のポリゴンの中心座標に直線移動できないとき、ループから抜ける
			if (!CheckPolyMoveWidth(m_nowPos, m_navMesh->GetPolyLinkInfo()[tempPathUnit->polyIndex].centerPos, width)) break;

			// チェック対象を経路上の更に１つ先のポリゴンに変更
			m_targetPathPlan = tempPathUnit;

			// ゴール地点のポリゴンの時ループを抜ける
			if (m_targetPathPlan == m_goal) break;
		}
	}

	// 移動方向を決定する（移動方向は現在の座標から中間地点のポリゴンの中心座標に向かう）
	m_moveDirection = m_navMesh->GetPolyLinkInfo()[m_targetPathPlan->polyIndex].centerPos - m_nowPos;
	m_moveDirection.y = 0;
	m_moveDirection = Math::Normalized(m_moveDirection);

	// ゴールにたどり着いていない
	return false;
}

// 現在のポリゴンにプレイヤーがいるかどうか
bool CheckRoot::CheckPlayerPoly(const Vector3& enemyPos, const Vector3& playerPos)
{
	if (m_navMesh->CheckPolyIndex(enemyPos) == m_navMesh->CheckPolyIndex(playerPos))
	{
		return true;
	}

	return false;
}