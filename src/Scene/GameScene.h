#pragma once
// #include "Character_Base.h"


class GameScene :public Scene_Base
{
public:

	


	GameScene();
	 ~GameScene();

	 //! ゲームシーンにだけ使う初期処理
	 void GameSceneInit();

	

	 //! @brief 初期処理
	void Init()override;

	//! @brief 更新処理
	//! @param BGMの音量
	//! @param SEの音量
	void Update(int bgm_volume, int se_volume)override;

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
	void HitField();
	
	//! @brief キャラクターの初期処理
	void Character_Init();

	//! @brief キャラクターの更新処理
	// @ param SEの音量
	void CharacterUpdate(int se_volume);

	//! @brief キャラクターの攻撃に関しての更新処理
	void AttackUpdate();

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
	// このクラスをただの関数でもよかったけど中にbool 型の変数が一つあるのでクラスにした
	Attack player_attack_hit;
	Attack monster_attack_hit;

	// フィールドオブジェクト
	Field field;
	
};