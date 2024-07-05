#pragma once



//! キャラの座標、回転、スケールをまとめたもの
class Transform
{
public:
	Vector3 pos{ 0.0f,0.0f,0.0f };
	Vector3 rot{ 0.0f,0.0f,0.0f };
	Vector3 scale{ 0.1f,0.1f,0.1f };
	//! コンストラクタ
	Transform();
	//! デストラクタ
	~Transform();
};
