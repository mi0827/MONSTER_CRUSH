#pragma once
//! @file HitStop.h
//! @brief ヒットストップ用のクラス
class HitStop
{
public:
	// ヒットストップしてほしいか動いてほしいかを返す関数
	// 返す値
	//  true：ヒットストップしてほしい
	// false：ヒットストップしてほしくない
	bool CheckHitStop();

	// stop_frame_countをリセットするための関数
	void StopCountReset();
public:
	static const int HIT_STOP_COUNT_MAX = 10; // ヒットストップで止まるフレーム数
	int m_stop_frame_count = 0; // 止まってるフレームをカウントを図るよう変数
};