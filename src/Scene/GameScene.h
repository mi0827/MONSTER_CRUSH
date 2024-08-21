#pragma once
// #include "Character_Base.h"


class GameScene :public Scene_Base
{
public:
	GameScene();
	// ~GameScene();
	// 初期処理
	void Init()override;

	// 更新処理
	void Update(int bgm_volume, int se_volume)override;

	// 描画処理
	void Draw()override;

	// 終了処理
	void Exit()override;

	// SEの初期化
	void SE_Init();
	// BGMの初期化
	void BGM_Init();

	// ライトの初期化
	void Light_Init();
	// ライトの更新処理
	void Light_Update();

	// チュートリアルの更新処理
	void Tutorial_Update();
	// メインプレイシーンの更新処理
	void PlayMain_Update();
	// プレイエンドの更新処理
	void PlayEnd_Update();
	// エンドシーンでの勝敗に関する関数
	void Play_Victory_Draw(CharacterBase* character1, CharacterBase* character2); // 勝者を決める関数

	// 描画用の文字列のサイズ調整
	// 引数
	// １：横の大きさ
	// ２：縦の大きさ
	// ３：文字列
	void Draw_String_Size(float* w, float* h, const char* sting);

	// キャラクターの更新処理
	void Character_Update(int se_volume);
	// タイマーの更新処理
	void Time_Update(int& time_count);
	// タイマーの描画
	void Time_Draw();
	// チュートリアルでの描画処理
	void Tutorial_Draw();
	// 準備完了を描画させるだけの関数
	void Ready_Draw();
	// エンドでの描画処理
	void End_Draw();

	// 攻撃のあたり判定を行う関数
	void Attack_Hit(int player1, int player2);
	// ガードの当たり判定
	void Block_Hit(int player1, int player2);
	// キャラクターのステータス描画処理
	void Draw_Status();

	// ゲームシーン
	enum play_scene
	{
		Play_Tutorial, // チュートリアルシーン
		Play_Main,     // ゲームメインシーン
		Play_End,      // プレイENDシーン

		Play_Max       // プレイシーンの最大
	};

	// SE
	enum play_se
	{
		READY, // チュートリアルの準備完了

		SE_MAX // SEの最大数
	};

	// BGM
	enum BGM
	{
		TUTORIAL_BGM,  // チュートリアルのBGM
		BATTLE_1_BGM,  // プレイメインのBGM１
		BATTLE_2_BGM,  // プレイメインのBGM２
		BATTLE_3_BGM,  // プレイメインのBGM３
		BGM_MAX        // BGMの最大数
	};

private:

	int m_time_count = 0;     // フレーム数を現実の時間で計算する用の変数
	int m_flame_count = 0;    // フレーム数をカウントをする変数
	int m_play_scene = 0;     // チュートリアルの途中かを判別
	int m_end_count = 0;      // エンドシーンの時間
	bool m_status_flag = false;   // ステータス更新処理していいかのフラグ

	//-------------------------------------------
	// チュートリアルで使う変数
	//-------------------------------------------
	const int BUTTON_COUNT_MAX = 180; // ３秒
	// 準備完了フラグ
	bool m_ready_flag1 = false;
	bool m_ready_flag2 = false;
	// ボタンの長押しをカウントする用の変数
	float m_button_count1 = 0;
	int m_button_count2 = 0;

	// ヒットストップの判断用変数
	bool m_stop = false;

	// ライト用の変数
	static const int light_MAX = 1;
	int m_light_handle[light_MAX] = {0}; // ライトハンドル
	COLOR_F m_original_dif_color = { 0,0,0 };
	COLOR_F m_original_spc_color = { 0,0,0 };
	COLOR_F m_original_amb_color = { 0,0,0 };
	Vector3 m_original_light_rot = { 0,0,0 }; // 初期ライトの向き
	Vector3 m_light_rot = { 0.0f,0.0f,0.0f }; // ライトの回転
	Vector3 m_light_pos = { 0.0f,0.0f,0.0f }; // ライトのポジション

	static const int FIGHT_START_COUNT_MAX = 300; // 戦いが始まるまでの準備時間
	bool m_fight_start_flag = false; // 戦いが始まる用のフラグ
	int m_fight_start_count = 0; // レディーされから戦いが始まるまでのカウント

	int m_draw_fight_pos_x = 0; // Lady,Fightの描画座標用変数
	//int shadowmap_handle; // シャドーマっプ
};