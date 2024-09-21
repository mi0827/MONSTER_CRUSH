#pragma once

// タイトルシーンのクラス
class TitleScene : public Scene_Base
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

	//! @brief エリアBOXの初期設定
	void SetAeraBoxSet();


public:
	

	

private:
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

	// 一定間隔で存在するエリアをまとめたもの
	enum Area
	{
		next_scene, // 次のシーンに行くためのエリア

		max
	};


	// 画面切り替え用の変数
	int m_title_scene = 0;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

	//! カメラクラスのオブジェクト
	Camera camera;

	// フィールドオブジェクト
	TitleField field;

	// プレイヤーのオブジェクト
	CharacterBase* player;

	// エリア用のBOX
	BoxCollision m_area_box[max];
	// BOXの座標
	Vector3 m_box_pos[max];
	// BOXのサイズ（全部一緒）
	Vector3 m_box_size;
};
