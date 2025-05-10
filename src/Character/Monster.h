


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

	//! @brief 攻撃を受けた時のエフェクトの更新処理
	void DamageEffectUpdate() override;

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
		idle_anim,       //!< 待機
		run_anim,        //!< 走り
		die_anim,        //!< 死亡
		shout_anim,      //!< 叫び
		hit_damage_anim, //!< 攻撃を受けた時
		stun_down_anim,  //!< スタンで倒れる時
		stun_up_anim,    //!< スタンで起き上がるとき

		punch_attack_anim,          //!< パンチ攻撃
		bigpunch_attack_anim,      //!< 大パンチ攻撃
		upperpunch_attack_anim,  //!< アッパー攻撃
		kick_attack_anim,             //!< キック攻撃
		tackle_attack_anim,          //!< タックル攻撃
		breath_attack_anim,         //!< ブレス攻撃
		rolling_anim,                    //!< ローリング
		jump_anim,                      //!< ジャンプアクション

		anim_max //!< アニメーションの最大数
	};

	//! 攻撃アニメーションの一番最初
	static constexpr int ATTACK_ANIM_START = punch_attack_anim;
	//! 攻撃アニメーション最大値（jumpを抜いた分ジャンプを抜いておかないとコンボ攻撃の時なバグる）
	static constexpr int ATTACK_ANIM_MAX = rolling_anim - ATTACK_ANIM_START;

	//! 攻撃アクションの数
	static constexpr int ATTACK_ACTION_MAX = anim_max - punch_attack_anim;

	// 攻撃番号の再設定
	enum AttackAnim
	{
		attack_end = -1,                                                //< コンボ攻撃の終わり
		attack_punch = punch_attack_anim - ATTACK_ANIM_START,           //< パンチ攻撃
		attack_bigpunch = bigpunch_attack_anim - ATTACK_ANIM_START,     //< 大パンチ攻撃
		attack_upperpunch = upperpunch_attack_anim - ATTACK_ANIM_START, //< アッパー攻撃
		attack_kick = kick_attack_anim - ATTACK_ANIM_START,             //< キック攻撃
		attack_takle = tackle_attack_anim - ATTACK_ANIM_START,           //< タックル攻撃
		attack_breath = breath_attack_anim - ATTACK_ANIM_START,         //< ブレス攻撃
		attack_rolling = rolling_anim - ATTACK_ANIM_START,              //< ローリング攻撃
		attack_jump = jump_anim - ATTACK_ANIM_START,                    //< ジャンプ攻撃

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
		//---------------------------------------------------------------------------------------
		//       コンボ１　       |          コンボ２        |         コンボ３        | コンボ終了
		//---------------------------------------------------------------------------------------
		  {attack_punch,             attack_breath,           attack_end,           attack_end},
		  {attack_bigpunch,        attack_breath,           attack_end,           attack_end},
		  {attack_kick,                attack_bigpunch,       attack_end,           attack_end},
		  {attack_upperpunch,     attack_punch,           attack_end,           attack_end},
		  {attack_punch,             attack_kick,              attack_end,            attack_end},
		  {attack_breath,            attack_end,               attack_end,           attack_end},
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

	// 攻撃の当たり判定を行ってほしいタイミングの設定の変数
	AttackFrame attack_frame[attack_max] =
	{
		// パンチ
		{ 50.0f, 70.0f, },
		// 大パンチ
		{ 68.0f, 100.0f, },
		// アッパー
		{ 50.0f, 95.0f, },
		// キック
		{ 25.0f, 50.0f, },
		// タックル
		{ 44.0f, 86.0f},
		// ブレス
		{ 100.0f,240.0f},
		// ローリング
		{ 10.0f, 50.0f, },
		// ジャンプ
		{ 15.0f, 105.0f, },

	};

	//! 各攻撃のダメージ
	int m_attack_damage[attack_max]
	{
		20, 30, 30, 10, 10, 50, 20, 20,
	};

	//! エフェクトの種類用の列挙体
	//enum Effect
	//{
	//	punch_attack_effect,           // パンチ攻撃時のエフェクト
	//	big_punch_attack_effect,       // 大パンチ攻撃時のエフェクト
	//	breath_attack_effect,          // ブレス攻撃のエフェクト
	//	damage_effect,                 // ダメージを受けた時のエフェクト
	//	effect_max
	//};
	//// エフェクトをつけたいアニメーションの種類を列挙体で管理
	//enum EffectInfoNum
	//{
	//	// 攻撃番号と合わせたいから攻撃に合うように攻撃から設定
	//	attack_punch_effect_info,       // パンチ攻撃
	//	attack_big_punch_effect_info,   // 大パンチ攻撃
	//	attack_upper_punch_effect_info, // アッパー攻撃
	//	attack_kick_effect_info,        // キック攻撃
	//	attack_tackle_effect_info,      // タックル攻撃
	//	attack_breath_effect_info,      // ブレス攻撃
	//	// ここからは攻撃とは別のエフェクト
	//	damage_effect_info,     // 攻撃を受けた時のエフェクト
	//	roar_effect_info,       // 咆哮時のエフェクト  
	//	effect_info_max
	//};
	
	//! エフェクトの種類用の列挙体
	enum Effect
	{
		punch_attack_effect,           // パンチ攻撃時のエフェクト
		big_punch_attack_effect,       // 大パンチ攻撃時のエフェクト
		breath_attack_effect,          // ブレス攻撃のエフェクト

		effect_max
	};

	// エフェクトをつけたいアニメーションの種類を列挙体で管理
	enum EffectInfoNum
	{
		// 攻撃番号と合わせたいから攻撃に合うように攻撃から設定
		attack_punch_effect_info,       // パンチ攻撃
		attack_big_punch_effect_info,   // 大パンチ攻撃
		attack_upper_punch_effect_info, // アッパー攻撃
		attack_kick_effect_info,        // キック攻撃
		attack_tackle_effect_info,      // タックル攻撃
		attack_breath_effect_info,      // ブレス攻撃

		// ここからは攻撃とは別のエフェクト
		roar_effect_info,       // 咆哮時のエフェクト  

		effect_info_max
	};
	//! エフェクトの種類用の列挙体
	enum DamageEffect
	{
		damage_effect,                 // ダメージを受けた時のエフェクト
		damage_effect_max
	};



	struct EffectInfo
	{
		// エフェクトのスケール
		Vector3 size;
		// モデルのノードにエフェクトを設定したい場合のノード(櫃脳なかったら-1を書く)
		int nodo_index;
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
		// パンチ攻撃
		{ {4.0f,4.0f,4.0f}, 9, {20.0f,5.0f,20.0f},{90.0f,180.0f,0.0f}, 30},
		// 大パンチ攻撃
		{ {8.0f,8.0f,8.0f}, 9, {20.0f,5.0f,20.0f},{35.0f,-90.0f,0.0f}, 50},
		// アッパー攻撃
		{ {4.0f,4.0f,4.0f}, 9, {10.0f,5.0f,10.0f},{0.0f, 0.0f, 0.0f}, 30},
		// キック攻撃
		{ {6.0f,6.0f,6.0f}, 9, {15.0f,20.0f,15.0f},{35.0f,-90.0f,0.0f}, 30},
		// タックル攻撃
		{ {5.0f,5.0f,5.0f}, 9, {15.0f,24.0f,15.0f},{85.0f,-90.0f,0.0f}, 1},
		// ブレス攻撃
		{ {8.0f,8.0f,8.0f}, 9, {30.0f,15.0f,30.0f},{0.0f,-90.0f,0.0f}, 100},
		
		// 咆哮時のエフェクト  
		{ {1.0f,1.0f,1.0f}, 9, {5.0f,12.0f,5.0f},{0.0f,0.0f,0.0f}, 1}
	};
	// ダメージを受けた時のエフェクト
	EffectInfo m_damage_effect_info = { {1.0f,1.0f,1.0f}, -1, {0.0f,15.0f,0.0f},{0.0f,0.0f,0.0f}, 1 };

	// SEの種類用の列挙体
	enum SE
	{
		punch_attack_se_1,  // パンチ攻撃
		big_punch_attack_se, // 大パンチ攻撃２
		breath_attack_se,    // ブウレ攻撃
		damage_se,            // ダメージを受けた時
		roar_se,                  // 咆哮時
		run_se,                   // 足音
		se_max
	};

	// SEををつけたいものの情報の列挙体
	enum SEInfoNum
	{
		// 攻撃番号と合わせたいから攻撃に合うように攻撃から設定
		attack_punch_se_info, // パンチ攻撃
		attack_big_punch_se_info, // 大パンチ攻撃
		attack_upper_punch_se_info, // アッパー攻撃
		attack_kick_se_info, // キック攻撃
		attack_tackle_se_info, // タックル攻撃
		attack_breath_se_info, // ブレス攻撃

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
		// パンチ攻撃
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 40},
		// 大パンチ攻撃
		{ big_punch_attack_se, DX_PLAYTYPE_BACK, true, 60},
		// アッパー攻撃
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 40},
		// キック攻撃
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 20},
		// タックル攻撃
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// ブレス攻撃
		{ breath_attack_se, DX_PLAYTYPE_BACK, true, 1},


		// 攻撃を受けた時
		{damage_se,DX_PLAYTYPE_BACK, true, 1},
		// 咆哮
		{roar_se,DX_PLAYTYPE_BACK, true, 1},
		// 足音
		{run_se, DX_PLAYTYPE_LOOP, true, 1},
	};

	//=================
	// 当たり判定
	//=================
	CapsuleCollision m_left_feet; //!< 左手のあたり判定
	CapsuleCollision m_right_feet; //!< 右手の当たり判定
	CapsuleCollision m_big_punch_hit; //!< 大パンチ専用の当たり判定
	CapsuleCollision m_breath_hit; //!< ブレス攻撃専用の当たり判定
};

