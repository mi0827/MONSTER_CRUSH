
#pragma once

// タイトルシーンのクラス
class EndScene : public Scene_Base
{
	//int effeckt_h;
public:

	//! コンストラクタ
	EndScene();
	//! デストラクタ
	~EndScene();


	// 初期処理
	void Init()override;

	// 更新処理
	void Update()override;

	// 描画処理
	void Draw()override;

	// 終了処理
	void Exit()override;

	// フィールドとキャラクターとの当たり判定
	void HitField()override;
public:
	//int m_background_image = 0; // 背景画像用の変数
	//Vector2 m_image_pos = {0.0f,0.0f};    // 画像用の描画座標
	//int m_count_flame = 0; // フレームカウント用の変数 
	//int m_count_time = 0;  // フレームから実際の割り出して入れる用の時間
	//int m_movie = 0;       // 動画用の変数

	//// フォントデータ用
	//int m_GTA_font_data = 0; // GTAのフォントデータ用の変数
	//LPCSTR m_GTA_font_path = 0;

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
	//! 描画したいテキストを番号で管理
	enum Text
	{
		// end_text,             // エンドシーンというのがわかりやすくする
		return_title,          // タイトルシーンに移動する
		return_qusetarea, // クエスト選択エリアに移動

		text_max
	};

	//! 文字列を描画するための構造体
	struct TextState
	{
		//! フォントサイズ
		int font_size;
		//! 文字列を保存するためのもの
		const char* text;
		//! 描画したい座標
		Vector2 draw_pos;

	};
	//! 描画したい文字列の数分作成
	TextState m_text[text_max]
	{
		{30,"LeftShift + Enter : クエスト受付エリアに移動", {385, 100}},
		{30,"RightShift + Enter : タイトルに移動", {300, 100}}
	};

	// 画面切り替え用の変数
	int m_title_scene = 0;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

	// カメラクラスのオブジェクト
	Camera camera;

	// フィールドオブジェクト
	Field field;

	// プレイヤーのオブジェクト
	CharacterBase* player;

private:
	//! シーン変更でどちらのシーンに移動するかを保存する
	int m_change_scene = 0;
};
