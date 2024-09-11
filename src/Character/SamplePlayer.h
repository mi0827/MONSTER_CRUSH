#pragma once

//! @fule SamplePlayer.h
//! @brief サンプルキャラのクラス
//! @brief キャラクターベースクラスを継承

class SamplePlayer :public CharacterBase
{
public:
	//-----------------------------------------------
	// 定数の宣言
	//-----------------------------------------------
	static constexpr float PLAYER_MOVE_SPEED = 2.0f;       //! プレイヤーの移動スピード
	static constexpr float PLAYER_R = 5.0f;                         //! プレイヤーの回転スピード
	static constexpr float PLAYER_ROLLING_SPEED = 1.1f;   //! プレイヤーのローリングスピード 
	static constexpr int HP_MAX = 100;                               //! HPの最大値
	static constexpr float PLAYER_HIT_R = 1.5;                         //! プレイヤーの円の当たり半径の半径
	//----------------------------------------------- 
	// 列挙体の宣言
	//-----------------------------------------------
public:
	//! アニメーションの種類用の列挙体
	//! @detail 攻撃アニメーションはコンボの続けたい順番に設定する
	enum Animation_Max
	{
		idle, //!< 待機
		run, //!< 走り
		rolling, //!< ローリング
		attack_anim_1,      //< 攻撃１
		attack_anim_2,      //< 攻撃２
		attack_anim_3,      //< 攻撃３
		attack_kick_anim_1, //< キック攻撃１
		attack_kick_anim_2, //< キック攻撃２
		attack_kick_anim_3, //< キック攻撃３

		anim_max //!< アニメーションの最大数
	};

	static constexpr int NORMAL_ACTION = 3;                     //! アニメーションの攻撃系以外のアニメーションの数
	static constexpr int ATTACK_ACTION = anim_max - NORMAL_ACTION; //! 攻撃アニメーションの最大数

	// 攻撃番号の再設定
	enum AttackAnim
	{
		attack_punch_1 = attack_anim_1 - NORMAL_ACTION,    //< 攻撃１
		attack_punch_2 = attack_anim_2 - NORMAL_ACTION,	   //< 攻撃２
		attack_punch_3 = attack_anim_3 - NORMAL_ACTION,	   //< 攻撃３
		attack_kick_1 = attack_kick_anim_1 - NORMAL_ACTION, //< キック攻撃１
		attack_kick_2 = attack_kick_anim_2 - NORMAL_ACTION,	//< キック攻撃２
		attack_kick_3 = attack_kick_anim_3 - NORMAL_ACTION,	//< キック攻撃３

		attack_max
	};

	// 当たり判定をとってほしいフレームの構造体
	struct AttackFrame
	{
		// 始まりのフレーム
		int start_frame;
		// 終わりのフレーム
		int end_frame;
	};
	AttackFrame attack_frame[attack_max] =
	{
		// パンチ１
		{ 0.0f, 0.0f, },
		// パンチ２
		{ 0.0f, 0.0f, },
		// パンチ３
		{	0.0f, 0.0f, },
		// キック１
		{	0.0f,	0.0f,	},
		// キック２
		{	0.0f, 0.0f, },
		// キック３
		{	0.0f,	0.0f, },
	};



	//! プレイヤーの状態
	enum PlayerMode
	{
		IDLE,      // 待機
		RUN,       // 走り状態
		ROLLING, // 回避
		ATTACK,    // 攻撃
	};


public:

	//-----------------------------------------------
	// 関数の宣言
	//-----------------------------------------------
	//! @brief コンストラクタ
	SamplePlayer();
	//! @brief デストラクタ
	~SamplePlayer();

	//! 継承先からの関数のオーバーライド
	//! @brief 初期処理
	void Init()override;
	//! @brief 更新処理
	//! @param カメラの向き
	void Update(Vector3* camera_rot) override;
	//! @brief 描画処理
	void Draw() override;
	//! @brief 終了処理
	void Exit() override;

	//! @brief あたり判定の更新処理
	void CDUpdate() override;

	//! @brief ステータスバーの設定用関数
	void Status_Bar_Init() override;
	//! @brief ステータスバーの描画関数
	void Status_Bar_Draw() override;

	//! @brief 当たり判定を行って欲しいタイミングを保存する関数
	void SetHitTimeInit() override;

	//! @brief アニメーション読み込み関数
	void Anima_Load_Init();

	//! @brief プレイヤーの移動用関数
	void Move_Update(Vector3* camera_rot);

	//! @brief プレイヤーの状態管理関数
	//! @param プレイヤーの状態
	void Player_Mode(int mode);

	//! @brief 攻撃開始かそうでないかを判断する関数
	void Attack_First();

	//! @brief 攻撃用の関数
	void Attack_Update();

	//! @brief ローリングセット用の関数
	void Set_Rolling();

	//! @brief ローリングアクション用の関数（回避）
	void Action_Rolling();

	//! @brief コンボ関数
	void Combo_Update();

public:


	//------------------------------------------
	// コンボ関連
	//------------------------------------------
	// コンボの最大数
	static constexpr int COMBO_MAX = 3;
	//! コンボ用フラグ
	bool m_combo_flag = false;
	// コンボの際マウスのどちらを押されたか
	int m_mouse_flag = 0; // MOUSE_INPUT_RIGHT : 右ボタン、MOUSE_INPUT_LEFT : 左ボタン 
	// コンボが何回続いているかのカウント
	int m_combo_count = 0;
	// 次のアニメーション番号を保存
	int m_next_anim = -1;
	// コンボをやめてほしい時のフラグ
	bool m_stop_combo_flag = false;

public:

	////=================
	//// バー型のUI
	////=================
	UIBra m_hp;

	//=================
	// ステータス
	//=================

	// 防御力(とりあえず20)
	static constexpr int M_DEFENSE = 20;

};
