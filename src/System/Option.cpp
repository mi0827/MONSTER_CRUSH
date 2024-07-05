#include "src/WinMain.h"
#include "Vector2.h"
#include "Option.h"

// サウンドの音の範囲
constexpr int Volume_MAX = 255; // 最大
constexpr int Volume_LEAST = 0; // 最小

constexpr int MENU_COUNT = 10; // メニューボタンが押されてから閉じるまでにかかる最低時間

constexpr int BOX_SIZE_X = 280;               // 四角の背景X方向のサイズ
constexpr int BOX_SIZE_Y = 350;               // 四角の背景Y方向のサイズ
constexpr int BOX_SIZE_HARF_X = BOX_SIZE_X / 2; // 背景X方向の半分のサイズ
constexpr int BOX_SIZE_HARF_Y = BOX_SIZE_Y / 2; // 背景Y方向の半分のサイズ

constexpr int BAR_SIZE = 30;                // バーのX座標のサイズ
constexpr int BAR_SPACE_X = BOX_SIZE_HARF_X / 2;// 中心からどれくらい隙間を開けるかの値(X座標)
constexpr int BAR_Y = Volume_MAX / 2;// 中心からどれくらい隙間を開けるかの値(Y座標)



Option::Option()
{
	m_menu_count = 0; // ０からスタート

	// 座標設定
	m_option_box_pos.set(SCREEN_W / 2 - BOX_SIZE_HARF_X, SCREEN_H / 2 - BOX_SIZE_HARF_Y); // 背景
	m_BGM_box_pos.set(SCREEN_W / 2 - BAR_SPACE_X, SCREEN_H / 2 + BAR_Y);
	m_SE_box_pos.set(SCREEN_W / 2 + BAR_SPACE_X - BAR_SIZE, SCREEN_H / 2 + BAR_Y);
	m_option_flag = false; // 最初はオプションメニューは閉じている
	m_BGM_Volume = 100;
	m_SE_Volume = 100;
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
}

//----------------------------------------------
// オプションの更新処理
//----------------------------------------------
void Option::Update()
{

	// オプションメニューを開くとき
	// ボタンを押されたら
	if (IsPadOn(PAD_ID::PAD_START, PAD_NO::PAD_NO1))
	{
		// オプションメニューを開く
		m_option_flag = true;
	}
	if (IsPadOn(PAD_ID::PAD_START, PAD_NO::PAD_NO2))
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
				m_BGM_Volume -= 10;
			}
			// BGMの音量を上げる
			// 上ボタン
			if (IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO2)) 
			{
				m_BGM_Volume += 10;
			}
		}
		else {
			// SEの音量を下げる
			// 下ボタン
			if (IsPadOn(PAD_ID::PAD_D_LEFT, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_LEFT, PAD_NO::PAD_NO2)) 
			{
				m_SE_Volume -= 10;
			}
			// SEの音量を上げる
			// 上ボタン
			if (IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_D_UP, PAD_NO::PAD_NO2))
			{
				m_SE_Volume += 10;
			}
		}
	}
	// BGMの最低の値で止める
	if (m_BGM_Volume <= Volume_LEAST) { m_BGM_Volume = Volume_LEAST; }
	// BGMの最大値で止める
	if (m_BGM_Volume >= Volume_MAX) { m_BGM_Volume = Volume_MAX; }
	// SEの最低の値で止める
	if (m_SE_Volume <= Volume_LEAST) { m_SE_Volume = Volume_LEAST; }
	// SEの最大値で止める
	if (m_SE_Volume >= Volume_MAX) { m_SE_Volume = Volume_MAX; }

	// カウントが一定以上になったら
	if (m_menu_count >= MENU_COUNT) 
	{
		// オプションメニューが開いていて
		// なおボタンが押されたら
		if (IsPadOn(PAD_ID::PAD_START, PAD_NO::PAD_NO1) || IsPadOn(PAD_ID::PAD_START, PAD_NO::PAD_NO2))
		{
			// オプションメニューを閉じる
			m_option_flag = false;
			// カウントをリセット
			m_menu_count = 0;
		}
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
		// BGMバーの描画 
		DrawBox(    (int)m_BGM_box_pos.x, (int)m_BGM_box_pos.y, (int)(m_BGM_box_pos.x + BAR_SIZE), (int)(m_BGM_box_pos.y - m_BGM_Volume), GetColor(0, 0, 0), true);
		DrawLineBox((int)m_BGM_box_pos.x, (int)m_BGM_box_pos.y, (int)(m_BGM_box_pos.x + BAR_SIZE), (int)(m_BGM_box_pos.y - m_BGM_Volume), GetColor(255, 255, 255));
		int original_font_size = GetFontSize();
		SetFontSize(28); // フォントサイズの変更
		const char* bgm = "BGM";
		Vector2 bgm_pos; // 文字列の座標
		bgm_pos.x = (float)GetDrawStringWidth(bgm, -1);
		bgm_pos.y = (float)GetFontSize();
		DrawFormatStringF((m_BGM_box_pos.x + BAR_SIZE / 2 - bgm_pos.x / 2), m_BGM_box_pos.y, GetColor(0, 0, 0), bgm);// 文字列の描画

		// SEバーの描画
		DrawBox(    (int)m_SE_box_pos.x, (int)m_SE_box_pos.y, (int)(m_SE_box_pos.x + BAR_SIZE), (int)(m_SE_box_pos.y - m_SE_Volume), GetColor(0, 0, 0), true);
		DrawLineBox((int)m_SE_box_pos.x, (int)m_SE_box_pos.y, (int)(m_SE_box_pos.x + BAR_SIZE), (int)(m_SE_box_pos.y - m_SE_Volume), GetColor(255, 255, 255));
		const char* se = "SE";
		Vector2 se_pos; // 文字列の座標
		se_pos.x = (float)GetDrawStringWidth(se, -1);
		se_pos.y = (float)GetFontSize();
		DrawFormatStringF((m_SE_box_pos.x + BAR_SIZE / 2 - se_pos.x / 2), m_SE_box_pos.y, GetColor(0, 0, 0), se); // 文字列の描画
	
	
		// どちらのバーを選んでいるかわかりやすくするためのもの
		if (m_select == 0) 
		{
			// BGM側
			DrawLineBox((int)m_BGM_box_pos.x, (int)m_BGM_box_pos.y, (int)(m_BGM_box_pos.x + BAR_SIZE), (int)(m_BGM_box_pos.y - m_BGM_Volume), GetColor(255, 255, 0));
			DrawFormatStringF(m_BGM_box_pos.x + BAR_SIZE / 2 - bgm_pos.x / 2 - 2, m_BGM_box_pos.y - 2, GetColor(255, 255, 0), bgm);
		}
		else
		{
			// SE側
			DrawLineBox((int)m_SE_box_pos.x, (int)m_SE_box_pos.y, (int)(m_SE_box_pos.x + BAR_SIZE), (int)(m_SE_box_pos.y - m_SE_Volume), GetColor(225, 255, 0));
			DrawFormatStringF(m_SE_box_pos.x + BAR_SIZE / 2 - se_pos.x / 2 -2, m_SE_box_pos.y-2, GetColor(255, 255, 0), se);
		}
		SetFontSize(original_font_size); // フォントサイズを戻す
	}

}
