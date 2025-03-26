#pragma once
#include"Vector3.h"
#include"DxLib.h"

class ModelLoader
{
private:

public:
	static int LoadModel(const char* model);	// モデルの読み込み
	static int LoadAnim(const char* anim);		// アニメーションの読み込み
	static int AttachAnim(const int& model, const int& animId);	// アニメーションのアタッチ
};