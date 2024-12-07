#pragma once
#pragma once

//! @fule Mutant.h
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
	static constexpr float MONSTER_MOVE_SPEED = 1.5f;      //! 移動スピード
	static constexpr float MONSTER_ROT_SPEED = 0.5f;       //! 回転スピード
	static constexpr float CHANGE_JUMP_RUNNIG_FRAME = 180; //! Run
	static constexpr float TARGET_DISTANCE = 40.0f;        //! ジャンプするとき相手と離れててほしい最低限の距離
	static constexpr float JUMP_DEVIATION_POS = 26.0f;     //! ジャンプ攻撃をしたときに
	// static constexpr float JUMP_UP_SPEED = 10.0f;       //! ジャンプの時の上昇スピード
	// static constexpr float JUMP_DOWN_SPEED = 2.0f;      //! ジャンプの時の下に落ちてほしいスピード 
	static constexpr int ROLLING_SPEED = 1.0f;             //! ローリングスピード
	static constexpr int HP_VALUE_MAX = 500;               //! HPの最大値
	static constexpr int STUN_VALUE_MAX = 150;             //! スタン値の最大
	//static constexpr int COMBO_PATTERN_MAX = 5;          //! コンボパターンの数
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
		attack_end = -1,                                                         //< コンボ攻撃の終わり
		attack_punch_1 = attack_1_anim - ATTACK_ANIM_START,      //< 攻撃１
		attack_punch_2 = attack_2_anim - ATTACK_ANIM_START,	   //< 攻撃２
		attack_punch_3 = attack_3_anim - ATTACK_ANIM_START,	   //< 攻撃３
		attack_punch_4 = attack_4_anim - ATTACK_ANIM_START,       //< 攻撃４
		attack_rolling = rolling_anim - ATTACK_ANIM_START,      	//< 攻撃５
		attack_jump = jump_anim - ATTACK_ANIM_START,

		attack_max
	};

	//! コンボパターンの数
	static const int M_COMBO_PATTERN_MAX = 6;
	//! 各パターンのコンボの最大数
	static const int M_COMBO_NUM_MAX = 6;
	//! 各コンボパターンにコンボに使用する攻撃番号を保存しコンボを作成する
	//! コンボの最後にはattack_endを入れること
	int m_combo_pattern[M_COMBO_PATTERN_MAX][M_COMBO_NUM_MAX]
	{
	  {attack_punch_1,attack_punch_2,attack_punch_3,attack_punch_4,attack_punch_1,attack_end},
	  {attack_punch_2,attack_punch_2,attack_punch_2,attack_punch_2,attack_punch_2,attack_end},
	  {attack_punch_3,attack_punch_3,attack_punch_3,attack_punch_3,attack_punch_3,attack_end},
	  {attack_punch_4,attack_punch_4,attack_punch_4,attack_punch_4,attack_punch_4,attack_end},
	  {attack_punch_1,attack_punch_1,attack_punch_1,attack_punch_1,attack_punch_1,attack_end},
	  {attack_punch_1,attack_punch_1,attack_punch_1,attack_punch_1,attack_punch_1,attack_end},
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
		{ 00.0f, 30.0f, },
		// パンチ２
		{ 0.0f, 90.0f, },
		// パンチ３
		{ 0.0f, 80.0f, },
		// パンチ４
		{ 0.0f, 70.0f, },
		// ローリング
		{ 0.0f, 50.0f, },
		// ジャンプ
		{ 0.0f, 40.0f, },

	};

	//! 各攻撃のダメージ
	int m_attack_damage[attack_max]
	{
		10,20,20,20,20,20
	};
};
