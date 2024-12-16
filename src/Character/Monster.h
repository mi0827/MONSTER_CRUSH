#pragma once
#pragma once

//! @file SamplePlayer.h
//! @brief モンスターのクラス
//! @brief モンスターベースクラスを継承

class Monster :public MonsterBase
{
public:
	//=========================================================================
	// 定数の宣言
	//=========================================================================
	static constexpr float MONSTER_MOVE_SPEED = 1.5f;   //! 移動スピード
	static constexpr float MONSTER_ROT_SPEED = 0.5f;    //! 回転スピード
	static constexpr float TARGET_DISTANCE = 170.0f;    //! ターゲットとの距離
	static constexpr float JUMP_UP_SPEED = 5.0f;        //! ジャンプの時の上昇スピード
	static constexpr float JUMP_DOWN_SPEED = 2.0f;      //! ジャンプの時の下に落ちてほしいスピード 
	static constexpr int HP_MAX = 500;                  //! HPの最大値
	//=========================================================================
	// 列挙体の宣言
	//=========================================================================
public:
	//! アニメーションの種類用の列挙体
	//! @detail 攻撃アニメーションはコンボの続けたい順番に設定する
	enum Animation_Max
	{
		idle, //!< 待機
		run,  //!< 走り
		attack_1, //!< 攻撃１
		rolling,  //!< ローリング
		jump,     //!< ジャンプアクション

		anim_max //!< アニメーションの最大数
	};

	//! 攻撃アニメーションの一番最初
	static constexpr int ATTACK_ANIM_START = attack_1;
	//! 攻撃アニメーション最大値
	static constexpr int ATTACK_ANIM_MAX = jump - ATTACK_ANIM_START;


	//! モンスターの状態
	enum MonsterMode
	{
		IDLE,      //!< 待機
		RUN,       //!< 走り状態
		ATTACK,    //!< 攻撃
	};

	enum Jump
	{
		STANDBY,     //! スタンバイ
		GOUP,           //! 上がるタイミング
		MOVE,           //! ジャンプ中の移動のタイミング
		DROPDOWN,  //! 落ちてくるタイミング

		max
	};
	//! ジャンプの状態を保存する変数
	int jump_num = 0;
public:

	//=========================================================================
	// 関数の宣言
	//=========================================================================
	
	//! @brief コンストラクタ
	Monster();
	//! @brief デストラクタ
	~Monster();

	//! 継承先からの関数のオーバーライド
	//! @brief 初期処理
	void Init()override;
	//! @brief 更新処理
	//! @param 移動の時のターゲットの座標
	//! @param ターゲットの半径
	void Update(Transform* traget_pos, float target_r) override;
	//! @brief 描画処理
	void Draw() override;
	//! @brief 終了処理
	void Exit() override;


	//! @brief あたり判定の更新処理
	void CDUpdate() override;

	//! @brief ステータスバーの設定用関数
	void StatusBarInit() override;
	//! @brief ステータスバーの描画関数
	void StatusBarDraw() override;

	//! @brief アニメーション読み込み関数
	void Anima_Load_Init();

	//! @brief プレイヤーの移動用関数
	void Move_Update();

	//! @brief 攻撃開始かそうでないかを判断する関数
	void Attack_First();

	//! @brief 攻撃用の関数
	void Attack_Update();

	//! @brief ジャンプ攻撃用の関数
	void Attack_Jump();

	//! @brief ジャンプ攻撃の更新処理
	void Jump_Update();

	//! @brief コンボ関数
	void Combo_Update();

	//! @drief 行いたいアニメーションをランダムで選ぶための関数
	int Set_Rand_Attack();

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
	int m_monster_mode = -1;
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

	//! 動いてほしいかのフラグ

	//! 次のアニメーション番号を保存
	int m_next_anim = -1;
	//! コンボをやめてほしい時のフラグ
	bool m_stop_combo_flag = false;



public:



	//=================
	// バー型のUI
	//=================
	UIBra m_hp;
	// HPの残量
	int m_hp_value = HP_MAX;
};
