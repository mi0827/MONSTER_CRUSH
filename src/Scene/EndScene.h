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

	//! @brief サウンドの再生
	//! @param 再生したいSE番号
	void SoundPlay(int se_num);


private:


	// テキストオブジェクト
	Text m_text;

	// エンドシーンで描画するテキストの数
	enum EndText
	{
		text1, // タイトルに戻るテキスト
		text2, // クエストシーンに戻るテキスト

		text_max
	};

	//! テキストの描画位置
	Vector2 m_text_draw_pos[text_max] =
	{
		{ (100),(SCREEN_H / 2)},
		{ (100 + SCREEN_W / 2),(SCREEN_H / 2)}
	};

	//! 画面切り替え用の変数
	int m_title_scene = 0;
	//! どちらを洗濯しているか
	int m_select_num = 0;
	//! 選択マスにあったているかどうかのフラグ
	bool m_hit_select_flag = false;
	// タイトル画面でボタンが押されたかどうかのフラグ
	bool m_start_flag = false;

	// カメラクラスのオブジェクト
	Camera camera;

	//! SE用のオブジェクト
	Sound m_se;
	enum se_num
	{
		se_1,

		se_max
	};

private:
	//! シーン変更でどちらのシーンに移動するかを保存する
	int m_change_scene = 0;

	//! エンドシーンで何フレーム進んでいるのかを保存するための変数
	int m_frame_count = 0;
	//! エンドシーンで指定のフレーム過ぎると勝手にシーンを変える
	static constexpr int END_SCENE_FRAME_MAX = 480; // 現在のゲームが６０フレームなので１０秒
};
