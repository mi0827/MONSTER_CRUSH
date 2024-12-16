#pragma once

//! @file Combo.h
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
	void ComboJudgmentKey(bool* combo_flag, int* mouse_flag, int key, float play_anim_time, float anim_total,int *combo_count);


	//! @brief コンボしてよいのかを指定の条件で判断する関数
	//! @param コンボをしてよいかのフラグ
	//! @param 指定条件のフラグ（bool）
	//! @param コンボしたいときのアニメーションの再生時間
	//! @param コンボしたいときのアニメーションの一周分の時間
	//! @detail デフォルトアニメーションのセット、アニメーションの入れ物の確保
	void ComboJudgmentCondition(bool* combo_flag, bool condition, float play_anim_time, float anim_total);


	//! @brief コンボモードの切り替え用関数
	//! @param 設定したいコンボモード
	void ChangeComboMode(int combo_mode);

	//! @breif コンボをを始めていいかを判断する関数
	void ComboStartJudgent();

	//! @brief コンボを続けていいか
	bool  ComboCanContinueJudgent();

	enum ComboMode
	{
		COMBO_START,    // コンボスタート
		COMBO_STANDBY,  // コンボ待機中
		COMBP_IN,       // コンボ中
		COMBO_FINISH,   // コンボ終了
	};
	//ComboMode m_combo_mode;
	int m_combo_mode;
};
