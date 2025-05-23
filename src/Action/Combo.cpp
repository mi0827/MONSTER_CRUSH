#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "Combo.h"

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
Combo::Combo()
{
}

//---------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------
Combo::~Combo()
{
}

//---------------------------------------------------------------------------
// コンボしてよいかを判断する関数
// 指定のキー（ボタンを押されたとき）
//---------------------------------------------------------------------------

void Combo::ComboJudgmentKey(bool* combo_flag, int* mouse_flag, int key, PAD_ID pad_button_no, float play_anim_time, float anim_total, int* combo_count)
{
	// コンボフラグが下がっていたら
	if (*combo_flag == false)
	{
		// 指定のマウスボタンを押していたら
		if (PushMouseInput(key)|| IsPadOn(pad_button_no))
		{
			// 攻撃アニメーションが終わる前だったら
			if (play_anim_time <= anim_total)
			{
				// コンボカウントを増やす
				*combo_count += 1;
				// どのボタンを押したかを保存
				*mouse_flag = key;
				// コンボフラグを上げる
				*combo_flag = true;

				ChangeComboMode(COMBO_STANDBY);
			}
			
		}
	}
}

//---------------------------------------------------------------------------
// コンボしてよいかを判断する関数
// 指定の条件を満たしているとき
//---------------------------------------------------------------------------
void Combo::ComboJudgmentCondition(bool* combo_flag, bool condition, float play_anim_time, float anim_total)
{
	// 攻撃アニメーションが終わる時に
	if (play_anim_time == anim_total - 5.0f)
	{
		if (*combo_flag == false)
		{
			// 指定の条件がtreuの時
			if (condition)
			{
				// コンボフラグを上げる
				*combo_flag = true;
				// コンボカウントを増やす
			}
		}
	}
}

//---------------------------------------------------------------------------
// コンボモードの切り替え用関数
//---------------------------------------------------------------------------
void Combo::ChangeComboMode(int combo_mode)
{
	// コンボモードの変更
	m_combo_mode = combo_mode;
}

//---------------------------------------------------------------------------
// コンボを始めていいかを返す用の関数
//---------------------------------------------------------------------------
void Combo::ComboStartJudgent()
{
	if (m_combo_mode == COMBO_FINISH)
	{
		m_combo_mode = COMBO_START;
	}
	
}


//---------------------------------------------------------------------------
// コンボの更新を続けていいかを返す関数
//---------------------------------------------------------------------------
bool Combo::ComboCanContinueJudgent()
{
	// コンボフィニッシュ以外ならコンボを続けていい
	if (m_combo_mode == COMBO_FINISH)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}


