


//! @brief モンスターのクラス
//! @brief モンスターベースクラスを継承
#pragma once

//! @file Monster.h
//! @brief モンスターのクラス
//! @brief モンスターベースクラスを継承
class Monster :public MonsterBase
{
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
	//! @param ターゲットのbodyのカプセル（当たり判定）
	//! @param カメラオブジェクト
	void Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera) override;

	//! @brief 生きてる時の更新処理
	//! @param ターゲットの座標
	//! @param ターゲットの半径
	//! @param カメラオブジェクト
	void LiveUpdate(Transform* target_pos, float target_r, Camera* camera) override;

	//! @brief 死んだときの更新処理 
	void DieUpdate() override;
	//! @brief 描画処理
	void Draw() override;
	//! @brief 終了処理
	void Exit() override;

	//! @brief 登場演出用の更新処理
	void EntryUpdate() override;

	//! @brief HPが一定まで減ったときのレベルアップ処理
	void ReinforceUpdate() override;

	//! @brief 攻撃を受けた時の更新処理
	void ComeAttackUpdate() override;

	//! @brief あたり判定の更新処理
	void CDUpdate() override;

	//! @brief ステータスバーの設定用関数
	void StatusBarInit() override;
	//! @brief ステータスバーの描画関数
	void StatusBarDraw() override;

	//! @brief 当たり判定を行って欲しいタイミングを保存する関数
	void SetAttackInfo() override;

	//! @brief モンスターの状態(フラグ)管理関数
	//! @param モンスターの状態
	void MonsterMode(int mode) override;

	//! @brief アニメーション読み込み関数
	void AnimLoadInit() override;

	//! @brief エフェクトの読み込みをまとめる関数
	void EffectLoadInit() override;
	//! @brief エフェクトの更新処理
	//! @param 行いたいエフェクト番号
	//! @param 行いたいエフェクトの情報番号
	void EffectUpdate(int effect_num, int effect_info_num) override;
	//! @brief SEの読み込み
	void SELoadInit() override;
	//! @brief SEの更新処理
	//! @param 行いたいSEの情報が保管されている番号
	void SEUpdate(int se_num) override;

	//=========================================================================
	// 定数の宣言
	//=========================================================================
	static constexpr float MONSTER_MOVE_SPEED = 1.5f;        //! 移動スピード
	static constexpr float MONSTER_ROT_SPEED = 0.5f;         //! 回転スピード

	static constexpr float CHANGE_JUMP_RUNNIG_FRAME = 150;   //! Runからジャンプ,ローリングに移るのに何フレームかかるか

	static constexpr float JUMP_TARGET_DISTANCE = 25.0f;     //! ジャンプするとき相手と離れててほしい最低限の距離
	static constexpr float JUMP_MOV_SPEED = 1.0f;            //! ジャンプ中の移動スピード
	static constexpr float JUMP_DEVIATION_POS = 26.0f;       //! ジャンプ攻撃をしたときにズレを直す値
	static constexpr float JUMP_STRAT_FRAME = 34.0f;         //! ジャンプの移動開始のフレーム
	static constexpr float JUMP_END_FRAME = 90.0f;           //! ジャンプの移動終了のフレーム

	static constexpr int ROLLING_SPEED = 1;             //! ローリングスピード
	static constexpr float ROLLING_STRAT_FRAME = 0.0f;       //! ローリングの移動開始フレーム
	static constexpr float ROLLING_END_FRAME = 70.0f;        //! ローリングの移動終了フレーム
	static constexpr float ROLLING_TARGET_DISTANCE = 15.0f;  //! ローリングするとき相手と離れててほしい最低限の距離

	static constexpr int HP_VALUE_MAX = 500;                 //! HPの最大値
	static constexpr int STUN_VALUE_MAX = 150;               //! スタン値の最大

	//=========================================================================
	// 列挙体の宣言
	//=========================================================================
	//! アニメーションの種類用の列挙体
	//! @detail 攻撃アニメーションはコンボの続けたい順番に設定する
	enum Animation_Max
	{
		idle_anim, //!< 待機
		run_anim,  //!< 走り
		die_anim,   //!< 死亡
		shout_anim, //!< 叫び
		hit_damage_anim, //!< 攻撃を受けた時
		stun_down_anim, //!< スタンで倒れる時
		stun_up_anim,  //!< スタンで起き上がるとき

		punch_attack_1_anim, //!< パンチ攻撃１
		sword_attack_1_anim, //!< ソード攻撃１
		sword_attack_2_anim, //!< ソード攻撃２
		sword_attack_3_anim, //!< ソード攻撃３
		sword_attack_4_anim, //!< ソード攻撃４
		sword_attack_5_anim, //!< ソード攻撃５
		rolling_anim,  //!< ローリング
		jump_anim,     //!< ジャンプアクション

		anim_max //!< アニメーションの最大数
	};

	//! 攻撃アニメーションの一番最初
	static constexpr int ATTACK_ANIM_START = punch_attack_1_anim;
	//! 攻撃アニメーション最大値（jumpを抜いた分ジャンプを抜いておかないとコンボ攻撃の時なバグる）
	static constexpr int ATTACK_ANIM_MAX = rolling_anim - ATTACK_ANIM_START;

	//! 攻撃アクションの数
	static constexpr int ATTACK_ACTION_MAX = anim_max - punch_attack_1_anim;

	// 攻撃番号の再設定
	enum AttackAnim
	{
		attack_end = -1, //< コンボ攻撃の終わり
		attack_punch_1 = punch_attack_1_anim - ATTACK_ANIM_START,  //< 攻撃１
		attack_sword_1 = sword_attack_1_anim - ATTACK_ANIM_START,  //< 攻撃２
		attack_sword_2 = sword_attack_2_anim - ATTACK_ANIM_START,  //< 攻撃３
		attack_sword_3 = sword_attack_3_anim - ATTACK_ANIM_START,  //< 攻撃４
		attack_sword_4 = sword_attack_4_anim - ATTACK_ANIM_START,  //< 攻撃５
		attack_sword_5 = sword_attack_5_anim - ATTACK_ANIM_START,  //< 攻撃６
		attack_rolling = rolling_anim - ATTACK_ANIM_START,   //< ローリング攻撃
		attack_jump = jump_anim - ATTACK_ANIM_START,         //< ジャンプ攻撃

		attack_max
	};

	//! コンボパターンの数
	static const int M_COMBO_PATTERN_MAX = 6;
	//! 各パターンのコンボの最大数
	static const int M_COMBO_NUM_MAX = 4;
	//! 各コンボパターンにコンボに使用する攻撃番号を保存しコンボを作成する
	//! コンボの最後にはattack_endを入れること
	int m_combo_pattern[M_COMBO_PATTERN_MAX][M_COMBO_NUM_MAX]
	{
	  {attack_sword_3,attack_sword_1,attack_end,attack_end},
	  {attack_punch_1,attack_sword_5,attack_end,attack_end},
	  {attack_sword_3,attack_sword_2,attack_end,attack_end},
	  {attack_sword_4,attack_sword_1,attack_punch_1,attack_end},
	  {attack_sword_5,attack_punch_1,attack_sword_3,attack_end},
	  {attack_punch_1,attack_sword_3,attack_sword_2,attack_end},
	};
	// 各コンボの後隙
	int m_combo_rear_crevice_frame[M_COMBO_PATTERN_MAX]
	{
		{30},{30},{20},{20},{20},{15}
	};

	// 当たり判定をとってほしいフレームの構造体
	struct AttackFrame
	{
		// 始まりのフレーム
		float start_frame;
		// 終わりのフレーム
		float end_frame;
	};

	// 当たり判定を行ってほしいタイミングの設定の変数
	// まだまだ未完成
	// 作品展までに完成させる
	AttackFrame attack_frame[attack_max] =
	{
		// パンチ１
		{ 13.0f, 30.0f, },
		// ソード１
		{ 40.0f, 90.0f, },
		// ソード２
		{ 30.0f, 90.0f, },
		// ソード３
		{ 25.0f, 80.0f, },
		// ソード４
		{10.0f,30.0f},
		// ソード５
		{45.0f,75.0f},
		// ローリング
		{ 10.0f, 50.0f, },
		// ジャンプ
		{ 15.0f, 105.0f, },

	};

	//! 各攻撃のダメージ
	int m_attack_damage[attack_max]
	{
		20,70,50,40,20,60,50,50,
	};

	//! エフェクトの種類用の列挙体
	enum Effect
	{
		sword_attack_effect, // 剣での攻撃時のエフェクト
		punch_attack_effect, // パンチ攻撃時のエフェクト
		damage_effect, // ダメージを受けた時のエフェクト
		roar_effect,      // 咆哮時のエフェクト
		effect_max
	};

	// エフェクトをつけたいアニメーションの種類を列挙体で管理
	enum EffectInfoNum
	{
		// 攻撃番号と合わせたいから攻撃に合うように攻撃から設定
		attack_punch_1_effect_info, // パンチ攻撃１
		attack_sword_1_effect_info, // ソード攻撃１
		attack_sowrd_2_effect_info, // ソード攻撃２
		attack_sowrd_3_effect_info, // ソード攻撃３
		attack_sowrd_4_effect_info, // ソード攻撃４
		attack_sowrd_5_effect_info, // ソード攻撃５

		// ここからは攻撃とは別のエフェクト
		damage_effect_info,     // 攻撃を受けた時のエフェクト
		roar_effect_info,       // 咆哮時のエフェクト  

		effect_info_max
	};
	struct EffectInfo
	{
		// エフェクトのスケール
		Vector3 size;
		// エフェクトの座標(キャラの座標からずらす分の座標)
		Vector3 pos;
		// エフェクトの向き
		Vector3 rot;

		// 下の二つはなくてもいいかも
		// エフェクトの再生を開始させたいときのアニメーションフレーム
		float effect_start_anim_frame;
	};
	EffectInfo m_effect_info[effect_info_max]
	{
		// パンチ攻撃１
		{ {4.0f,4.0f,4.0f},{15.0f,14.0f,15.0f},{90.0f,180.0f,0.0f}, 5},
		// ソード攻撃１
		{ {6.0f,6.0f,6.0f},{20.0f,18.0f,20.0f},{35.0f,-90.0f,0.0f}, 60},
		// ソード攻撃２
		{ {6.0f,6.0f,6.0f},{20.0f,25.0f,20.0f},{155.0f,-90.0f,0.0f}, 40},
		// ソード攻撃３
		{ {6.0f,6.0f,6.0f},{15.0f,20.0f,15.0f},{35.0f,-90.0f,0.0f}, 30},
		// ソード攻撃４
		{ {5.0f,5.0f,5.0f},{15.0f,24.0f,15.0f},{85.0f,-90.0f,0.0f}, 1},
		// ソード攻撃５
		{ {6.0f,6.0f,6.0f},{15.0f,20.0f,15.0f},{90.0f,-90.0f,10.0f}, 30},
		// ダメージを受けた時のエフェクト
		{ {1.0f,1.0f,1.0f},{0.0f,15.0f,0.0f},{0.0f,0.0f,0.0f}, 1},
		// 咆哮時のエフェクト  
		{ {1.0f,1.0f,1.0f},{5.0f,12.0f,5.0f},{0.0f,0.0f,0.0f}, 1},

	};


	// SEの種類用の列挙体
	enum SE
	{
		punch_attack_se,    // パンチ攻撃
		sword_attack_se_1, // 剣攻撃１
		sword_attack_se_2, // 剣攻撃２
		damage_se,            // ダメージを受けた時
		roar_se,                  // 咆哮時
		run_se,                   // 足音
		se_max
	};

	// SEををつけたいものの情報の列挙体
	enum SEInfoNum
	{
		// 攻撃番号と合わせたいから攻撃に合うように攻撃から設定
		attack_punch_1_se_info, // パンチ攻撃１
		attack_sword_1_se_info, // ソード攻撃１
		attack_sowrd_2_se_info, // ソード攻撃２
		attack_sowrd_3_se_info, // ソード攻撃３
		attack_sowrd_4_se_info, // ソード攻撃４
		attack_sowrd_5_se_info, // ソード攻撃５

		damage_se_info,     // 攻撃を受けた時
		roar_se_info,       // 咆哮時  
		run_se_info,        // 足音
		se_info_max
	};

	// 各SEの情報
	struct SEInfo
	{
		// 再生したいSE番号
		int se_num;
		// 再生するタイプ
		int play_type;
		// ループするかどうか
		bool loop;
		// 再生するタイミング
		int se_start_frame;
	};
	SEInfo m_se_info[se_info_max]
	{
		// 攻撃番号と合わせたいので攻撃のエフェクト情報から先に調べる
		// キック1
		{ punch_attack_se, DX_PLAYTYPE_BACK, true, 1},
		// 剣攻撃１
		{ sword_attack_se_1, DX_PLAYTYPE_BACK, true, 10},
		// 剣攻撃２
		{ sword_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// 剣攻撃３
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// 剣攻撃4
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// 剣攻撃5
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},


		// 攻撃を受けた時
		{damage_se,DX_PLAYTYPE_BACK, true, 1},
		// 咆哮
		{roar_se,DX_PLAYTYPE_BACK, true, 1},
		// 足音
		{run_se, DX_PLAYTYPE_LOOP, true, 1},
	};
};

//class Monster :public MonsterBase
//{
//public:
//	//=========================================================================
//	// 定数の宣言
//	//=========================================================================
//	static constexpr float MONSTER_MOVE_SPEED = 1.5f;   //! 移動スピード
//	static constexpr float MONSTER_ROT_SPEED = 0.5f;    //! 回転スピード
//	static constexpr float TARGET_DISTANCE = 170.0f;    //! ターゲットとの距離
//	static constexpr float JUMP_UP_SPEED = 5.0f;        //! ジャンプの時の上昇スピード
//	static constexpr float JUMP_DOWN_SPEED = 2.0f;      //! ジャンプの時の下に落ちてほしいスピード 
//	static constexpr int HP_MAX = 500;                  //! HPの最大値
//	//=========================================================================
//	// 列挙体の宣言
//	//=========================================================================
//public:
//	//! アニメーションの種類用の列挙体
//	//! @detail 攻撃アニメーションはコンボの続けたい順番に設定する
//	enum Animation_Max
//	{
//		idle, //!< 待機
//		run,  //!< 走り
//		attack_1, //!< 攻撃１
//		rolling,  //!< ローリング
//		jump,     //!< ジャンプアクション
//
//		anim_max //!< アニメーションの最大数
//	};
//
//	//! 攻撃アニメーションの一番最初
//	static constexpr int ATTACK_ANIM_START = attack_1;
//	//! 攻撃アニメーション最大値
//	static constexpr int ATTACK_ANIM_MAX = jump - ATTACK_ANIM_START;
//
//
//	//! モンスターの状態
//	enum MonsterMode
//	{
//		IDLE,      //!< 待機
//		RUN,       //!< 走り状態
//		ATTACK,    //!< 攻撃
//	};
//
//	enum Jump
//	{
//		STANDBY,     //! スタンバイ
//		GOUP,           //! 上がるタイミング
//		MOVE,           //! ジャンプ中の移動のタイミング
//		DROPDOWN,  //! 落ちてくるタイミング
//
//		max
//	};
//	//! ジャンプの状態を保存する変数
//	int jump_num = 0;
//public:
//
//	//=========================================================================
//	// 関数の宣言
//	//=========================================================================
//	
//	//! @brief コンストラクタ
//	Monster();
//	//! @brief デストラクタ
//	~Monster();
//
//	//! 継承先からの関数のオーバーライド
//	//! @brief 初期処理
//	void Init()override;
//	//! @brief 更新処理
//	//! @param 移動の時のターゲットの座標
//	//! @param ターゲットの半径
//	//! @param カメラオブジェクト
//	void Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera) override;
//
//	//! @brief 描画処理
//	void Draw() override;
//	//! @brief 終了処理
//	void Exit() override;
//
//
//	//! @brief あたり判定の更新処理
//	void CDUpdate() override;
//
//	//! @brief ステータスバーの設定用関数
//	void StatusBarInit() override;
//	//! @brief ステータスバーの描画関数
//	void StatusBarDraw() override;
//
//	//! @brief アニメーション読み込み関数
//	void Anima_Load_Init();
//
//	//! @brief プレイヤーの移動用関数
//	void Move_Update();
//
//	//! @brief 攻撃開始かそうでないかを判断する関数
//	void Attack_First();
//
//	//! @brief 攻撃用の関数
//	void Attack_Update();
//
//	//! @brief ジャンプ攻撃用の関数
//	void Attack_Jump();
//
//	//! @brief ジャンプ攻撃の更新処理
//	void Jump_Update();
//
//	//! @brief コンボ関数
//	void Combo_Update();
//
//	//! @drief 行いたいアニメーションをランダムで選ぶための関数
//	int Set_Rand_Attack();
//
//public:
//	//-----------------------------------------------
//	// 変数の宣言
//	//-----------------------------------------------
//	//! アイドル状態かのフラグ
//	bool m_idle_flag = false;
//	//! 走っていい以下のフラグ
//	bool m_run_flag = false;
//
//	//! 攻撃状態かどおかのフラグ
//	bool m_attack_flag = false;
//	//! プレイヤーのモードを管理する変数
//	int m_monster_mode = -1;
//	//! 今のアニメーション番号を保存する用の変数
//	int m_now_attack_anim = -1;
//
//
//	//! 壁擦り判定のためにいったん座標を保存しておく変数
//	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
//	//! 移動の際の当たり判定用のサイズ
//	Vector3 move_hit_size = { 1.0f ,0.0f,1.0f };
//
//	//------------------------------------------
//	// コンボ関連
//	//------------------------------------------
//	// コンボの最大数
//	static constexpr int COMBO_MAX = 3;
//	//! コンボ用フラグ
//	bool m_combo_flag = false;
//
//	//! 動いてほしいかのフラグ
//
//	//! 次のアニメーション番号を保存
//	int m_next_anim = -1;
//	//! コンボをやめてほしい時のフラグ
//	bool m_stop_combo_flag = false;
//
//
//
//public:
//
//
//
//	//=================
//	// バー型のUI
//	//=================
//	UIBra m_hp;
//	// HPの残量
//	int m_hp_value = HP_MAX;
//};
