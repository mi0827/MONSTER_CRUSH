#pragma once
// #include "Character_Base.h"


class GameScene :public Scene_Base
{
public:



	//! @brief コンストラクタ
	GameScene();
	//! @brief デストラクタ
	~GameScene();

	//! ゲームシーンにだけ使う初期処理
	void GameSceneInit();

	//! @brief 初期処理
	void Init()override;

	//! @brief 更新処理
	//! @param BGMの音量
	//! @param SEの音量
	void Update()override;

	// バトルの更新処理
	void GameUpdate();
	// バトル場面が終わったときの処理
	void EndUpdate();

	//! @brief  描画処理
	void Draw()override;

	//! @brief 終了処理
	void Exit()override;

	//! @brief ステータスバーの描画
	void StatusDraw();

	// 現時点で必要かわからない
	//// SEの初期化
	//void SE_Init();
	//// BGMの初期化
	//void BGM_Init();

	// ライトの初期化
	void LightInit();

	//! @brief フィールドとキャラクターとの当たり判定
	void HitField()override;

	//! @brief キャラクターの初期処理
	void Character_Init();

	//! @brief キャラクターの更新処理
	void CharacterUpdate();

	//! @brief キャラクターの攻撃に関しての更新処理
	void AttackUpdate();

	//! @brief 勝敗が付いたらそれを知らせるメッセージを描画するための関数
	void VDMessage();

	// タイマーの更新処理
	// @param 時間をカウントするためのもの
	//void Time_Update(int& time_count);
	// タイマーの描画
	//void Time_Draw();
	// チュートリアルでの描画処理
	// void Tutorial_Draw();
	// 準備完了を描画させるだけの関数
	//void Ready_Draw();
	// エンドでの描画処理
	//void End_Draw();
	// 攻撃のあたり判定を行う関数
	//void Attack_Hit(int player1, int player2);
	// ガードの当たり判定
	// void Block_Hit(int player1, int player2);
	// キャラクターのステータス描画処理
	// void Draw_Status();
	// ゲームシーン
	//enum play_scene
	//{
	//	Play_Tutorial, // チュートリアルシーン
	//	Play_Main,     // ゲームメインシーン
	//	Play_End,      // プレイENDシーン
	//	Play_Max       // プレイシーンの最大
	//};
	// SE
	//enum play_se
	//{
	//	READY, // チュートリアルの準備完了
	//	SE_MAX // SEの最大数
	//};
	// BGM
	//enum BGM
	//{
	//	TUTORIAL_BGM,  // チュートリアルのBGM
	//	BATTLE_1_BGM,  // プレイメインのBGM１
	//	BATTLE_2_BGM,  // プレイメインのBGM２
	//	BATTLE_3_BGM,  // プレイメインのBGM３
	//	BGM_MAX        // BGMの最大数
	//};
public:

	//! カメラクラスのオブジェクト
	Camera camera;

	//! プレイヤーキャラ用のオブジェクト
	CharacterBase* player;

	//! モンスター用のオブジェクト
	MonsterBase* monster;

	//! 攻撃アクションの当たり判定用のオブジェクト
	Attack player_attack_hit;
	Attack monster_attack_hit;

	// フィールドオブジェクト
	Field field;

	// ヒットストップ用のオブジェクト
	HitStop hit_stop;
	
	//=================
	// カメラの回転に関しての変数
	//! カメラの移動スピード
	static constexpr float CAMERA_ROT_SPEED = 2.0f;
	//! カメラの回転する方向
	static constexpr int CAMERA_DIRECTIN_RIGHT = 0; // 右回転
	static constexpr int CAMERA_DIRECTIN_FLET = 1;   // 左回転
	// どちらが死んだかをわかりやすく管理
	enum Ditection
	{
		player_die,     // プレイヤーが死んだとき , 勝ち
		monster_die,  // モンスターが死んだとき , 負け

		max,
	};
	// 誰が死んだかを保存するためのもの
	int m_who_died;


	//==========================
	// バトルシーンかそうでないかに関しての変数
	// どのシーンかをわかりやすく保存
	enum WhatScene
	{
		battle, // バトル場面
		result, // バトルが終わった後の場面
	};
	//! どのバトルシーンかを保存するためのもの
	int m_what_scene = battle; //!< 最初はバトルシーンから始める(今後変更予定)
	//! 一秒あたりのフレームの数
	static constexpr int ONE_SECOND_FLAME = 60;
	int m_count_flame = 0; // フレームカウント用の変数 
	int m_count_time = 0;  // フレームから実際の時間を割り出して入れる用の時間


	// バトルが終わってからどれくらいの時間で次のシーンに移動するかの時間
	static constexpr int CHANGE_TIME = 5;

	//================================
	//  バトルが終わった後に描画するメッセージに関する変数
	struct Message
	{
		//! メッセージを入れる変数
		const char* message;
	};
	Message m_massage[max]
	{
		{ "QUEST : FAILED"}, // 失敗
		{ "QUEST : CLEAR" } // クリア
		
	};
	//! メッセージの描画座標
	Vector2 m_massage_pos{ 0.0f,0.0f };


};