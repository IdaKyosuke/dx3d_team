#pragma once
#include "SceneBase.h"

class Node;
class Chest;

// �^�C�g���V�[��
class SceneTitle : public SceneBase
{
private:
	Node* m_rootNode;
	int m_bgm;

	Chest* m_chest;
public:
	// �R���X�g���N�^
	SceneTitle() :
		m_rootNode(nullptr),
		m_bgm(0),
		m_chest(nullptr){}

	virtual void Initialize() override;		// ������
	virtual void Finalize() override;		// �I��
	virtual SceneBase* Update() override;	// �X�V
	virtual void Draw() override;			// �`��
};
