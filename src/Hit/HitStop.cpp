#include "src/WinMain.h"
#include "HitStop.h"

// --------------------------------------------------
// ヒットストップしてほしいか動いてほしいかを返す関数
// --------------------------------------------------
bool HitStop::Hit_Stop()
{
	m_stop_frame_count--; // カウントを減らす

	// ストップカウントが一定の値以上あると
	if (m_stop_frame_count > 0)
	{
		return  true; // ヒットストップしてほしい
	}
	else
	{
		m_stop_frame_count = 0;
	    return false; // してほしくない
	}

}

// --------------------------------------------------
// stop_frame_countをリセットするための関数
// --------------------------------------------------
void HitStop::Stop_Count_Reset()
{
	// カウントのリセット
	m_stop_frame_count = m_hit_stop_count_max;
}
