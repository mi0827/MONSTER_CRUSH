#pragma once
//! @file TitleScene.h
//! @brief タイトルシーンのクラス
class TitleScene : public Scene_Base
{
public:
	// 初期処理
	void Init()override;

	// 更新処理
	void Update()override;
	// 描画処理
	void Draw()override;

	// 終了処理
	void Exit()override;

	//! @brief フィールドとキャラクターとの当たり判定
	void HitField()override;

	//! @brief オプションメニューでもらってきた値を反映する関数
	//! //! @param BGMの音量
	//! @param SEの音量
	//! @param マウス感度
	void OptionValuesReflect(int bgm, int se, int mouse)override;

	//! @brief タイトルの更新処理
	void TitleUpdate();

	

private:
	//! タイトル描画時間
	static constexpr int TITLE_TIME_MAX = 5;
	//! フレームカウントのマックス
	static constexpr int  FLAME_MAX = 60;
	//! フレームカウント用変数
	int m_count_flame = 0;
	//! フレームから実際の時間を割り出して入れる用の変数
	int m_count_time = 0;

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

	// タイトル画面での切り替え用
	enum Title_SCENES
	{
		TITLE, // タイトル画面
		MOVIE, // 動画画面
	};
	// タイトルシーンの状態を保存する変数（最初はタイトル状態から始める）
	int m_title_mode_num = Title;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

	//! カメラクラスのオブジェクト
	Camera camera;
	

	//! テキストオブジェクト
	Text m_text;
	//! どのテキストファイルを開くか
	int m_text_num = 0;


	//! プレイヤー動画用変数
	int m_movie = 0;
	//! 背景画像用の変数
	int m_background_image = 0;
};
