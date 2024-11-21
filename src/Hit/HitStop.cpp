#include "src/WinMain.h"
#include "HitStop.h"

// --------------------------------------------------
// ヒットストップしてほしいか動いてほしいかを返す関数
// --------------------------------------------------
bool HitStop::CheckHitStop()
{
	// カウントのリセットはヒットストップが起こってほしい処理が行われた時におこなうようにする
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
void HitStop::StopCountReset()
{
	// カウントのリセット
	// リセットすることでヒットストップの判定ができる
	m_stop_frame_count = HIT_STOP_COUNT_MAX;
}
