#pragma once
#include "src/Scene/Base/Scene_Base.h"
#include "src/System/Vector2.h"
// タイトルシーンのクラス
class TiteleScene : public Scene_Base
{
	//int effeckt_h;
public:
	// 初期処理
	void Init()override;

	// 更新処理
	void Update(int bgm_volume, int se_volume)override;

	// 描画処理
	void Draw()override;

	// 終了処理
	void Exit()override;

public:
	int m_background_image = 0; // 背景画像用の変数
	Vector2 m_image_pos = {0.0f,0.0f};    // 画像用の描画座標
	int m_count_flame = 0; // フレームカウント用の変数 
	int m_count_time = 0;  // フレームから実際の割り出して入れる用の時間
	int m_movie = 0;       // 動画用の変数

	// フォントデータ用
	int m_GTA_font_data = 0; // GTAのフォントデータ用の変数
	LPCSTR m_GTA_font_path = 0;

	enum Title_se
	{
		DECISION, // タイトル画面で指定のボタンを押された時

		SE_MAX,   // SEの最大数
	};

	enum BGM
	{
		TITLE_BGM,  // タイトル画面で流すBGM

		BGM_MAX // BGMの最大数
	};

private:

	// タイトル画面での切り替え用
	enum Title_SCENES
	{
		TITLE, // タイトル画面
		MOVIE, // 動画画面
	};

	// 画面切り替え用の変数
	int m_title_scene = 0;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

};
