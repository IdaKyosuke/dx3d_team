#pragma once
#pragma once
#include "Node.h"
#include "Transform3D.h"
#include <string>

class Collider3D;

class Actor3D : public Node
{
protected:
	std::string m_name;		// オブジェクト名
	Transform3D m_transform;	// 姿勢情報
	Collider3D* m_collider;	// 衝突判定の形状

	int m_itemNum;

	virtual void Load() override;		// リソースの読み込み
	virtual void Release() override;	// リソースの解放
	virtual void Draw() override;		// 描画

public:
	// コンストラクタ
	Actor3D(
		const char* name,
		const Vector3& position = Vector3(),
		int itemNum = 0
	);

	// 子ノードを含む更新
	virtual void TreeUpdate() override;

	// 姿勢の取得
	const Transform3D& GetTransform() const
	{
		return m_transform;
	}

	// 座標の取得
	const Vector3& GetPosition() const
	{
		return m_transform.position;
	}

	// 名前の取得
	const std::string& GetName() const
	{
		return m_name;
	}

	// 衝突判定の形状を取得
	const Collider3D* GetCollider() const
	{
		return m_collider;
	}

	int GetItemNum() const
	{
		return m_itemNum;
	}

	// 衝突イベント
	virtual void OnCollision(const Actor3D* other) {}
};
