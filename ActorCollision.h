#pragma once
#include <list>

class Actor;

// Actor同士の衝突判定クラス
class ActorCollision
{
private:
	std::list<Actor*> m_actorList;	// 衝突判定をするActorのリスト

	// コンストラクタ
	ActorCollision() {}

public:
	// シングルトン
	static ActorCollision* GetInstance()
	{
		static ActorCollision instance;
		return &instance;
	}

	// 登録
	void Register(Actor* actor)
	{
		m_actorList.push_back(actor);
	}

	// 削除
	void Remove(Actor* actor)
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
