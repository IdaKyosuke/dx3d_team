#include"CollisionStage.h"
#include"LoadPlayer.h"

// ステージの当たり判定を作成
CollisionStage::CollisionStage(const char* floorModelPath, const char* wallModelPath, const Vector3& pos) :
	m_cap1(Vector3(0, 0, 0))
{
	// ステージの読み込み
	// 床
	m_modelFloor = MV1LoadModel(floorModelPath);
	// 壁
	m_modelWall = MV1LoadModel(wallModelPath);

	// 座標を決定
	MV1SetPosition(m_modelFloor, Vector3(0, 0, 0));
	MV1SetPosition(m_modelWall, Vector3(0, 0, 0));

	// モデル全体のコリジョン情報を構築
	// 床
	MV1SetupCollInfo(m_modelFloor, -1, 8, 8, 8);
	MV1SetupReferenceMesh(m_modelFloor, -1, true);
	// 壁
	MV1SetupCollInfo(m_modelWall, -1, 8, 8, 8);
	MV1SetupReferenceMesh(m_modelWall, -1, true);

#ifdef _DEBUG
	m_refPolyFloor = MV1GetReferenceMesh(m_modelFloor, -1, true);
	m_refPolyWall = MV1GetReferenceMesh(m_modelWall, -1, true);
#endif // _DEBUG

	// レイの設定
	m_checkMoveStart = Vector3(0, 0, 0);
	m_checkMoveEnd = Vector3(0, 0, 0) - CheckLineLength;

	m_getHeightStart = Vector3(0, 0, 0) + DiffGetLine;
	m_getHeightEnd = Vector3(0, 0, 0);

	// 構造体の初期化
	m_stagePoly = MV1CollCheck_Line(m_modelFloor, -1, m_checkMoveStart, m_checkMoveEnd);
	m_polyHeight = MV1CollCheck_Line(m_modelFloor, -1, m_getHeightStart, m_getHeightEnd);
}

void CollisionStage::Release()
{
	MV1DeleteModel(m_modelFloor);
	MV1DeleteModel(m_modelWall);
}

// 移動予定先に足場があるかどうか
int CollisionStage::CheckStage(const Vector3& pos)
{
	m_checkMoveStart = pos + DeffRay;
	m_checkMoveEnd = pos - CheckLineLength;

	// モデルと線分（プレイヤー）との当たり判定
	m_stagePoly = MV1CollCheck_Line(m_modelFloor, -1, m_checkMoveStart, m_checkMoveEnd);

	return m_stagePoly.HitFlag;
}

// プレイヤーの足元までのレイ
MV1_COLL_RESULT_POLY CollisionStage::GetHeight(const Vector3& pos)
{
	m_getHeightStart = pos + DiffGetLine;
	m_getHeightEnd = pos;
	m_getHeightEnd.y -= 1;

	m_polyHeight = MV1CollCheck_Line(m_modelFloor, -1, m_getHeightStart, m_getHeightEnd);

	return m_polyHeight;
}

// カメラが埋まらないよう調整
MV1_COLL_RESULT_POLY_DIM CollisionStage::CheckCamHitWall(Vector3 camPos, Vector3 camDir)
{
	// 当たり判定の結果を削除
	MV1CollResultPolyDimTerminate(m_camFrontPoly);

	Vector3 camTargetPos = camPos + (camDir * 2.0f);

	m_camFrontPoly = MV1CollCheck_LineDim(m_modelWall, -1, camPos, camTargetPos);

	if (m_camFrontPoly.HitNum > 0)
	{
		int i = 0;
	}

	return m_camFrontPoly;
}

// プレイヤーのカプセルとステージの当たり判定をとる(カプセルとモデルの当たったポリゴン数)
int CollisionStage::CapsuleCollider(const Vector3& pos)
{
	// 当たり判定の結果を削除
	MV1CollResultPolyDimTerminate(m_spherePoly);

	// 壁との当たり判定を球で取る（ガクつくので修正予定）
	m_cap1 = pos;
	m_spherePoly = MV1CollCheck_Sphere(m_modelWall, -1, Vector3(pos.x, pos.y + 100, pos.z), Radius);

	return m_spherePoly.HitNum;
}

// 指定の座標の直下、若しくは直上にあるポリゴンの番号を取得する
int CollisionStage::CheckOnPolyIndex(const Vector3& pos, const MV1_REF_POLYGONLIST& polyList)
{
	Vector3 linePos1;
	Vector3 linePos2;
	HITRESULT_LINE hitRes;
	MV1_REF_POLYGON* refPoly;

	// 指定座標のY軸方向に大きく伸びる線分をセット
	linePos1 = Vector3(pos.x, 1400.0f, pos.z);
	linePos2 = Vector3(pos.x, -1400.0f, pos.z);

	// ステージモデルのポリゴン数だけ繰り返す
	refPoly = polyList.Polygons;
	for (int i = 0; i < polyList.PolygonNum; i++, refPoly++)
	{
		// 線分と接するポリゴンを返す
		hitRes = HitCheck_Line_Triangle(
			linePos1,
			linePos2,
			polyList.Vertexs[refPoly->VIndex[0]].Position,
			polyList.Vertexs[refPoly->VIndex[1]].Position,
			polyList.Vertexs[refPoly->VIndex[2]].Position
		);

		if (hitRes.HitFlag) return i;
	}

	// ポリゴンが無い場合
	return -1;
}

void CollisionStage::Draw()
{

#ifdef _DEBUG
	//MV1DrawModel(m_model);
	// ポリゴンの数だけ繰り返し
	for (int i = 0; i < m_refPolyFloor.PolygonNum; i++)
	{
		// ポリゴンを形成する三頂点を使用してワイヤーフレームを描画する
		DrawLine3D(
			m_refPolyFloor.Vertexs[m_refPolyFloor.Polygons[i].VIndex[0]].Position,
			m_refPolyFloor.Vertexs[m_refPolyFloor.Polygons[i].VIndex[1]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPolyFloor.Vertexs[m_refPolyFloor.Polygons[i].VIndex[1]].Position,
			m_refPolyFloor.Vertexs[m_refPolyFloor.Polygons[i].VIndex[2]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPolyFloor.Vertexs[m_refPolyFloor.Polygons[i].VIndex[2]].Position,
			m_refPolyFloor.Vertexs[m_refPolyFloor.Polygons[i].VIndex[0]].Position,
			GetColor(255, 255, 0));
	}

	// ポリゴンの数だけ繰り返し
	for (int i = 0; i < m_refPolyWall.PolygonNum; i++)
	{
		// ポリゴンを形成する三頂点を使用してワイヤーフレームを描画する
		DrawLine3D(
			m_refPolyWall.Vertexs[m_refPolyWall.Polygons[i].VIndex[0]].Position,
			m_refPolyWall.Vertexs[m_refPolyWall.Polygons[i].VIndex[1]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPolyWall.Vertexs[m_refPolyWall.Polygons[i].VIndex[1]].Position,
			m_refPolyWall.Vertexs[m_refPolyWall.Polygons[i].VIndex[2]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPolyWall.Vertexs[m_refPolyWall.Polygons[i].VIndex[2]].Position,
			m_refPolyWall.Vertexs[m_refPolyWall.Polygons[i].VIndex[0]].Position,
			GetColor(255, 255, 0));
	}

	//DrawCapsule3D(m_cap1, m_cap2, Radius, 8, GetColor(255, 255, 0), GetColor(255, 255, 0), FALSE);

	Vector3 pos = m_cap1;
	DrawSphere3D(Vector3(pos.x, pos.y + 100, pos.z), Radius, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
#endif // DEBUG
}
