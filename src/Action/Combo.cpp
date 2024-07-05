#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "Combo.h"

Combo::Combo()
{
}

Combo::~Combo()
{
}

// コンボしてよいかを判断する関数
void Combo::Combo_Judgment(bool* combo_flag, int* mouse_flag, int key, float play_anim_time, float anim_total,int*combo_count)
{
	// 攻撃アニメーションが終わる前に
	if (play_anim_time <= anim_total)
	{
		if (*combo_flag == false)
		{
			// 指定のマウスボタンを押していたら
			if (PushMouseInput(key))
			{
				// コンボカウントを増やす
				*combo_count += 1;
				// どのボタンを押したかを保存
				*mouse_flag = key;
				// コンボフラグを上げる
				*combo_flag = true;
				// コンボカウントを増やす
			}
		}
	}
}
