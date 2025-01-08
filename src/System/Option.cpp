#include "src/WinMain.h"
#include "Vector2.h"
#include "UIBar.h"
#include "src/Sound/Sound.h"
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
	m_bgm_Volume = 5;
	m_se_Volume = 5;
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
	// 各オプションの値の設定
	for (int i = 0; i < MENU_MAX; i++)
	{
		option_menu[i].m_value = option_menu[i].m_value / 2;
	}

	// 操作説明だけこっちで書く
	//m_op.Set({ BRA_X_STRAT ,m_option_box_pos.y + BRA_Y_DIVISION * 4 }, { 200,30 }, 0, true);
	// SEの初期化
	m_se.NewArraySecureSound(se_max);
	m_se.LoadSound("Data/SE/menu_open.mp3", open_menu_se); // オプションメニューを開いた時
	m_se.LoadSound("Data/SE/menu_close.mp3", close_menu_se); // オプションメニューを閉じるとき
	m_se.LoadSound("Data/SE/menu_selection.mp3", selection_menu_se); // 各メニューを選択しているとき
	m_se.LoadSound("Data/SE/menu_change_value.mp3", change_value_se); // 各数値を変更しているとき
}

//----------------------------------------------
// オプションの更新処理
//----------------------------------------------
void Option::Update()
{
	// オプションメニューだけここでSEのボリュームを調整する
	m_se.SetSoundVolume(option_menu[SE].m_value);

	// オプションメニューを開くとき
	// ボタンを押されたら
	if (PushHitKey(KEY_INPUT_TAB))
	{
		// オプションメニューを開く
		m_option_flag = true;
		// SE再生可能にしておく
		m_se.m_playing_flag = true;
		// 現在再生中のSEを止める
		m_se.StopSound();
	}

	// オプションメニューが開いているとき
	if (m_option_flag)
	{
		// SEの再生
		if (m_se.m_playing_flag)
		{
			SoundPlay(open_menu_se);
		}
		// オプションの更新処理
		OpenMenuUpdate();
	}

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
			// SE再生可能にしておく
			m_se.m_playing_flag = true;
			// 現在再生中のSEを止める
			m_se.StopSound();
			// SEの再生
			SoundPlay(close_menu_se);
		}
	}
	// バーの更新処理
	for (int i = 0; i < MENU_MAX; i++)
	{
		m_bra[i].Update(option_menu[i].m_value);
	}
}

//----------------------------------------------
// オプションの設定
//----------------------------------------------
void Option::SetOptionMenu()
{
	// 各バーの設定
	for (int i = 0; i < MENU_MAX; i++)
	{
		// 各バーの作成
		m_bra[i].Set(option_menu[i].m_pos, m_bra_size, &option_menu[i].m_value, true);

		// 名前の設定
		m_bra[i].SetName(option_menu[i].m_name);

		// 色の設定
		// バー本体の色
		m_bra[i].SetColor(255, 255, 0, &m_bra[i].m_color);
		// バーのバックの色
		m_bra[i].SetColor(128, 128, 0, &m_bra[i].m_back_color);
		// 外枠の色
		m_bra[i].SetColor(0, 0, 0, &m_bra[i].m_line_color);
		// 文字の色
		m_bra[i].SetColor(0, 0, 0, &m_bra[i].m_character_color);
	}
}

//----------------------------------------------
// どのメニューを操作するかの選択処理
//----------------------------------------------
void Option::MenuSelect()
{
	// 上キーを押された時
	if (PushHitKey(KEY_INPUT_UP))
	{
		// SEの再生
		SoundPlay(selection_menu_se);
		m_selection_menu--;
		// 一定以上上に行くと下に移動する
		if (m_selection_menu < BGM)
		{
			m_selection_menu = MOUSE;
		}
	}
	if (PushHitKey(KEY_INPUT_DOWN))
	{
		// SEの再生
		SoundPlay(selection_menu_se);
		m_selection_menu++;
		// 一定以上下に行くと上に移動する
		if (m_selection_menu > MOUSE)
		{
			m_selection_menu = BGM;
		}
	}
	// 選択しているメニューだけ外枠の色を変える
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (i == m_selection_menu)
		{
			// 外枠の色
			m_bra[m_selection_menu].SetColor(255, 255, 255, &m_bra[m_selection_menu].m_line_color);
		}
		else
		{
			// 外枠の色
			m_bra[i].SetColor(0, 0, 0, &m_bra[i].m_line_color);
		}
	}

}

//----------------------------------------------
// オプションが開いているときの更新処理
//----------------------------------------------
void Option::OpenMenuUpdate()
{
	// SE再生可能にしておく
	//m_se.m_playing_flag = true;
	// 現在再生中のSEを止める
	//m_se.StopSound();
	//// SEの再生
	//SoundPlay(close_menu_se);

	// どのメニューを操作するかの選択
	MenuSelect();
	m_menu_count++; // カウントを増やす
	// 選択されたメニューバーの値を増やす
	if (PushHitKey(KEY_INPUT_RIGHT))
	{
		// SEが再生中でなければ
		if (m_se.PlayingSound())
		{
			// SE再生可能にしておく
			m_se.m_playing_flag = true;
			// 現在再生中のSEを止める
			m_se.StopSound();
		}

		// SEの再生
		SoundPlay(change_value_se);
		// 十分割できる用に値を設定して値に足す
		// マウスの変更でけ少し別
		if (m_selection_menu == MOUSE)
		{
			option_menu[m_selection_menu].m_value += MOUSE_SENSI_VALUE;
			if (option_menu[m_selection_menu].m_value >= MOUSE_SENSI_MAX)
			{
				// 最大の値を超えないようにする
				option_menu[m_selection_menu].m_value = MOUSE_SENSI_MAX;
			}
		}
		else
		{
			option_menu[m_selection_menu].m_value += VOLUME_CONSTANT_VALUE;
			if (option_menu[m_selection_menu].m_value >= VOLUME_MAX)
			{
				// 最大の値を超えないようにする
				option_menu[m_selection_menu].m_value = VOLUME_MAX;
			}
		}
	}
	// 選択されたメニューバーの値を減らす
	if (PushHitKey(KEY_INPUT_LEFT))
	{
		// SEの再生
		SoundPlay(change_value_se);
		if (m_selection_menu == MOUSE)
		{
			option_menu[m_selection_menu].m_value -= MOUSE_SENSI_VALUE;
			if (option_menu[m_selection_menu].m_value <= MOUSE_SENSI_MIN)
			{
				// 最小の値を超えないようにする
				option_menu[m_selection_menu].m_value = MOUSE_SENSI_MIN;
			}
		}
		else
		{
			// 十分割できる用に値を設定して値に足す
			option_menu[m_selection_menu].m_value -= VOLUME_CONSTANT_VALUE;
			if (option_menu[m_selection_menu].m_value <= VOLUME_MIN)
			{
				// 最小の値を超えないようにする
				option_menu[m_selection_menu].m_value = VOLUME_MIN;
			}
		}

	}
}

//----------------------------------------------
// サウンドの再生
//----------------------------------------------
void Option::SoundPlay(int se_num)
{
	// メニュー画面ではボタン連打があるため
	// 再生中にからならないために再生中のものを消してい空付きのを鳴らす
	// SEが再生中でなければ
	if (m_se.PlayingSound())
	{
		// SE再生可能にしておく
		m_se.m_playing_flag = true;
		// 現在再生中のSEを止める
		m_se.StopSound();
	}
	// SEの再生
	m_se.PlaySound_(se_num, m_se_info[se_num].play_type, m_se_info[se_num].loop);
	m_se.m_playing_flag = false;
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
