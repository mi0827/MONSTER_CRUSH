#pragma once

//! @fule HeroPlayer.h
//! @brief ヒーローのクラス
//! @brief キャラクターベースクラスを継承

class Hero :public CharacterBase
{
public:


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

	//! @brief プレイヤーの移動用関数
	//! (ランアニメーションの番号をを引数で渡す)
	//void MoveAction(Vector3* camera_rot) override;

	//! @brief プレイヤーの状態(フラグ)管理関数
	//! @param プレイヤーの状態
	void PlayerMode(int mode) override;
public:

	//-----------------------------------------------
	// 定数の宣言
	//-----------------------------------------------
	static constexpr float  PLAYER_MOVE_SPEED = 2.0f; //! 移動スピード
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
	struct AttackFrame
	{
		// 始まりのフレーム
		float start_frame;
		// 終わりのフレーム
		float end_frame;
	};

	// 当たり判定を行ってほしいタイミングの設定
	// 今のところ仮
	// 作品展までに完成させる
	AttackFrame attack_frame[attack_max] =
	{
		// 剣攻撃１
		{ 33.0f, 52.0f, },
		// 剣攻撃２
		{ 48.0f, 62.0f, },
		// 剣攻撃３
		{	52.0f, 70.0f, },
		// 剣攻撃4
		{	32.0f, 44.0f, },
		// キック1
		{	25.0f, 45.0f, },
		// キック2
		{	37.0f, 60.0f, },
		// カウンター攻撃
		{   93.0f,  120.0f},
	};

	// 各攻撃のダメージ
	int m_attack_damage[attack_max]
	{
		50,70,70,70,70,70,100,
	};


	//------------------------------------------
	// コンボ関連
	//------------------------------------------
	// コンボの最大数
	static constexpr int COMBO_MAX = 3;

	////=================
//// バー型のUI
////=================
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
