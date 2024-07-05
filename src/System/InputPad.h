//---------------------------------------------------------------------------
//!	@file	InputPad.h
//! @brief	パッド入力管理
//---------------------------------------------------------------------------
#pragma once

//---------------------------------------------------------------------------
//! パッド種別
//---------------------------------------------------------------------------
//@{

enum class PAD_NO : unsigned int
{
	PAD_NO1 = 0,
	PAD_NO2,
	PAD_NO3,
	PAD_NO4,

	PAD_NO_MAX
};

//@}
//---------------------------------------------------------------------------
//! ボタン種別
//---------------------------------------------------------------------------
//@{
enum class PAD_ID : unsigned int
{
	PAD_A = 0,	// Aキー
	PAD_B,		// Bキー
	PAD_X,		// Xキー
	PAD_Y,		// Yキー
	PAD_L,		// LBボタン（左人差し指側）
	PAD_R,		// RBボタン（右人差し指側）
	PAD_START,	// STARTボタン
	PAD_BACK,	// BACKボタン
	PAD_L_PUSH,	// 左アナログスティック押し込み
	PAD_R_PUSH,	// 右アナログスティック押し込み

	// 左アナログスティック
	PAD_UP,		// 上
	PAD_DOWN,	// 下
	PAD_LEFT,	// 左
	PAD_RIGHT,	// 右

	// 右アナログスティック
	PAD_R_UP,	// 上
	PAD_R_DOWN,	// 下
	PAD_R_LEFT,	// 左
	PAD_R_RIGHT,// 右

	// 方向キー
	PAD_D_UP,	// 上
	PAD_D_DOWN,	// 下
	PAD_D_LEFT,	// 左
	PAD_D_RIGHT,// 右
	PAD_D_ALL,	// 方向キー全て

	PAD_ID_MAX,

	// 場所不明
	PAD_C,		// 対応キーなし
	PAD_Z,		// 対応キーなし
	PAD_M,		// 対応キーなし

};

//@}
//===========================================================================
//!	@name	システム関数
//===========================================================================
//@{

//! 初期化
void InputPadInit();

//! 更新
void InputPadUpdate();

//! 終了
void InputPadExit();

//@}
//===========================================================================
//!	@name	入力関連関数
//===========================================================================
//@{

//! 指定パッドのボタンの1回だけ押下検証
//! @param	[in]	pad_id	パッドのボタン種別
//! @param	[in]	pad_no	パッドの種別(指定がなければPAD_NO1)
//! @retval true	引数に指定されたパッドのボタンが押された
//! @retval false	引数に指定されたパッドのボタンが押されていない
bool IsPadOn(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 指定パッドのボタンの1回だけ押下検証
//! @param	[in]	pad_id	パッドのボタン種別
//! @param	[in]	pad_no	パッドの種別(指定がなければPAD_NO1)
//! @retval true	引数に指定されたパッドのボタンが離されている
//! @retval false	引数に指定されたパッドのボタンが押されている
bool IsPadRelease(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 指定パッドのボタンの1回だけ押下検証
//! @param	[in]	pad_id	パッドのボタン種別
//! @param	[in]	pad_no	パッドの種別(指定がなければPAD_NO1)
//! @retval true	引数に指定されたパッドのボタンが長押し（2フレーム以上）されている
//! @retval false	引数に指定されたパッドのボタンが押されていない（押されているフレーム数が1以下）
bool IsPadRepeat(PAD_ID pad_id, PAD_NO pad_no = PAD_NO::PAD_NO1);

////////////////////////////////////////////
//		新規追加
////////////////////////////////////////////
//! 指定パッドの入力状態の詳細を取得
//! @param	[in]	pad_state	パッドの入力状態の詳細（参照）
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval true	成功
//! @retval false	失敗
bool GetPadInputState(DINPUT_JOYSTATE& pad_state, PAD_NO pad_no = PAD_NO::PAD_NO1);

//! 指定パッドの左アナログスティックのX方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval float	-1（左に100%傾いている）～0（傾きなし）～1（右に100％傾いている）
float GetPadInputAnalogLX(int pad_no = 1);
//! 指定パッドの左アナログスティックのY方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval float	-1（上に100%傾いている）～0（傾きなし）～1（下に100％傾いている）
float GetPadInputAnalogLY(int pad_no = 1);

//! 指定パッドの右アナログスティックのX方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval float	-1（左に100%傾いている）～0（傾きなし）～1（右に100％傾いている）
float GetPadInputAnalogRX(int pad_no = 1);
//! 指定パッドの右アナログスティックのY方向の入力値を取得
//! @param	[in]	pad_no		パッドの種別(指定がなければPAD_NO1)
//! @retval float	-1（上に100%傾いている）～0（傾きなし）～1（下に100％傾いている）
float GetPadInputAnalogRY(int pad_no = 1);


// パッドの振動用関数
// 引数
// １：どのパッドを振動させるか
// ２：振動の強さ
// ３：振動させる時間
// ４：振動させるモーターの番号（-1で自動処理）
void PadVidation(int inputType, int power, int time, int motor);

// Debug
void InputPadRender();

//@}

