#pragma once
#pragma once

//! @file Mutant.h
//! @brief ミュータントのクラス
//! @brief モンスターベースクラスを継承
class Mutant :public MonsterBase
{
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
	void Update(Transform* target_pos, float target_r) override;
	//! @brief 生きてる時の更新処理
	void LiveUpdate(Transform* target_pos, float target_r) override;
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


	//=========================================================================
	// 定数の宣言
	//=========================================================================
	static constexpr float MONSTER_MOVE_SPEED = 1.5f;        //! 移動スピード
	static constexpr float MONSTER_ROT_SPEED = 0.5f;         //! 回転スピード

	static constexpr float CHANGE_JUMP_RUNNIG_FRAME = 150;   //! Runからジャンプ,ローリングに移るのに何フレームかかるか

	static constexpr float JUMP_TARGET_DISTANCE = 60.0f;     //! ジャンプするとき相手と離れててほしい最低限の距離
	static constexpr float JUMP_MOV_SPEED = 1.0f;            //! ジャンプ中の移動スピード
	static constexpr float JUMP_DEVIATION_POS = 26.0f;       //! ジャンプ攻撃をしたときにズレを直す値
	static constexpr float JUMP_STRAT_FRAME = 34.0f;         //! ジャンプの移動開始のフレーム
	static constexpr float JUMP_END_FRAME = 90.0f;           //! ジャンプの移動終了のフレーム

	static constexpr float ROLLING_SPEED = 1.0f;             //! ローリングスピード
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

		attack_1_anim, //!< 攻撃１
		attack_2_anim, //!< 攻撃２
		attack_3_anim, //!< 攻撃３
		attack_4_anim, //!< 攻撃４
		attack_5_anim, //!< 攻撃５
		attack_6_anim, //!< 攻撃６
		rolling_anim,  //!< ローリング
		jump_anim,     //!< ジャンプアクション

		anim_max //!< アニメーションの最大数
	};

	//! 攻撃アニメーションの一番最初
	static constexpr int ATTACK_ANIM_START = attack_1_anim;
	//! 攻撃アニメーション最大値（jumpを抜いた分ジャンプを抜いておかないとコンボ攻撃の時なバグる）
	static constexpr int ATTACK_ANIM_MAX = rolling_anim - ATTACK_ANIM_START;

	//! 攻撃アクションの数
	static constexpr int ATTACK_ACTION_MAX = anim_max - attack_1_anim;

	// 攻撃番号の再設定
	enum AttackAnim
	{
		attack_end = -1, //< コンボ攻撃の終わり
		attack_punch_1 = attack_1_anim - ATTACK_ANIM_START,  //< 攻撃１
		attack_punch_3 = attack_3_anim - ATTACK_ANIM_START,	 //< 攻撃３
		attack_punch_2 = attack_2_anim - ATTACK_ANIM_START,	 //< 攻撃２
		attack_punch_4 = attack_4_anim - ATTACK_ANIM_START,  //< 攻撃４
		attack_punch_5 = attack_5_anim - ATTACK_ANIM_START,  //< 攻撃５
		attack_punch_6 = attack_6_anim - ATTACK_ANIM_START,  //< 攻撃６
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
	  {attack_punch_4,attack_punch_2,attack_end,attack_end},
	  {attack_punch_1,attack_punch_6,attack_end,attack_end},
	  {attack_punch_4,attack_punch_3,attack_end,attack_end},
	  {attack_punch_5,attack_punch_2,attack_punch_1,attack_end},
	  {attack_punch_6,attack_punch_1,attack_punch_4,attack_end},
	  {attack_punch_1,attack_punch_4,attack_punch_3,attack_end},
	};
	// 各コンボの後隙
	int m_combo_rear_crevice_frame[M_COMBO_PATTERN_MAX]
	{
		{120},{120},{120},{120},{120},{120}
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
		// パンチ２
		{ 40.0f, 90.0f, },
		// パンチ３
		{ 30.0f, 90.0f, },
		// パンチ４
		{ 25.0f, 75.0f, },
		// パンチ５
		{10.0f,30.0f},
		// パンチ６
		{45.0f,75.0f},
		// ローリング
		{ 10.0f, 50.0f, },
		// ジャンプ
		{ 15.0f, 105.0f, },

	};

	//! 各攻撃のダメージ
	int m_attack_damage[attack_max]
	{
		100,100,100,100,100,100,100,100,
	};
};
