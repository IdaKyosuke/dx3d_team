#include"ModelLoader.h"
#include<string>

// モデルの読み込み
int ModelLoader::LoadModel(const char* model)
{
	std::string fullPath;
	fullPath += "Resource/";
	fullPath += model;
	return MV1LoadModel(fullPath.c_str());
}

// アニメーションの読み込み
int ModelLoader::LoadAnim(const char* anim)
{
	return MV1LoadModel(anim);
}

// アニメーションのアタッチ
int ModelLoader::AttachAnim(const int& model, const int& animId)
{
	return MV1AttachAnim(model, 0, animId);
}
