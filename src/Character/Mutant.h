#pragma once
#pragma once

//! @fule Mutant.h
//! @brief ミュータントのクラス
//! @brief モンスターベースクラスを継承

class Mutant :public MonsterBase
{
public:
	//=========================================================================
	// 定数の宣言
	//=========================================================================
	static constexpr float MONSTER_MOVE_SPEED = 1.5f; //! 移動スピード
	static constexpr float MONSTER_ROT_SPEED = 0.5f;   //! 回転スピード
	static constexpr float TARGET_DISTANCE = 100.0f;     //! ジャンプするときの相手との距離
	static constexpr float JUMP_UP_SPEED = 10.0f;          //! ジャンプの時の上昇スピード
	static constexpr float JUMP_DOWN_SPEED = 2.0f;      //! ジャンプの時の下に落ちてほしいスピード 
	static constexpr int HP_MAX = 500;                           //! HPの最大値
	static constexpr int ROLLING_SPEED = 1.0f;               //! ローリングスピード
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
		attack_2, //!< 攻撃２
		attack_3, //!< 攻撃３
		attack_4, //!< 攻撃４
		rolling,    //!< ローリング
		jump,     //!< ジャンプアクション

		anim_max //!< アニメーションの最大数
	};

	//! 攻撃アニメーションの一番最初
	static constexpr int ATTACK_ANIM_START = attack_1;
	//! 攻撃アニメーション最大値（jumpを抜いた分）
	static constexpr int ATTACK_ANIM_MAX = jump - ATTACK_ANIM_START;

	//! 攻撃アクションの数
	static constexpr int ATTACK_ACTION_MAX = anim_max - attack_1;
	
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
	int jump_num=0;
public:

	//=========================================================================
	// 関数の宣言
	//=========================================================================
	//! @brief コンストラクタ
	Mutant();
	//! @brief デストラクタ
	~Mutant();

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
	void Status_Bar_Init() override;
	//! @brief ステータスバーの描画関数
	void Status_Bar_Draw() override;

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
	

	

	

public:
	//-----------------------------------------------
	// クラスのオブジェクトの宣言
	//-----------------------------------------------

	////! モデルクラスのオブジェクト
	//Model m_model;
	////! アニメーションクラスのオブジェクト
	//Animation m_animation;
	////! コンボクラス
	//Combo m_combo;
	////! カプセルコリジョン
	////=================
	//// 当たり判定
	////=================
	//CapsuleCollision m_left_hand; //!< 左手のあたり判定
	//CapsuleCollision m_right_hand; //!< 右手の当たり判定
	//CapsuleCollision m_body;       //!< 本体のあたり判定

	//==================================
	// 攻撃時の当たり判定とダメージの設定当たり判定
	//==================================
	// このカプセルの当たり判定を保存することがうまくできない
	Attack_Hit_Damage attack_hit_damage[ATTACK_ACTION_MAX];

	//=================
	// バー型のUI
	//=================
	UIBra m_hp;
	
};
