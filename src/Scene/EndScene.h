#pragma once

class EndScene : public Scene_Base
{
public:
	// 初期処理
	void Init()override;

	// 更新処理
	void Update(int bgm_volume, int se_volume)override;

	// 描画処理
	void Draw()override;

	// 終了処理
	void Exit()override;

	// BGMの初期処理
	void BGM_Init();

public:
	int m_background_image = 0; // 背景画像用の
	Vector2 m_image_pos = {0.0f,0.0f};    // 画像用の描画座標

	int m_count_flame = 0; // フレームカウント用の変数 
	int m_count_time = 0;  // フレームから実際の割り出して入れる用の時間

	// フォントデータ用
	int m_GTA_font_data = 0; // GTAのフォントデータ用の変数
	LPCSTR m_GTA_font_path = 0;


	enum BGM
	{

		BGM_1, // 一つ目のbgm

		BGM_MAX // bgmの最大数
	};
};