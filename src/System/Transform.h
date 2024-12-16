#pragma once
//! @file Transform.h
//! @brief キャラの座標、回転、スケールをまとめたもの
class Transform
{
public:
	//! 座標
	Vector3 pos{ 0.0f,0.0f,0.0f };
	//! 向き
	Vector3 rot{ 0.0f,0.0f,0.0f };
	//! 大きさ
	Vector3 scale{ 0.1f,0.1f,0.1f };
	//! コンストラクタ
	Transform();
	//! デストラクタ
	~Transform();
};
