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
	static constexpr int TITLE_TIME_MAX = 60;
	//! フレームカウントのマックス
	static constexpr int  FLAME_MAX = 60;
	//! フレームカウント用変数
	int m_count_flame = 0;
	//! フレームから実際の時間を割り出して入れる用の変数
	int m_count_time = 0;

	// スペースキーをちかちかさせるためのもの
	//! フォントの透明度の最大値
	static constexpr int TEXT_BLEND_MAX = 255;
	//! フォントの透明度の最小値
	static constexpr int TEXT_BLEND_MIN = 0;
	//! ブレンド率を変更するための変数
	int m_text_blend_value = 0;
	//! ブレンド率が上限、下限に達したときを判断するためのフラグ
	//! true : 達した、false : 達していない
	bool m_blend_flag = false;
	//! ブレンド率を上げるか下げるかのフラグ
	//! true : 増やす、false : 減らす
	bool m_blend_change_flag = true;


	//! タイトルシーンのSE
	Sound m_sound;
	enum Title_se
	{
		decision, // タイトル画面で指定のボタンを押された時
		
		SE_MAX,   // SEの最大数
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
