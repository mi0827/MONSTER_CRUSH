#pragma once

//! @file EndScene.h
//! @brief タイトルシーンのクラス
class EndScene : public Scene_Base
{
	//int effeckt_h;
public:

	//! コンストラクタ
	EndScene();
	//! デストラクタ
	~EndScene();


	//! @brief 初期処理
	void Init()override;

	//! @brief 更新処理
	void Update()override;

	//! @brief 描画処理
	void Draw()override;

	//! @brief 終了処理
	void Exit()override;

	//! @brief フィールドとキャラクターとの当たり判定
	void HitField()override;

	//! @brief オプションメニューでもらってきた値を反映する関数
    //! @param BGMの音量
	//! @param SEの音量
	//! @param マウス感度
	void OptionValuesReflect(int bgm, int se, int mouse)override;

public:
	

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
	////! 描画したいテキストを番号で管理
	//enum Text
	//{
	//	// end_text,             // エンドシーンというのがわかりやすくする
	//	return_title,          // タイトルシーンに移動する
	//	return_qusetarea, // クエスト選択エリアに移動

	//	text_max
	//};


	// テキストオブジェクト
	Text m_text;
	
	// エンドシーンで描画するテキストの数
	enum EndText
	{
		text1,
		text2,
		text3,
		text4,
		text_max
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
