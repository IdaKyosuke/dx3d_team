#pragma once

// 時間管理クラス
class Time
{
private:
	int m_prevTime;		// 前フレームの時刻
	float m_deltaTime;	// 前フレームからの経過時間

	Time();	// コンストラクタ

public:
	// シングルトン
	static Time* GetInstance()
	{
		static Time instance;
		return &instance;
	}

	void Update();	// 更新
	
	// 前フレームからの経過時間を取得
	float GetDeltaTime()
	{
		return m_deltaTime;
	}
};
