#pragma once

struct Animation
{
public:
	const char* textureName;	// 画像のパス
	int textureId;				// 描画する画像
	int gridAmount;				// アニメーションのコマ数
	float sampleRate;				// 1秒間にアニメーションのコマを切り替える回数
	bool loopFlag;				// アニメーションがループするかどうか

	// コンストラクタ
	Animation(
		const char* textureName,
		int gridAmount = 1,
		float sampleRate = 10,
		bool loopFlag = true
	) :
		textureName(textureName),
		textureId(0),
		gridAmount(gridAmount),
		sampleRate(sampleRate),
		loopFlag(loopFlag) {}
};
