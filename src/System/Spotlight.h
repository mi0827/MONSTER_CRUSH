#pragma once

//! @file Spotlight.h
//! @brief スポットライト用のクラス
class Spotlight
{
public:
	// 9月13日のげーぷろにスポットライトの使用が書いてある
	//! コンストラクタ
	Spotlight();
	//! デストラクタ
	~Spotlight();

	//! スポットライトの一番最初にする設定
	//! @param
	void InitSetLight(Vector3 pos, Vector3 spot_pos, float outAngle, float inangle, float range, float atten0, float atten1, float atten2);

	//! ライトの座標をセットする
	void SetLightPos(Vector3* pos);

	//! ライトが照らす座標をセットする
	void SetLightSpotPos(Vector3* spot_pos);

	//! スポットライトの座標の更新処理
	void UpdateTransform(Vector3* pos, Vector3* spot_pos);

private:
	//! ライトハンドル
	int m_light_handle = 0;
public:
	//! キャラのことを後ろから照らすスポットライト

	float out_angle = 1.0f;
	float in_angle = 1.0f;
	float range = 1.0f;
	float atten0 = 1.0f;
	float atten1 = 1.0f;
	float atten2 = 1.0f;



};