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
				VAdd(m_polyList.Vertexs[refPoly->VIndex[1]].Position, m_polyList.Vertexs[refPoly->VIndex[2]].Position)),
			1.0f / 3.0f
		);
	}

	// ポリゴン同士の隣接情報を作成
	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++, polyLinkInfo++)
	{
		// 最初に隣接情報をリセットする
		for (int i = 0; i < 3; i++)
		{
			polyLinkInfo->linkPolyIndex[i] = -1;
		}

		// 隣接するポリゴンを探すためにポリゴンの数だけ繰り返す

	}

}

// ポリゴン同士の連結情報の削除
void NavMesh::RemovePolyLinkInfo()
{

}

// 指定の２点間を直線的に移動できるか
bool NavMesh::CheckPolyMove(Vector3 startPos, Vector3 goalPos)
{
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