#pragma once

class HitStop
{
public:
	// ヒットストップしてほしいか動いてほしいかを返す関数
	// 返す値
	//  true：ヒットストップしてほしい
	// false：ヒットストップしてほしくない
	bool Hit_Stop();

	// stop_frame_countをリセットするための関数
	void Stop_Count_Reset();
public:
	static const int HIT_STOP_COUNT_MAX = 5; // ヒットストップで止まるフレーム数
	int m_stop_frame_count = 0; // 止まってるフレームをカウントを図るよう変数
};