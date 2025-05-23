#pragma once

//! @file HeroPlayer.h
//! @brief ヒーローのクラス
//! @brief キャラクターベースクラスを継承

class Hero :public CharacterBase
{
public:

	//-----------------------------------------------
	// 関数の宣言
	//-----------------------------------------------
	//! @brief コンストラクタ
	Hero();
	//! @brief デストラクタ
	~Hero();

	//-----------------------------------------------
   // 関数の宣言
   //-----------------------------------------------


	//! 継承先からの関数のオーバーライド
	//! @brief 初期処理
	void Init()override;
	//! @brief 更新処理
	//! @param カメラの向き
	void Update(Vector3* camera_rot) override;

	//! @brief 生きてる時の更新処理
	//! @param カメラの向き
	void LiveUpdate(Vector3* camera_rot) override;

	//! @brief 死んだときの更新処理 
	void DieUpdate() override;


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

	//! @brief 当たり判定を行って欲しいタイミングを保存する関数
	void SetHitTimeInit() override;

	//! @brief アニメーション読み込み関数
	//! 攻撃アニメーションの始まるアニメーション番号
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
	void SEUpdate(int se_info_num) override;

	//! @brief プレイヤーの状態(フラグ)管理関数
	//! @param プレイヤーの状態
	void PlayerMode(int mode) override;

	//! @brief 攻撃を受けた時の更新処理
	void ComeAttackUpdate() override;
public:

	//-----------------------------------------------
	// 定数の宣言
	//-----------------------------------------------
	static constexpr float  PLAYER_MOVE_SPEED = 2.0f;     //! 移動スピード
	static constexpr float  HERO_R = 5.0f;                //! 回転スピード
	static constexpr float PLAYER_ROLLING_SPEED = 1.1f;   //! プレイヤーのローリングスピード 
	static constexpr int HP_MAX = 100;                    //! HPの最大値
	static constexpr int PLAYER_HIT_R = 1;                //! プレイヤーの円の当たり半径の半径
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
		rolling, //! ローリング
		hit_damage, //! ダメージを受けた時
		die, //! 倒された時

		// 左クリック攻撃
		attack_sword_anim_1,      //< ソード攻撃１
		attack_sword_anim_2,      //< ソード攻撃２
		attack_sword_anim_3,      //< ソード攻撃３

		attack_sword_anim_4, //< ソード攻撃４
		attack_kick_anim_1,   //< キック攻撃1
		attack_kick_anim_2,   //< キック攻撃1

		counter_anim,              // カウンター攻撃

		anim_max //!< アニメーションの最大数
	};

	static constexpr int ATTACK_ANIM_STAR = attack_sword_anim_1;                     //! アニメーションの攻撃系以外のアニメーションの数
	static constexpr int ATTACK_ACTION = anim_max - ATTACK_ANIM_STAR; //! 攻撃アニメーションの最大数

	// 攻撃番号の再設定
	enum AttackAnim
	{
		attack_sword_1 = attack_sword_anim_1 - ATTACK_ANIM_STAR,	   //< 攻撃２
		attack_sword_2 = attack_sword_anim_2 - ATTACK_ANIM_STAR,	   //< 攻撃３
		attack_sword_3 = attack_sword_anim_3 - ATTACK_ANIM_STAR,    //< 攻撃１
		attack_sword_4 = attack_sword_anim_4 - ATTACK_ANIM_STAR, //< キック攻撃１
		attack_kick_1 = attack_kick_anim_1 - ATTACK_ANIM_STAR,	//< キック攻撃２
		attack_kick_2 = attack_kick_anim_2 - ATTACK_ANIM_STAR,	//< キック攻撃３

		attack_counter = counter_anim - ATTACK_ANIM_STAR,       //< カウンター攻撃

		attack_max
	};

	// 当たり判定をとってほしいフレームの構造体
	struct AttackTiming
	{
		// 始まりのフレーム
		float start_frame;
		// 終わりのフレーム
		float end_frame;
		// ヒットストップを行っていいかどうか
		bool can_hit_stop;
	};


	// 当たり判定を行ってほしいタイミングの設定
	// 今のところ仮
	// 作品展までに完成させる
	AttackTiming attack_frame[attack_max] =
	{
		// 剣攻撃１
		{ 33.0f, 52.0f, false},
		// 剣攻撃２
		{ 48.0f, 62.0f, false},
		// 剣攻撃３
		{	52.0f, 70.0f, true},
		// 剣攻撃4
		{	32.0f, 44.0f, false},
		// キック1
		{	25.0f, 45.0f, false},
		// キック2
		{	37.0f, 60.0f, true},
		// カウンター攻撃
		{   93.0f,  120.0f,true},
	};

	// 各攻撃のダメージ
	int m_attack_damage[attack_max]
	{
		30,40,60,25,30,50,100,
	};

	//! エフェクトの種類用の列挙体
	enum Effect
	{
		attack_sword_effect, // 攻撃時の剣の攻撃エフェクト
		attack_kick_effect,    // キック時のエフェクト
		damage_effect,        // 攻撃を受けた時のエフェクト
		rolling_effect,           // ローリングエフェクト
		effect_max
	};
	// エフェクトをつけたいアニメーションの種類を列挙体で管理
	enum EffectInfoNum
	{
		// 攻撃番号と合わせたいから攻撃に合うように攻撃から設定
		attack_sword_1_effect_info, // 剣攻撃１
		attack_sword_2_effect_info, // 剣攻撃２
		attack_sowrd_3_effect_info, // 剣攻撃３
		attack_sowrd_4_effect_info, // 剣攻撃４
		attack_kick_1_effect_info,    // キック１
		attack_kick_2_effect_info,    // キック２
		attack_counter_effect_info,  // カウンター

		// ここからは攻撃とは別のエフェクト
		damage_effect_info,     // 攻撃を受けた時のエフェクト
		rolling_effect_info,       // ローリング時のエフェクト  

		effect_info_max
	};
	// エフェクトの細かい設定いの内容(攻撃関連のアニメーションにつけるエフェクト用)
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

	// エフェクトの情報
	EffectInfo m_effect_info[effect_info_max] =
	{
		// 攻撃番号と合わせたいので攻撃のエフェクト情報から先に調べる
		// 剣攻撃１
		{ {2.0f,2.0f,2.0f},{10.0f,10.0f,10.0f},{-40.0f,-90.0f,0.0f}, 15},
		// 剣攻撃２
		{ {2.5f,2.5f,2.5f},{8.0f,8.0f,8.0f},{90.0f,-90.0f,0.0f}, 30},
		// 剣攻撃３
		{ {3.5f,3.5f,3.5f},{8.0f,8.0f,8.0f},{-225.0f,90.0f,160.0f}, 20},
		// 剣攻撃4
		{ {2.0f,2.0f,2.0f},{8.0f,8.0f,8.0f},{225.0f,90.0f,160.0f}, 10},
		// キック1
		{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}, 1},
		// キック2
		{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}, 1},
		// カウンター攻撃
		{ {3.0f,3.0f,3.0f},{8.0f,8.0f,8.0f},{195.0f,90.0f,160.0f}, 85},

		// 攻撃を受けた時
		{{1.2f,1.2f,1.2f},{0.0f,10.0f,0.0f},{90.0f,-90.0f,90.0f}, 1},
		// ローリング
	    {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{90.0f,-180.0f,90.0f}, 1}
	};


	// SEの種類用の列挙体
	enum SE
	{
		sword_attack_se_1, // 剣攻撃１
		sword_attack_se_2, // 剣攻撃２
		kick_attack_se_1,    // キック攻撃１
		kick_attack_se_2,    // キック攻撃２
		run_se,                   // 足音
		rolling_se,               // ローリング
		damage_se,            // 攻撃を受けた時のサウンド
		se_max
	};
	// SEををつけたいものの情報の列挙体
	enum SEInfoNum
	{
		// 攻撃番号と合わせたいから攻撃に合うように攻撃から設定
		attack_sword_1_se_info, // 剣攻撃１
		attack_sword_2_se_info, // 剣攻撃２
		attack_sowrd_3_se_info, // 剣攻撃３
		attack_sowrd_4_se_info, // 剣攻撃４
		attack_kick_1_se_info,    // キック１
		attack_kick_2_se_info,    // キック２
		attack_counter_se_info,  // カウンター

		// ここからは攻撃とは別のエフェクト
		run_se_info,           // 足音用
		rolling_se_info,       // ローリング時のエフェクト  
		damage_se_info,     // 攻撃を受けた時のエフェクト

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
		// 剣攻撃１
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 10},
		// 剣攻撃２
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// 剣攻撃３
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// 剣攻撃4
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// キック1
		{ kick_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// キック2
		{ kick_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// カウンター攻撃
		{  sword_attack_se_1, DX_PLAYTYPE_BACK, true, 0},

		// 足音
		{run_se, DX_PLAYTYPE_LOOP, true, 1},
		// ローリング
		{rolling_se, DX_PLAYTYPE_BACK, true, 1},
		// 攻撃を受けた時
		{damage_se,DX_PLAYTYPE_BACK, true, 1},
	};
	//------------------------------------------
	// コンボ関連
	//------------------------------------------
	// コンボの最大数
	static constexpr int COMBO_MAX = 3;

	//=================
	// バー型のUI
	//=================
	UIBra m_hp;

public:
	//-----------------------------------------------
	// クラスのオブジェクトの宣言
	//-----------------------------------------------


	//! カプセルコリジョン
	//=================
	// 当たり判定
	//=================
	CapsuleCollision m_sword;     //!< ソードの当たり判定

};
