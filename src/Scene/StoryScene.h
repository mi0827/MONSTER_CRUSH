#pragma once
#pragma once
//! @file StoryScene.h
//! @brief タイトルシーンのクラス
class StoryScene : public Scene_Base
{
	//int effeckt_h;
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

	//! @brief エリアBOXの初期設定
	void SetAeraBoxSet();

private:
	enum SE
	{
		DECISION, // ストーリー画面で指定のボタンを押された時

		SE_MAX,   // SEの最大数
	};

	//enum BGM
	//{
	//	Story_BGM,  // タイトル画面で流すBGM

	//	BGM_MAX // BGMの最大数
	//};

	

	// 一定間隔で存在するエリアをまとめたもの（このエリアでテキストの進行も進める）
	enum Area
	{

		story1, // ストーリー１
		story2, // ストーリー２
		story3, // ストーリー３
		story4, // ストーリー４
		story5, // ストーリー５
		story6, // ストーリー6
		next_scene, // 次のシーンに行くためのエリア

		Area_Max
	};

	// ストーリーの数
	static constexpr int STORY_NUM_MAX = next_scene;

	// 画面切り替え用の変数
	int m_title_scene = 0;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

	//! カメラクラスのオブジェクト
	Camera camera;

	// エリア用のBOX
	BoxCollision m_area_box[Area_Max];
	// BOXの座標
	Vector3 m_box_pos[Area_Max]
	{
		{0.0f, 50.0f, 750},
		{0.0f, 50.0f, 650},
		{0.0f, 30.0f, 550},
		{0.0f, 30.0f, 450},
		{0.0f, 0.0f, 350},
		{0.0f, 0.0f, 250},
		{0.0f, 0.0f, 150},
	};
	// BOXのサイズ（全部一緒）
	Vector3 m_box_size;

	// テキストオブジェクト
	Text m_text;
	// どのテキストファイルを開くか
	int m_text_num = 0;

	// 何番のエリアと当たり判定があったかを保存する
	int hit_area = 0;
};
