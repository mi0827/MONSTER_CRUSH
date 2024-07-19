#pragma once

//! @fule Combo.h
//! @brief コンボ用のクラス
class Combo
{
public:
	//! @brief コンストラクタ
	Combo();
	//! @brief デストラクタ
	~Combo();

	//! @brief コンボしてよいのかを指定のキー操作で判断する関数
	//! @param コンボをしてよいかのフラグ
	//! @param マウスのどちらを押したかを入れる
	//! @param ある一定のキー（マウスのボタンでも可）
	//! @param コンボしたいときのアニメーションの再生時間
	//! @param コンボしたいときのアニメーションの一周分の時間
	//! @detail デフォルトアニメーションのセット、アニメーションの入れ物の確保
	void Combo_Judgment_Key(bool* combo_flag, int* mouse_flag, int key, float play_anim_time, float anim_total,int *combo_count);


	//! @brief コンボしてよいのかを指定の条件で判断する関数
	//! @param コンボをしてよいかのフラグ
	//! @param 指定条件のフラグ（bool）
	//! @param コンボしたいときのアニメーションの再生時間
	//! @param コンボしたいときのアニメーションの一周分の時間
	//! @detail デフォルトアニメーションのセット、アニメーションの入れ物の確保
	void Combo_Judgment_Condition(bool* combo_flag, bool condition, float play_anim_time, float anim_total);
};
