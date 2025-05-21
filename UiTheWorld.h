#pragma once
#include "Actor.h"
#include "Transform.h"
#include "Sprite.h"
#include "Vector2.h"


class UiTheWorld : public Actor
{
private:
	static constexpr Vector2 GridSize = Vector2(80, 80);	// �A�j���[�V�����̃T�C�Y
	static constexpr int GridNum = 5;	// �A�j���R�}��
	static constexpr Vector2 Offset = Vector2(50, 25);

	int m_imageId;
	Transform m_transform;


	const char* AnimeName[1] =
	{
		"TheWorld"
	};

	const Animation AnimeData[1] =
	{
		Animation("theworld_icon.png",5, 9)
	};

protected:
	virtual void Load() override;	// ���\�[�X�̓ǂݍ���
	virtual void Release() override;// ���\�[�X�̔j��
	virtual void Update() override;	// �X�V
	virtual void Draw() override;	// �`��

public:
	UiTheWorld();
};
