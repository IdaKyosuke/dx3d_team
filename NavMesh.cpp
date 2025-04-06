#include"NavMesh.h"
#include"Collision3D.h"

NavMesh::NavMesh(Collision3D* collision3D) :
	m_collision3D(collision3D)
{

}

// 指定座標の直下、直上のポリゴン番号を取得（ない場合は-1）
int NavMesh::CheckPolyIndex(Vector3 pos)
{
	return m_collision3D->CheckPolyIndex(pos);
}

// ポリゴン同士の連結情報を作成
void NavMesh::SetPolyLinkInfo()
{
	// ステージのメッシュ情報を取得
	m_polyList = m_collision3D->GetStageMesh();

	// メモリ領域を確保（mallocはメモリブロックを割り当てる）
	m_polyLinkInfo = (PolyLinkInfo*)malloc(sizeof(PolyLinkInfo) * m_polyList.PolygonNum);

	// 全ポリゴンの中心座標を算出
	PolyLinkInfo* polyLinkInfo = m_polyLinkInfo;
	MV1_REF_POLYGON* refPoly = m_polyList.Polygons;

	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++, polyLinkInfo++)
	{
		polyLinkInfo->centerPos = VScale(
			VAdd(m_polyList.Vertexs[refPoly->VIndex[0]].Position,
			VAdd(m_polyList.Vertexs[refPoly->VIndex[1]].Position, 
				m_polyList.Vertexs[refPoly->VIndex[2]].Position)),
			1.0f / 3.0f
		);
	}

	// ポリゴン同士の隣接情報を作成
	polyLinkInfo = m_polyLinkInfo;
	refPoly = m_polyList.Polygons;
	PolyLinkInfo* polyLinkInfoSub;
	MV1_REF_POLYGON* refPolySub;

	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++, polyLinkInfo++)
	{
		// 最初に隣接情報をリセットする
		for (int j = 0; j < 3; j++)
		{
			polyLinkInfo->linkPolyIndex[i] = -1;
		}

		// 隣接するポリゴンを探すためにポリゴンの数だけ繰り返す
		polyLinkInfoSub = m_polyLinkInfo;
		refPolySub = m_polyList.Polygons;

		for (int j = 0; j < m_polyList.PolygonNum; j++, refPolySub++, polyLinkInfoSub++)
		{
			// 自分自身を無視
			if (i == j) continue;

			// ポリゴン頂点番号(0,1)で形成される辺と隣接していたら隣接情報に追加
			if (
				polyLinkInfo->linkPolyIndex[0] == -1 &&
				(refPoly->VIndex[0] == refPolySub->VIndex[0] && refPoly->VIndex[1] == refPolySub->VIndex[2]) ||
				(refPoly->VIndex[0] == refPolySub->VIndex[1] && refPoly->VIndex[1] == refPolySub->VIndex[0]) ||
				(refPoly->VIndex[0] == refPolySub->VIndex[2] && refPoly->VIndex[1] == refPolySub->VIndex[1])
			)
			{
				polyLinkInfo->linkPolyIndex[0] = j;
				polyLinkInfo->linkPolyDistance[0] = VSize(VSub(polyLinkInfoSub->centerPos, polyLinkInfo->centerPos));
			}
			// ポリゴン頂点番号(1,2)で形成される辺と隣接していたら隣接情報に追加
			else if(
				polyLinkInfo->linkPolyIndex[0] == -1 &&
				(refPoly->VIndex[1] == refPolySub->VIndex[0] && refPoly->VIndex[2] == refPolySub->VIndex[2]) ||
				(refPoly->VIndex[1] == refPolySub->VIndex[1] && refPoly->VIndex[2] == refPolySub->VIndex[0]) ||
				(refPoly->VIndex[1] == refPolySub->VIndex[2] && refPoly->VIndex[2] == refPolySub->VIndex[1])
				)
			{
				polyLinkInfo->linkPolyIndex[1] = j;
				polyLinkInfo->linkPolyDistance[1] = VSize(VSub(polyLinkInfoSub->centerPos, polyLinkInfo->centerPos));
			}
			// ポリゴン頂点番号(2,0)で形成される辺と隣接していたら隣接情報に追加
			else if (
				polyLinkInfo->linkPolyIndex[0] == -1 &&
				(refPoly->VIndex[2] == refPolySub->VIndex[0] && refPoly->VIndex[0] == refPolySub->VIndex[2]) ||
				(refPoly->VIndex[2] == refPolySub->VIndex[1] && refPoly->VIndex[0] == refPolySub->VIndex[0]) ||
				(refPoly->VIndex[2] == refPolySub->VIndex[2] && refPoly->VIndex[0] == refPolySub->VIndex[1])
				)
			{
				polyLinkInfo->linkPolyIndex[2] = j;
				polyLinkInfo->linkPolyDistance[2] = VSize(VSub(polyLinkInfoSub->centerPos, polyLinkInfo->centerPos));
			}
		}
	}
}

// ポリゴン同士の連結情報の削除
void NavMesh::RemovePolyLinkInfo()
{
	free(m_polyLinkInfo);
	m_polyLinkInfo = nullptr;
}

// 指定の２点間を直線的に移動できるか（連結情報を使用する）
bool NavMesh::CheckPolyMove(Vector3 startPos, Vector3 goalPos)
{
	int startPoly;
	int goalPoly;
	PolyLinkInfo* polyInfoStart;
	PolyLinkInfo* polyInfoGoal;
	//Vector3 startPos;
	//Vector3 targetPos;
	Vector3 polyPos[3];
	int checkPoly[3];
	int checkPolyPrev[3];
	int checkPolyNum;
	int checkPolyPrevNum;
	int nextCheckPoly[3];
	int nextCheckPolyPrev[3];
	int nextCheckPolyNum;
	int nextCheckPolyPrevNum;

	// 開始座標と目標座標のYを0にして平面上の判定を行う
	startPos.y = 0;
	goalPos.y = 0;

	// 開始座標と目標座標の真下のポリゴンを調べる
	startPoly = CheckPolyIndex(startPos);
	goalPoly = CheckPolyIndex(goalPos);

	// ポリゴンが存在しない場合
	if (startPoly == -1 || goalPoly == -1) return false;

	// 開始座標と目標座標の直上 or 直下のポリゴンを登録
	polyInfoStart = &m_polyLinkInfo[startPoly];
	polyInfoGoal = &m_polyLinkInfo[goalPoly];


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

		}
	}

	return false;
}

// 指定の２点間を経路探索
bool NavMesh::SetPathPlan(Vector3 startPos, Vector3 goalPos)
{
	return false;
}

// 経路探索情報を削除
void NavMesh::RemovePathPlan()
{

}

// 探索経路の移動処理の初期化
void NavMesh::MoveInitialize()
{

}

// 探索経路の移動処理
void NavMesh::Move()
{

}

// 探索経路の移動方向を更新（true:目標地点に到達, false:目標地点に未到達）
bool NavMesh::RefreshMoveDirection()
{
	return false;
}