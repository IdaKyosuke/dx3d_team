#pragma once
#include"Node.h"

class Animation3D : public Node
{
private:
	static constexpr float m_durationTime = 0.4f;
	static constexpr int FrameRate = 60;

	int m_model;		// 動かすモデル
	int m_animId;		// ロードしたアニメーション
	int m_attachAnim;	// アニメーションがアタッチされたモデル
	float m_animTotalTime;	// アニメーションの総再生時間
	float m_blendStart;
	float m_blendEnd;

	float m_elapsedTime;
	float m_animTime;
	bool m_isActive;
	bool m_roopFlg;
	bool m_finishAnim;	// ループしないアニメーションが終了したか

protected:
	virtual void Update() override;

public:
	Animation3D(const int model, const char* anim, bool roopFlg = true);
	virtual ~Animation3D();

	// ブレンド用
	void FadeIn();
	void FadeOut();

	// 瞬時に切り替えたい時葉
	void ChangeOut();
	void ChangeIn();

	// ループしないアニメーションの終了フラグ
	bool FinishAnim(){ return m_finishAnim; }
};