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

	//! @brief 受付嬢の目印の更新処理
	void LandMarkUpdate();

	//! @brief クエストエリアの更新処理
	void QuestAreaUpdate();

	//! @brief 会話していない状態の更新処理
	void ModeNormalUpdate();

	//! @brief 会話パートの更新処理
	void ConvoUpdate();

	//! @breif クエストを受けているときの更新処理
	void AcceptingQuestUpdate();

	//! @brief 会話していない状態の描画処理
	void ModeNormalDraw();

	//! @brief 会話パートの描画処理
	void ConvoDraw();

	//! @breif クエストを受けているときの描画処理
	void AcceptingQuestDraw();


private:
	// このシーンの状態を管理
	enum SecenNum
	{
		normal, // 会話をしていないとき
		convo,  // 会話中
		accepting_quest, // クエスト受付中

		secen_max
	};
	// クエストシーンがどの状態化を補完する
	int secen_mode_num = normal;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

	//! カメラクラスのオブジェクト
	Camera camera;

	//! 受付嬢のオブジェクト
	Receptionist receptionist;
	//! エリア用のカプセル
	CapsuleCollision m_area;
	//! 話せるエリアに入ったか入っていないかのフラグ
	bool m_area_hit = false;

	//! プレイヤーが会話中かどうか
	bool m_convo_flag = false;

	//! クエストメニューテキスト
	Text m_quest_text;
	//! クエストエリアのテキスト
	Text m_quest_area_text;
	//! クエスト受付嬢のテキスト
	Text m_reception_text;
	//! 各テキストのフォントサイズ
	static constexpr int TEXT_FONT_SIZE = 60;
	//! 各テキストの何行目を呼んでいるのかを保存するための変数
	int m_text_line_num;
	//　エリアでのテキストの数
	enum PlayerText
	{
		story1,
		story_max
	};

	// クエストテキストの数
	enum QuestText
	{
		quest1,
		quest2,
		confirmatio1,
		yes,
		no,
		confirmation2,
		quest_max
	};

	// 受付嬢のテキストの数
	enum ReceptionText
	{
		convo_text_1,
		convo_text_2,
		convo_text_3,
		convo_text_4,
		teception_max
	};

	//! 描画するテキストの番号として管理
	enum LandMarkText
	{
		f_text,                // Fボタンをしたらのメッセージ
		excamation_pos, // ビックリマーク

		landmark_text_max
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
	TextState m_landmark_text[landmark_text_max]
	{
		{	/*100,*/"F : 話す", {0.0f,0.0f,0.0f},{-8.0f,10.0f,0.0f},false},
		{	/*100,*/" ! ", {0.0f,0.0f,0.0f},{0.0f,20.0f,0.0f},false},
	};
	// 目印の文字の大きさ
	static constexpr int FONT_SIZE = 100;

	// 現在どのテキストを描画するのかを管理するための列挙体
	enum TextNum
	{
		landmark_text, // 目印となっているテキスト
		player_text,      // プレイヤーが話しているときに出てくるテキスト
		reception_text, // 受付嬢が話しているときに出てくるテキスト
		quest_text,      // クエスト画面を描画しているときの処理

		text_max
	};

	//---------------------------------------------
	// クエスト関連
	//---------------------------------------------
	// クエスト選択画面の状態
	enum QuestSelectionMode
	{
		quest_selection,
		reply_selection,
	};
	// クエスト選択画面の状態
	int m_quest_selection_num;
	// どのクエストを選んだか保存する変数
	int m_quest_num;
	// 返答がどちらかなのかを保存する変数
	int m_reply_num;
	// 選択画面で選択しているものを示すための変数
	int m_select_num = 0;


	Vector2 m_quest_draw_pos[quest_max] =
	{
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
	};


	//---------------------------------------------
    // 会話関連(謎の女、受付嬢)
    //---------------------------------------------
	// 受付嬢の会話がどの状態なのかを管理
	enum ConvoMode
	{
		convo_mode_1, // クエストを見せる前
		convo_mode_2, // クエスト出発確認
		convo_mode_3, // 後ほど声をかけるように促す

		convo_mode_max
	};
	int m_convo_mode_num = convo_mode_1;

	// 誰が話しているのかを描画するためのもの
	struct TextName
	{
		//! 誰が話しているのかの名前
		const char* name;
		//! 描画したい座標
		Vector2 draw_pos;
	};

	TextName m_text_info[text_max]
	{
		{"Player", {0.0f,0.0f}},
		{"Player", {0.0f,0.0f}},
		{"謎の女", {0.0f,0.0f}},
		{"クエスト", {0.0f,0.0f}},

	};
	static constexpr int MANE_FONT_SIZE = 30;
	// テキストの描画座用
	Vector2 m_text_draw_pos;
	// 目印の描画座標
	Vector2 m_landmark_draw_pos;



};
