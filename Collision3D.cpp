#include"Collision3D.h"
#include"LoadPlayer.h"

// ステージの当たり判定を作成
Collision3D::Collision3D(const char* modelPath, const Vector3& pos) :
	m_cap1(Vector3(0,0,0)),
	m_cap2(Vector3(0,0,0))
{
	// ステージの読み込み
	m_model = MV1LoadModel(modelPath);
	MV1SetPosition(m_model, Vector3(0,0,0));
	// モデル全体のコリジョン情報を構築
	MV1SetupCollInfo(m_model, -1, 8, 8, 8);

	MV1SetupReferenceMesh(m_model, -1, true);
	m_refPoly = MV1GetReferenceMesh(m_model, -1, true);

	// レイの設定
	m_checkMoveStart = Vector3(0, 0, 0);
	m_checkMoveEnd = Vector3(0, 0, 0); - DiffCheckLine;

	m_getHeightStart = Vector3(0, 0, 0) + DiffGetLine;
	m_getHeightEnd = Vector3(0, 0, 0);

	// 構造体の初期化
	m_stagePoly = MV1CollCheck_Line(m_model, -1, m_checkMoveStart, m_checkMoveEnd);
	m_polyHeight = MV1CollCheck_Line(m_model, -1, m_getHeightStart, m_getHeightEnd);
}

void Collision3D::Release()
{
	MV1DeleteModel(m_model);
}

// 指定した座標に足場があるかどうか（どのポリゴンでも関係なし）
int Collision3D::CheckStage(const Vector3& pos)
{
	m_checkMoveStart = pos;
	m_checkMoveEnd = pos - DiffCheckLine;

	// モデルと線分（プレイヤー）との当たり判定
	m_stagePoly = MV1CollCheck_Line(m_model, -1, m_checkMoveStart, m_checkMoveEnd);

	return m_stagePoly.HitFlag;
}

// 指定座標直下、直上のポリゴン番号を取得
int Collision3D::CheckPolyIndex(const Vector3& pos)
{
	HITRESULT_LINE hitRes;	// 伸ばした線分とポリゴンの当たり判定をとる
	// 指定の座標のY軸方向に大きく伸びる線分の２座標をセット
	Vector3 lineUp = Vector3(pos.x, 1000000.0f, pos.z);
	Vector3 lineDown = Vector3(pos.x, -1000000.0f, pos.z);

	// ポリゴンの数だけ繰り返す
	for (int i = 0; i < m_refPoly.PolygonNum; i++)
	{
		hitRes = HitCheck_Line_Triangle(
			lineUp, lineDown,
			m_refPoly.Vertexs[m_refPoly.Polygons->VIndex[0]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons->VIndex[1]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons->VIndex[2]].Position
		);
		// 当たり判定があったらポリゴン番号を返す
		if (hitRes.HitFlag)
		{
			return i;
		}
	}

	// 当たり判定がなかった
	return -1;
}

// 指定座標のレイ
MV1_COLL_RESULT_POLY Collision3D::GetHeight(const Vector3& pos, bool terning)
{
	if (!terning)
	{
		m_getHeightStart = pos + DiffGetLine;
		m_getHeightEnd = pos;
		m_getHeightEnd.y -= 1;
	}
	else
	{
		m_getHeightStart = pos - DiffGetLine;
		m_getHeightEnd = pos;
		m_getHeightEnd.y += 1;
	}
	m_polyHeight = MV1CollCheck_Line(m_model, -1, m_getHeightStart, m_getHeightEnd);

	return m_polyHeight;
}

// プレイヤーのカプセルとステージの当たり判定をとる(カプセルとモデルの当たったポリゴン数)
int Collision3D::CapsuleCollider(const Vector3& pos, bool terning)
{
	if (!terning)
	{
		m_cap1 = pos + Vector3(0, Radius + DiffCapsule, 0);			// 球の半径分 + 余剰分中心を上にあげる
		m_cap2 = pos + Vector3(0, CapsuleLength - Radius - DiffCapsule, 0);	// 球の距離分中心を上げて、半径分下げる
	}
	else
	{
		m_cap1 = pos - Vector3(0, Radius + DiffCapsule, 0);			// 球の半径分 + 余剰分中心を上にあげる
		m_cap2 = pos - Vector3(0, CapsuleLength - Radius - DiffCapsule, 0);	// 球の距離分中心を上げて、半径分下げる
	}

	return MV1CollCheck_Capsule(m_model, -1, m_cap1, m_cap2, Radius).HitNum;

}

void Collision3D::Draw()
{
	MV1DrawModel(m_model);

#ifdef _DEBUG
	DrawLine3D(m_getHeightStart, m_getHeightEnd, GetColor(255, 255, 0));
	
	// ポリゴンの数だけ繰り返し
	for (int i = 0; i < m_refPoly.PolygonNum; i++)
	{
		// ポリゴンを形成する三頂点を使用してワイヤーフレームを描画する
		DrawLine3D(
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[0]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[1]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[1]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[2]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[2]].Position,
			m_refPoly.Vertexs[m_refPoly.Polygons[i].VIndex[0]].Position,
			GetColor(255, 255, 0));
	}
	
	DrawCapsule3D(m_cap1, m_cap2, Radius, 8, GetColor(255, 255, 0), GetColor(255, 255, 0), FALSE);

	// 当たったかどうかを表示する
	DrawFormatString(0, 80, GetColor(255, 255, 255), "HIT:%d", m_polyHeight.HitFlag);
#endif // DEBUG
}
