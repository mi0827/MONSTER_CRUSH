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

public:
	int m_BGM_Volume = 0; // BGMの音量
	int m_SE_Volume = 0;  // SEの音量
	bool m_option_flag = false;    // オプションメニューを開くためのもの

private:
	Vector2 m_option_box_pos = { 0.0f,0.0f }; // オプションメニュー画面の四角座標
	Vector2 m_BGM_box_pos = { 0.0f,0.0f };    // BGMバーの座標
	Vector2 m_SE_box_pos = { 0.0f,0.0f };     // SEバーの座標

	int m_image_box = 0; // 背景画像用の入れ物
	int m_menu_count = 0; // メニューボタンが押されてから閉じるまでにかかる最低時間をカウントするための物
	int m_select = 0; // SEかBGMを洗濯するためのもの
};