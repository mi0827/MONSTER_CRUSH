#pragma once

//! @fule HeroPlayer.h
//! @brief ヒーローのクラス
//! @brief キャラクターベースクラスを継承

class Hero :public CharacterBase
{
public:
	//-----------------------------------------------
	// 定数の宣言
	//-----------------------------------------------
	static constexpr float  HERO_MOVE_SPEED = 2.0f; //! 移動スピード
	static constexpr float  HERO_R = 5.0f;                   //! 回転スピード
	static constexpr float PLAYER_ROLLING_SPEED = 1.1f;   //! プレイヤーのローリングスピード 
	static constexpr int HP_MAX = 100;                               //! HPの最大値
	static constexpr int PLAYER_HIT_R = 1;                         //! プレイヤーの円の当たり半径の半径
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
		attack_1,      //< 攻撃１
		attack_2,      //< 攻撃２
		attack_3,      //< 攻撃３

		attack_kick_1, //< キック攻撃２
		attack_kick_2, //< キック攻撃３
		attack_4,      //< 攻撃4

		anim_max //!< アニメーションの最大数
	};

	//! ヒーローの状態
	enum HeroMode
	{
		IDLE,      // 待機
		RUN,       // 走り状態
		ATTACK,    // 攻撃
	};


public:

	//-----------------------------------------------
	// 関数の宣言
	//-----------------------------------------------
	//! @brief コンストラクタ
	Hero();
	//! @brief デストラクタ
	~Hero();

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

	//! @brief ステータスバーの描画関数
	void StatusBarDraw() override;

	//! @brief アニメーション読み込み関数
	void Anima_Load_Init();

	//! @brief プレイヤーの移動用関数
	void Move_Update(Vector3* camera_rot);

	//! @brief プレイヤーの状態管理関数
	//! @param プレイヤーの状態
	void Hero_Mode(int mode);

	//! 攻撃開始かそうでないかを判断する関数
	void Attack_First();

	//! @brief 攻撃用の関数
	void Attack_Update();

	//! @brief コンボ関数
	void Combo_Update();

public:
	//-----------------------------------------------
	// 変数の宣言
	//-----------------------------------------------
	//! アイドル状態かのフラグ
	bool m_idle_flag = false;
	//! 走っていい以下のフラグ
	bool m_run_flag = false;

	//! 攻撃状態かどおかのフラグ
	bool m_attack_flag = false;
	//! プレイヤーのモードを管理する変数
	int m_hero_mode = -1;
	//! 今のアニメーション番号を保存する用の変数
	int m_now_attack_anim = -1;


	//! 壁擦り判定のためにいったん座標を保存しておく変数
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
	//! 移動の際の当たり判定用のサイズ
	Vector3 move_hit_size = { 1.0f ,0.0f,1.0f };

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
	//-----------------------------------------------
	// クラスのオブジェクトの宣言
	//-----------------------------------------------

	//! モデルクラスのオブジェクト
	Model m_model;
	//! アニメーションクラスのオブジェクト
	Animation m_animation;
	//! コンボクラス
	Combo m_combo;
	//! カプセルコリジョン
	//=================
	// 当たり判定
	//=================
	CapsuleCollision m_right_hand; //!< 右手のあたり判定
	CapsuleCollision m_left_hand; //!< 右手の当たり判定
	CapsuleCollision m_left_feet;  //!< 左足の当たり判定
	CapsuleCollision m_right_feet; //!< 右足の当たり判定
	CapsuleCollision m_sword;     //!< ソードの当たり判定
	CapsuleCollision m_body;       //!< 本体のあたり判定
};
