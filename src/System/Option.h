#pragma once

class Option
{
public:
	// コンストラクタ
	Option();
	// デストラクタ
	~Option();
	// 初期処理
	void Init();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	//! @brief 各オプションメニューのバーの設定
	void SetOptionMenu();

private:
	// 範囲
	static constexpr int VOLUME_MAX = 10;  // 最大
	static constexpr int VOLUME_LEAST = 0; // 最小
	static constexpr int VOLUME_CONSTANT_VALUE = 25; // VOLUMEが１で変化する値
	// メニューボタンが押されてから閉じるまでにかかる最低時間
	static constexpr int MENU_COUNT = 10; 

	static constexpr int BOX_SIZE_X = SCREEN_W/ 2;         // 四角の背景X方向のサイズ
	static constexpr int BOX_SIZE_Y = SCREEN_H / 2;        // 四角の背景Y方向のサイズ
	static constexpr int BOX_SIZE_HARF_X = BOX_SIZE_X / 2; // 背景X方向の半分のサイズ
	static constexpr int BOX_SIZE_HARF_Y = BOX_SIZE_Y / 2; // 背景Y方向の半分のサイズ
	// 各バーの設定で使う
	static constexpr int BRA_X_DIVISION =  BOX_SIZE_X / 5;  // 背景画像用X座標指定の分割で分けた時の大きさ
	static constexpr int BRA_Y_DIVISION =  BOX_SIZE_Y / 5;  // 背景画像用Y座標指定の分割で分けた時の大きさ


	static constexpr int BAR_X_SIZE = 550;                 // バーのX座標のサイズ
	static constexpr int BAR_Y_SIZE = 50;                  // バーの座標のサイズ
	static constexpr int BAR_SPACE_X = BOX_SIZE_HARF_X / 2; // 中心からどれくらい隙間を開けるかの値(X座標)
	static constexpr int BAR_Y = VOLUME_MAX / 2;            // 中心からどれくらい隙間を開けるかの値(Y座標)


public:
	int m_bgm_Volume = 0; // BGMの音量
	int m_se_Volume = 0;  // SEの音量
	int m_mouse_sensi = 0; // マウス感度
	bool m_option_flag = false;    // オプションメニューを開くためのもの

private:

	// オプションメニュー種類
	enum Menu
	{
		BGM,     // 音量
		SE,        // 音量
		MOUSE, //マウスの感度
		OP,        // 操作説明

		MAX
	};
	// 操作説明をはぶいたメニューの最大数
	static constexpr int MENU_MAX = MOUSE + 1;

	//! オプションメニュー画面の背景の中心座標
	Vector2 m_option_box_pos = { SCREEN_W / 2 - BOX_SIZE_HARF_X, SCREEN_H / 2 - BOX_SIZE_HARF_Y }; 
	// 各バーのX座標
	static constexpr float BRA_X_STRAT = SCREEN_W / 2 - BOX_SIZE_HARF_X + BRA_X_DIVISION;
	
	// 各ステータスで使うもの
	struct Information
	{
		Vector2 m_pos;
		// 値
		int m_value = 0;
		// 文字列用の入れ物
		const char* m_name;
		// バーの色
		int m_color; 
	};
	// 操作説明以外の３つ分を用意
	Information option_menu[MENU_MAX]
	{
		//! BGM用
		{{ BRA_X_STRAT ,  m_option_box_pos.y + BRA_Y_DIVISION},  VOLUME_MAX, "BGM"},
		//! SE用
		{{ BRA_X_STRAT , m_option_box_pos.y + BRA_Y_DIVISION * 2}, VOLUME_MAX, "SE"},
		//! マウス感度用
		{{ BRA_X_STRAT ,m_option_box_pos.y + BRA_Y_DIVISION * 3},  VOLUME_MAX, "マウス感度"}

	};

	// それぞれで描画するバー
	UIBra m_bra[MENU_MAX];
	UIBra m_op;

	// 各バーのサイズの設定
	Vector2 m_bra_size = { BAR_X_SIZE, BAR_Y_SIZE };

	
	

	int m_image_box = 0; // 背景画像用の入れ物
	int m_menu_count = 0; // メニューボタンが押されてから閉じるまでにかかる最低時間をカウントするための物
	int m_select = 0; // SEかBGMを洗濯するためのもの
};