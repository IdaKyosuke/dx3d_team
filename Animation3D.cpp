#include"Animation3D.h"
#include"Lerp.h"
#include"Time.h"
#include"DxLib.h"
#include<string>

Animation3D::Animation3D(const int model, const char* anim, bool roopFlg) :
	m_model(model),
	m_animTotalTime(0),
	m_blendStart(-1.0f),
	m_blendEnd(0),
	m_elapsedTime(0.0f),
	m_animTime(0),
	m_isActive(false),
	m_roopFlg(roopFlg),
	m_finishAnim(false),
	m_isStop(false)
{
	std::string path;
	path += "Resource/";
	path += anim;
	m_animId = MV1LoadModel(path.c_str());
	m_attachAnim = (MV1AttachAnim(model, 0, m_animId));

	m_animTotalTime = MV1GetAttachAnimTotalTime(m_model, m_attachAnim);

	// 現在再生中のアニメーションを止める
	MV1SetAttachAnimBlendRate(m_model, m_attachAnim, 0);
}

Animation3D::~Animation3D()
{
	MV1DeleteModel(m_animId);
}

void Animation3D::Update()
{
	if (!m_isActive) return;

	if (m_isStop) return;

	// アニメーション時間の加算
	m_animTime += FrameRate * Time::GetInstance()->GetDeltaTime();
	
	if (m_animTime >= m_animTotalTime)
	{
		if (m_roopFlg)
		{
			// アニメーションのループ処理
			// 再生時間を超えたら時間をリセット
			m_animTime = 0;
		}
		else
		{
			// ループしないアニメーションの終了フラグを立てる
			m_finishAnim = true;
		}
	}
	
	// アニメーション時間
	MV1SetAttachAnimTime(m_model, m_attachAnim, m_animTime);

	if (m_blendStart != -1.0f)
	{
		m_elapsedTime += Time::GetInstance()->GetDeltaTime();
		float t = m_elapsedTime / m_durationTime;

		if (t >= 1.0f)
		{
			t = 1.0f;
			m_blendStart = -1.0f;
			m_elapsedTime = 0;
			if (m_blendEnd == 0.0f)
			{
				m_isActive = false;
			}
		}
		float rate = Lerp::Exec(m_blendStart, m_blendEnd, t);

		MV1SetAttachAnimBlendRate(m_model, m_attachAnim, rate);
	}
}

void Animation3D::FadeIn()
{
	m_blendStart = 0.0f;
	m_blendEnd = 1.0f;
	m_animTime = 0;
	m_isActive = true;
	// アニメーション終了フラグのリセット
	if (m_finishAnim) m_finishAnim = false;
}

void Animation3D::FadeOut()
{
	m_blendStart = 1.0f;
	m_blendEnd = 0.0f;
}

void Animation3D::ChangeIn()
{
	MV1SetAttachAnimBlendRate(m_model, m_attachAnim, 1);
	m_animTime = 0;
	m_isActive = true;
}

void Animation3D::ChangeOut()
{
	MV1SetAttachAnimBlendRate(m_model, m_attachAnim, 0);
	m_isActive = false;
}