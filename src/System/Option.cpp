#include "src/WinMain.h"
#include "Vector2.h"
#include "UIBar.h"
#include "Option.h"

// オプションメニューでやること
// BGM,SE,のボリューム調整
// マウスの感度調整
// 操作説明
// 

// 
// オプション背景は半透明で
// 音量は１０分割のタイプにする
// マウス感度の１０分割
// 操作説明は後回し

Option::Option()
{
	m_menu_count = 0; // ０からスタート

	// 座標設定
	//m_option_box_pos.set(SCREEN_W / 2 - BOX_SIZE_HARF_X, SCREEN_H / 2 - BOX_SIZE_HARF_Y); // 背景
	//m_bgm_box_pos.set(SCREEN_W / 2 - BAR_SPACE_X, SCREEN_H / 2 + BAR_Y);
	//m_se_box_pos.set(SCREEN_W / 2 + BAR_SPACE_X - BAR_SIZE, SCREEN_H / 2 + BAR_Y);
	m_option_flag = false; // 最初はオプションメニューは閉じている
	m_bgm_Volume = 10;
	m_se_Volume = 10;
	m_select = 0; // BGMからスタート
}

Option::~Option()
{
}

//----------------------------------------------
// 初期処理
//----------------------------------------------
void Option::Init()
{
	m_image_box = LoadGraph("Data/Option/OptionMenu.png");
	// 各バーの設定
	SetOptionMenu();
	// 操作説明だけこっちで書く
	//m_op.Set({ BRA_X_STRAT ,m_option_box_pos.y + BRA_Y_DIVISION * 4 }, { 200,30 }, 0, true);
}



//----------------------------------------------
// オプションの更新処理
//----------------------------------------------
void Option::Update()
{

	// オプションメニューを開くとき
	// ボタンを押されたら
	if (PushHitKey(KEY_INPUT_TAB))
	{
		// オプションメニューを開く
		m_option_flag = true;
	}

	// オプションメニューが開いているとき
	if (m_option_flag)
	{
		m_menu_count++; // カウントを増やす
		// 左右のボタンで変更したい方を選択
		// ０：BGM
		// １：SE
		// 左ボタン
		if (IsPadOn(PAD_ID::PAD_D_DOWN, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_DOWN, PAD_NO::PAD_NO2))
		{
			m_select -= 1;
			if (m_select < 0)
			{
				m_select = 1;
			}
		}
		// 右ボタン
		if (IsPadOn(PAD_ID::PAD_D_RIGHT, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_RIGHT, PAD_NO::PAD_NO2))
		{
			m_select += 1;
			if (m_select >= 2)
			{
				m_select = 0;
			}
		}
		if (m_select == 0) {
			// BGMの音量を下げる
			// 下ボタン
			if (IsPadOn(PAD_ID::PAD_D_LEFT, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_LEFT, PAD_NO::PAD_NO2))
			{
				m_bgm_Volume -= 10;
			}
			// BGMの音量を上げる
			// 上ボタン
			if (IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO2))
			{
				m_bgm_Volume += 10;
			}
		}
		else {
			// SEの音量を下げる
			// 下ボタン
			if (IsPadOn(PAD_ID::PAD_D_LEFT, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_LEFT, PAD_NO::PAD_NO2))
			{
				m_bgm_Volume -= 10;
			}
			// SEの音量を上げる
			// 上ボタン
			if (IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO2))
			{
				m_se_Volume += 10;
			}
		}
	}
	//// BGMの最低の値で止める
	//if (m_bgm_Volume <= Volume_LEAST) { m_bgm_Volume = Volume_LEAST; }
	//// BGMの最大値で止める
	//if (m_bgm_Volume >= Volume_MAX) { m_bgm_Volume = Volume_MAX; }
	//// SEの最低の値で止める
	//if (m_se_Volume <= Volume_LEAST) { m_se_Volume = Volume_LEAST; }
	//// SEの最大値で止める
	//if (m_se_Volume >= Volume_MAX) { m_se_Volume = Volume_MAX; }

	// カウントが一定以上になったら
	if (m_menu_count >= MENU_COUNT)
	{
		// オプションメニューが開いていて
		// なおボタンが押されたら
		if (PushHitKey(KEY_INPUT_TAB))
		{
			// オプションメニューを閉じる
			m_option_flag = false;
			// カウントをリセット
			m_menu_count = 0;
		}
	}

	// バーの更新処理
	for (int i = 0; i < MENU_MAX; i++)
	{
		m_bra[i].Update(&option_menu[i].m_value);
	}
}

void Option::SetOptionMenu()
{
	// 各バーの設定
	for (int i = 0; i < MENU_MAX; i++)
	{
		m_bra[i].Set(option_menu[i].m_pos, m_bra_size, &option_menu[i].m_value, true);

		// 名前の設定
		m_bra[i].SetName(option_menu[i].m_name);

		// 色の設定
		// バー本体の色
		m_bra[i].SetColor(255, 255, 0, &m_bra[i].m_color);
		// バーのバックの色
		m_bra[i].SetColor(128, 128, 0, &m_bra[i].m_back_color);
		// 外枠の色
		m_bra[i].SetColor(255, 255, 255, &m_bra[i].m_line_color);
		// 文字の色
		m_bra[i].SetColor(0, 0, 0, &m_bra[i].m_character_color);
	}



}

//----------------------------------------------
// オプションの描画処理
//----------------------------------------------
void Option::Draw()
{
	// ChangeFont("ＭＳ 明朝");
	// オプションメニューが開いている時だけ
	// 描画する
	if (m_option_flag)
	{
		DrawExtendGraphF(m_option_box_pos.x, m_option_box_pos.y, m_option_box_pos.x + BOX_SIZE_X, m_option_box_pos.y + BOX_SIZE_Y, m_image_box, TRUE);  // オプションメニューの背景
		//	// BGMバーの描画 
		//	DrawBox(    (int)m_bgm_box_pos.x, (int)m_bgm_box_pos.y, (int)(m_bgm_box_pos.x + BAR_SIZE), (int)(m_bgm_box_pos.y - m_bgm_Volume), GetColor(0, 0, 0), true);
		//	DrawLineBox((int)m_bgm_box_pos.x, (int)m_bgm_box_pos.y, (int)(m_bgm_box_pos.x + BAR_SIZE), (int)(m_bgm_box_pos.y - m_bgm_Volume), GetColor(255, 255, 255));

		//	const char* bgm = "BGM";
		//	Vector2 bgm_pos; // 文字列の座標
		//	bgm_pos.x = (float)GetDrawStringWidth(bgm, -1);
		//	bgm_pos.y = (float)GetFontSize();
		//	DrawFormatStringF((m_bgm_box_pos.x + BAR_SIZE / 2 - bgm_pos.x / 2), m_bgm_box_pos.y, GetColor(0, 0, 0), bgm);// 文字列の描画

		//	// SEバーの描画
		//	DrawBox(    (int)m_se_box_pos.x, (int)m_se_box_pos.y, (int)(m_se_box_pos.x + BAR_SIZE), (int)(m_se_box_pos.y - m_se_Volume), GetColor(0, 0, 0), true);
		//	DrawLineBox((int)m_se_box_pos.x, (int)m_se_box_pos.y, (int)(m_se_box_pos.x + BAR_SIZE), (int)(m_se_box_pos.y - m_se_Volume), GetColor(255, 255, 255));
		//	const char* se = "SE";
		//	Vector2 se_pos; // 文字列の座標
		//	se_pos.x = (float)GetDrawStringWidth(se, -1);
		//	se_pos.y = (float)GetFontSize();
		//	DrawFormatStringF((m_se_box_pos.x + BAR_SIZE / 2 - se_pos.x / 2), m_se_box_pos.y, GetColor(0, 0, 0), se); // 文字列の描画
		//
		//
		//	// どちらのバーを選んでいるかわかりやすくするためのもの
		//	if (m_select == 0) 
		//	{
		//		// BGM側
		//		DrawLineBox((int)m_bgm_box_pos.x, (int)m_bgm_box_pos.y, (int)(m_bgm_box_pos.x + BAR_SIZE), (int)(m_bgm_box_pos.y - m_bgm_Volume), GetColor(255, 255, 0));
		//		DrawFormatStringF(m_bgm_box_pos.x + BAR_SIZE / 2 - bgm_pos.x / 2 - 2, m_bgm_box_pos.y - 2, GetColor(255, 255, 0), bgm);
		//	}
		//	else
		//	{
		//		// SE側
		//		DrawLineBox((int)m_se_box_pos.x, (int)m_se_box_pos.y, (int)(m_se_box_pos.x + BAR_SIZE), (int)(m_se_box_pos.y - m_se_Volume), GetColor(225, 255, 0));
		//		DrawFormatStringF(m_se_box_pos.x + BAR_SIZE / 2 - se_pos.x / 2 -2, m_se_box_pos.y-2, GetColor(255, 255, 0), se);
		//	}

		// デフォルトの文字の大きさを保存しておく
		int original_font_size = GetFontSize();
		SetFontSize(28); // フォントサイズの変更
		// 各バーの描画処理
		for (int i = 0; i < MENU_MAX; i++)
		{
			m_bra[i].Draw();
		}
		SetFontSize(original_font_size); // フォントサイズを戻す
	}

}
