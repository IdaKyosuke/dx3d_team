#pragma once
#include "SceneBase.h"

class Node;

class SceneMenu : public SceneBase
{
private:
	Node* m_rootNode;

public:
	SceneMenu() :
	m_rootNode(nullptr){}

	virtual void Initialize() override;		// ������
	virtual void Finalize() override;		// �I��
	virtual SceneBase* Update() override;	// �X�V
	virtual void Draw() override;			// �`��
};