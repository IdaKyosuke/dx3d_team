#pragma once
#include <list>

class Actor3D;

// Actor3D同士の衝突判定クラス
class ActorCollision3D
{
private:
	std::list<Actor3D*> m_actorList;	// 衝突判定をするActorのリスト

	// コンストラクタ
	ActorCollision3D() {}

public:
	// シングルトン
	static ActorCollision3D* GetInstance()
	{
		static ActorCollision3D instance;
		return &instance;
	}

	// 登録
	void Register(Actor3D* actor)
	{
		m_actorList.push_back(actor);
	}

	// 削除
	void Remove(Actor3D* actor)
	{
		m_actorList.remove(actor);
	}

	// 更新
	void Update();

#ifdef _DEBUG
	// 描画
	void Draw() const;
#endif
};
