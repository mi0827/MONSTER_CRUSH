#pragma once
// #include "Character_Base.h"

//! @file GameScene.h
//! @brief メインのゲームシーンクラス
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
	void Update()override;

	//! @brief モンスターの登場演出
	void EntryUpdate();

	//! @brief バトルの更新処理
	void GameUpdate();
	//! @brief バトル場面が終わったときの処理
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

	//! @brief オプションメニューでもらってきた値を反映する関数
	//! //! @param BGMの音量
	//! @param SEの音量
	//! @param マウス感度
	void OptionValuesReflect(int bgm, int se, int mouse)override;

	//! @brief キャラクターの初期処理
	void CharacterInit();

	//! @brief キャラクターの更新処理
	void CharacterUpdate();

	//! @brief キャラクターの攻撃に関しての更新処理
	void AttackUpdate();

	//! @brief 勝敗が付いたらそれを知らせるメッセージを描画するための関数
	void VDMessage();

	////! @brief モンスターの咆哮攻撃の際にプレイヤーを遠ざけるための処理
	////! @param モンスターの咆哮中華のフラグ
	////! @param モンスターの座標と向き
	////! @param 遠ざけたいキャラの向きと座標
	////! @param キャラクターの移動スピード
	//void CharacterKeepAway(bool moster_roar_flag, Transform monster,Transform character,float character_move_speed);

	//! @brief モンスターの咆哮攻撃の際にプレイヤーを遠ざけるための処理
	void CharacterKeepAway();

public:

	//! カメラクラスのオブジェクト
	Camera camera;

	//! プレイヤーキャラ用のオブジェクト
	// CharacterBase* player;

	//! モンスター用のオブジェクト
	MonsterBase* monster;

	//// フィールドオブジェクト
	//Field field;

	// ヒットストップ用のオブジェクト
	HitStop hit_stop;
	
	//=================
	// カメラに関しての変数
	//! カメラの移動スピード
	
	//! ターゲットカメラを使用するかマウスでカメラを操作するかの判断用変数
	bool m_camera_change = true;

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
		entry, // 登場演出(モンスター)
		battle, // バトル場面
		power_up, // パワーアップ(モンスター)
		result, // バトルが終わった後の場面
	};
	//! どのバトルシーンかを保存するためのもの
	int m_what_scene = entry; //!< 最初はバトルシーンから始める(今後変更予定)
	//! 一秒あたりのフレームの数
	static constexpr int ONE_SECOND_FLAME = 60;
	int m_count_flame = 0; // フレームカウント用の変数 
	int m_count_time = 0;  // フレームから実際の時間を割り出して入れる用の時間


	// バトルが終わってからどれくらいの時間で次のシーンに移動するかの時間
	static constexpr int CHANGE_TIME = 3;

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