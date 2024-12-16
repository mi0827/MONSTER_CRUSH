#pragma once
//! @file QusetAreaScene.h
//! @brief タイトルシーンのクラス
class QuestAreaScene : public Scene_Base
{
public:
	//! コンストラクタ
	QuestAreaScene();
	//! デストラクタ
	~QuestAreaScene();

	//!  話せるようになる範囲の半径
	static constexpr float AREA_FIELD_R = 15.0f;

	//! @brief 初期処理
	void Init()override;

	//! @brief 更新処理
	void Update()override;

	//! @brief 描画処理
	void Draw()override;

	//! @brief 終了処理
	void Exit()override;

	//! @brief フィールドとキャラクターの当たり判定
	void HitField()override;

	//! @brief オプションメニューでもらってきた値を反映する関数
	//! //! @param BGMの音量
	//! @param SEの音量
	//! @param マウス感度
	void OptionValuesReflect(int bgm, int se, int mouse)override;

	//! @brief 話せるようになるエリアの設定
	void InitArea();



private:


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

	//! @brief クエストメニューテキスト
	Text m_quest_text;
	//! @brief クエストエリアでのテキスト
	Text m_quest_area_text;

	//　エリアでのテキストの数
	enum StoryText
	{
		story1,
		story2,
		story_max
	};
	
	// クエストテキストの数
	enum QuestText
	{
		quest1,
		quest2,
		quest_max
	};

	//! 描画するテキストの番号として管理
	enum Text
	{
		f_text,                // Fボタンをしたらのメッセージ
		excamation_pos, // ビックリマーク

		text_max
	};

	//! 文字列スクリーン座標に描画するための構造体
	struct TextState
	{
		////! フォントサイズ
		//int font_size;
		//! 文字列を保存するためのもの
		const char* text;
		//! 描画したい座標
		Vector3 draw_pos;
		//! ずらしたい分の座標
		Vector3 shift_pos;
		//! 一定のスクリーン画面から出たら描画しないようにするためのもの
		bool draw_flag;
	};
	// 描画したい文字列の数分作成
	TextState m_text[text_max]
	{
		{	/*100,*/"F : 話す", {0.0f,0.0f,0.0f},{-8.0f,10.0f,0.0f},false},
		{	/*100,*/" ! ", {0.0f,0.0f,0.0f},{0.0f,20.0f,0.0f},false},
	};
	static constexpr int FONT_SIZE = 100;

};
