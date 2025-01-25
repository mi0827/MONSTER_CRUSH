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

	//! @brief 会話はじめの処理
	void Talk_Start();

	//! @brief 会話パートの更新処理
	void TalkUpdate();

	//! @breif クエストを受けているときの更新処理
	void AcceptingQuestUpdate();

	//! @brief 会話していない状態の描画処理
	void ModeNormalDraw();

	//! @brief 会話スタート時の描画処理
	void TalkStartDraw();

	//! @brief 会話パートの描画処理
	void ConvoDraw();

	//! @breif クエストを受けているときの描画処理
	void AcceptingQuestDraw();


private:
	// このシーンの状態を管理
	enum SceneNum
	{
		normal, // 会話をしていないとき
		talk_start, // 話始め
		receptionist_talk,  // 受付嬢が会話中
		accepting_quest, // クエスト受付中

		scene_max
	};
	// クエストシーンがどの状態化を補完する
	int scene_mode_num = normal;

	bool m_start_flag = false; // タイトル画面でボタンが押されたかどうかのフラグ

	//! カメラクラスのオブジェクト
	Camera camera;

	//! 受付嬢のオブジェクト
	Receptionist receptionist;
	//! 話せるエリア用のカプセル
	CapsuleCollision m_area;
	//! 話せるエリアに入ったか入っていないかのフラグ
	bool m_area_hit = false;

	//! プレイヤーが会話中かどうか
	bool m_talk_flag = false;

	
	
	
	//! 各テキストのフォントサイズ
	static constexpr int TEXT_FONT_SIZE = 60;
	////! 各テキストの何行目を呼んでいるのかを保存するための変数
	//int m_reception_text_line;
	//　クエストエリアでのテキストの数

	//-------------------------------------------------
	// プレイヤーに何をしたらいいかを促すテキスト関連
	//-------------------------------------------------
	enum PlayerText
	{
		palyer_story1,
		palyer_story2,
		palyer_story_max
	};
	//! クエストエリアのテキスト
	Text m_quest_area_text;
	//! テキストの何行目を見ているのかを保存する変数
	int m_quset_area_text_line; 



	//-------------------------------------------------
	// クエストメニュー関連
	//-------------------------------------------------
	// クエストテキストの数
	enum QuestText
	{
		quest_quest1, // クエスト１
		quest_quest2, // クエスト２
		quest_confirmatio1, // どのクエストがいいかを聞いてくる
		quest_yes, // 返答YES
		quest_no,  // 返答NO
		quest_confirmation2, // このクエストにしますか
		quest_max
	};
	//! クエストメニューテキスト
	Text m_quest_text;
	//! クエスト選択テキストの何行目かを保存するための
	int m_quest_text_line = 0;



	//-------------------------------------------------
	// 受付嬢会話関連
	//-------------------------------------------------
	// 受付嬢のテキストの数
	enum ReceptionText
	{
		recept_convo_text_1, // 受付嬢の感謝
		recept_yes, // 返答YES
		recept_no,  // 返答NO
		recept_convo_text_2, // 出発するのかの確認
		recept_convo_text_3, // ボットの存在を知らせてくれる
		recept_convo_text_4, // 返答でNOが返ってきたとき
		recept_teception_max
	};
	//! クエスト受付嬢のテキスト
	Text m_reception_text;
	//! 受付嬢のテキストの何行目かを保存するための
	int m_reception_text_line;



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
	enum TalkMode
	{
		quest_before_accepting, // クエストを受ける前
		quest_confirmation,     // クエスト出発確認
		quest_after_accepting,  // クエストを受けた後　(後ほど声をかけるように促す)

		talk_mode_max
	};
	int m_quest_acception_num = quest_before_accepting;

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
