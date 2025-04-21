#include"NavMesh.h"
#include"CollisionStage.h"
#include"Math.h"
#include"Vector3.h"
#include <cstdlib>
#include <ctime>

NavMesh::NavMesh(CollisionStage* collisionStage) :
	m_collisionStage(collisionStage)
{
	srand(static_cast<unsigned int>(time(NULL)));
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
	PolyLinkInfo* plInfo;
	PolyLinkInfo* plInfoSub;
	MV1_REF_POLYGON* refPoly;
	MV1_REF_POLYGON* refPolySub;

	int model = m_collisionStage->GetMeshModel();

	m_polyList = MV1GetReferenceMesh(model, -1, true);

	// メモリ領域を確保
	m_polyLink = new PolyLinkInfo[sizeof(PolyLinkInfo) * m_polyList.PolygonNum];

	plInfo = m_polyLink;
	refPoly = m_polyList.Polygons;
	// 全ポリゴンの中心座標を算出
	for (int i = 0; i < m_polyList.PolygonNum; i++, refPoly++, plInfo++)
	{
		plInfo->centerPos = VScale(
			VAdd(m_polyList.Vertexs[refPoly->VIndex[0]].Position,
				VAdd(m_polyList.Vertexs[refPoly->VIndex[1]].Position,
					m_polyList.Vertexs[refPoly->VIndex[2]].Position)),
			1.0f / 3.0f
		);
	}

	// ポリゴン同士の隣接情報を作成
	plInfo = m_polyLink;
	refPoly = m_polyList.Polygons;

	for (int i = 0; i < m_polyList.PolygonNum; i++, plInfo++, refPoly++)
	{
		// 最初に隣接情報をリセットする
		for (int j = 0; j < 3; j++)
		{
			plInfo->linkPolyIndex[j] = -1;
		}

		// 隣接するポリゴンを探すためにポリゴンの数だけ繰り返す
		refPolySub = m_polyList.Polygons;
		plInfoSub = m_polyLink;

		// 隣接情報を追加しようとしているポリゴンの頂点座標
		Vector3 vertexPos[3] = { Vector3(0, 0, 0) };
		for (int j = 0; j < 3; j++)
		{
			// 対応したインデックスの要素に座標をコピー
			vertexPos[j] = m_polyList.Vertexs[refPoly->VIndex[j]].Position;
		}

		for (int j = 0; j < m_polyList.PolygonNum; j++, refPolySub++, plInfoSub++)
		{
			// 自分自身を無視
			if (i == j) continue;

			Vector3 subVertexPos[3] = { Vector3(0, 0, 0) };

			// この周で隣り合っているか調べるポリゴンの頂点座標
			for (int k = 0; k < 3; k++)
			{
				subVertexPos[k] = m_polyList.Vertexs[refPolySub->VIndex[k]].Position;
			}

			// 法線からポリゴンの向きを計算
			Vector3 v1 = subVertexPos[1] - subVertexPos[0];
			Vector3 v2 = subVertexPos[2] - subVertexPos[0];

			// 外積
			Vector3 outv = v1.CrossP(v2);
			outv = Math::Normalized(outv);

			// 角度がついているポリゴンは床とみなさない
			if (outv.y <= 0.5)
			{
				continue;
			}

			// ポリゴン頂点番号(0,1)で形成される辺と隣接していたら隣接情報に追加
			if (
				plInfo->linkPolyIndex[0] == -1 &&
				((vertexPos[0] == subVertexPos[0] && vertexPos[1] == subVertexPos[2]) ||
					(vertexPos[0] == subVertexPos[1] && vertexPos[1] == subVertexPos[0]) ||
					(vertexPos[0] == subVertexPos[2] && vertexPos[1] == subVertexPos[1]))
				)
			{
				plInfo->linkPolyIndex[0] = j;
				plInfo->linkPolyDistance[0] = VSize(VSub(plInfoSub->centerPos, plInfo->centerPos));
			}
			// ポリゴン頂点番号(1,2)で形成される辺と隣接していたら隣接情報に追加
			else if (
				plInfo->linkPolyIndex[1] == -1 &&
				((vertexPos[1] == subVertexPos[0] && vertexPos[2] == subVertexPos[2]) ||
					(vertexPos[1] == subVertexPos[1] && vertexPos[2] == subVertexPos[0]) ||
					(vertexPos[1] == subVertexPos[2] && vertexPos[2] == subVertexPos[1]))
				)
			{
				plInfo->linkPolyIndex[1] = j;
				plInfo->linkPolyDistance[1] = VSize(VSub(plInfoSub->centerPos, plInfo->centerPos));
			}
			// ポリゴン頂点番号(2,0)で形成される辺と隣接していたら隣接情報に追加
			else if (
				plInfo->linkPolyIndex[2] == -1 &&
				((vertexPos[2] == subVertexPos[0] && vertexPos[0] == subVertexPos[2]) ||
					(vertexPos[2] == subVertexPos[1] && vertexPos[0] == subVertexPos[0]) ||
					(vertexPos[2] == subVertexPos[2] && vertexPos[0] == subVertexPos[1]))
				)
			{
				plInfo->linkPolyIndex[2] = j;
				plInfo->linkPolyDistance[2] = VSize(VSub(plInfoSub->centerPos, plInfo->centerPos));
			}
		}
	}
}

// ポリゴン同士の連結情報の削除
void NavMesh::RemovePolyLinkInfo()
{
	delete[] m_polyLink;
}

// ポリゴンの情報を使って、ポジションを返す
Vector3 NavMesh::GetPos(const int polygonNum)
{
	if (polygonNum == 0)
	{
		int index;
		Vector3 outv;

		// 抽選の場合、法線からポリゴンの角度を取得 => 歩けない場所の時は再抽選
		do
		{
			index = rand() % m_polyList.PolygonNum;

			Vector3 subVertexPos[3] = { Vector3(0, 0, 0) };

			// この周で隣り合っているか調べるポリゴンの頂点座標
			for (int k = 0; k < 3; k++)
			{
				subVertexPos[k] = m_polyList.Vertexs[m_polyList.Polygons[index].VIndex[k]].Position;
			}

			// 法線からポリゴンの向きを計算
			Vector3 v1 = subVertexPos[1] - subVertexPos[0];
			Vector3 v2 = subVertexPos[2] - subVertexPos[0];

			// 外積
			outv = v1.CrossP(v2);
			outv = Math::Normalized(outv);
		} while (outv.y <= 0.5);
		
		return m_polyLink[index].centerPos;
	}
	else
	{
		return m_polyLink[polygonNum].centerPos;
	}
}