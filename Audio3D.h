#pragma once
#include"Actor3D.h"

class Enemy;

class Audio3D : public Actor3D
{
private:
	static constexpr float SoundRange = 2000.0f;		// 音の聞こえる範囲
	static constexpr int MinPlayCoolTime = 5;		// 音を再生するまでの時間の最小値
	static constexpr int RangePlayCoolTime = 10;		// 音を再生するまでの時間の範囲

	const char* m_audioHundle;

	int m_sound;	// 音源ハンドル
	Enemy* m_enemy;	// 自身の親（座標取得用）

	float m_durationTime;	// 現在の経過時間
	float m_playCoolTime;	// 再生までの時間
	bool m_isSetCoolTime;	// 音源を再生するまでのクールタイム

protected:
	virtual void Update() override;
	virtual void Load() override;
	virtual void Release() override;

public:
	Audio3D(const char* audioHundle, Enemy* enemy);

};