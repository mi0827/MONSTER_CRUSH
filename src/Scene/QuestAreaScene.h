
#pragma once

// タイトルシーンのクラス
class QuestAreaScene : public Scene_Base
{
	//int effeckt_h;
public:
	// 話せるようになる範囲の半径
	static constexpr float AREA_FIELD_R = 15.0f;

	//! 初期処理
	void Init()override;

	//! 更新処理
	void Update()override;

	//! 描画処理
	void Draw()override;

	//! 終了処理
	void Exit()override;

	//! フィールドとキャラクターの当たり判定
	void HitField()override;

	//! 話せるようになるエリアの設定
	void InitArea();

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

	// タイトル画面での切り替え用
	enum Title_SCENES
	{
		TITLE, // タイトル画面
		MOVIE, // 動画画面
	};

	//! 画面切り替え用の変数
	int m_title_scene = 0;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

	//! カメラクラスのオブジェクト
	Camera camera;

	//! フィールドオブジェクト
	TitleField field;

	//! プレイヤーのオブジェクト
	CharacterBase* player;

	//! 受付嬢のオブジェクト
	Receptionist receptionist;
	//! エリア用のカプセル
	CapsuleCollision m_area;
	//! 話せるエリアに入ったか入っていないかのフラグ
	bool m_area_hit = false;


};
