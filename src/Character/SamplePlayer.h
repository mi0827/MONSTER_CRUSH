#pragma once

//! @fule SamplePlayer.h
//! @brief サンプルキャラのクラス
//! @brief キャラクターベースクラスを継承

class SamplePlayer :public CharacterBase
{
public:

public:
	
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
	void Anima_Load_Init();

	//! @brief プレイヤーの移動用関数
	void Move_Update(Vector3* camera_rot);

	//! @brief プレイヤーの状態(フラグ)管理関数
	//! @param プレイヤーの状態
	void PlayerMode(int mode) override;

	//! @brief 攻撃開始かそうでないかを判断する関数
	void AttackFirst();

	//! @brief 攻撃用の関数
	void AttackUpdate();

	//! @brief ローリングセット用の関数
	void SetRolling();

	//! @brief ローリングアクション用の関数（回避）
	void ActionRolling();

	//! @brief カウンターアクション
	void CounterAction();

	//! @brief コンボ関数
	void ComboUpdate();

	//! @brief 攻撃を受けたかをチェックするための関数
	void CheckHitDamage();

	//! @brief 攻撃を受けて時の処理
	void HitDamageUpdate();

public:
	//! アニメーションの種類用の列挙体
	//! @detail 攻撃アニメーションはコンボの続けたい順番に設定する
	enum Animation_Max
	{
		idle, //!< 待機
		run, //!< 走り
		rolling, //!< ローリング
		hit_damage, //!< ダメージを受けた時
		die,     //!< 倒された時
		attack_anim_1,      //< 攻撃１
		attack_anim_2,      //< 攻撃２
		attack_anim_3,      //< 攻撃３
		attack_kick_anim_1, //< キック攻撃１
		attack_kick_anim_2, //< キック攻撃２
		attack_kick_anim_3, //< キック攻撃３

		anim_max //!< アニメーションの最大数
	};

	static constexpr int ATTACK_ANIM_STAR = attack_anim_1;                     //! アニメーションの攻撃系以外のアニメーションの数
	static constexpr int ATTACK_ACTION = anim_max - ATTACK_ANIM_STAR; //! 攻撃アニメーションの最大数

	// 攻撃番号の再設定
	enum AttackAnim
	{
		attack_punch_1 = attack_anim_1 - ATTACK_ANIM_STAR,    //< 攻撃１
		attack_punch_2 = attack_anim_2 - ATTACK_ANIM_STAR,	   //< 攻撃２
		attack_punch_3 = attack_anim_3 - ATTACK_ANIM_STAR,	   //< 攻撃３
		attack_kick_1 = attack_kick_anim_1 - ATTACK_ANIM_STAR, //< キック攻撃１
		attack_kick_2 = attack_kick_anim_2 - ATTACK_ANIM_STAR,	//< キック攻撃２
		attack_kick_3 = attack_kick_anim_3 - ATTACK_ANIM_STAR,	//< キック攻撃３

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
		// パンチ１
		{ 0.0f, 30.0f, },
		// パンチ２
		{ 15.0f, 50.0f, },
		// パンチ３
		{	50.0f, 90.0f, },
		// キック１
		{	20.0f, 55.0f, },
		// キック２
		{	15.0f, 60.0f, },
		// キック３
		{	25.0f, 60.0f, },
	};

	// 各攻撃のダメージ
	int m_attack_damage[attack_max]
	{
		50,70,70,70,70,70,
	};


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

	
	//-----------------------------------------------
// 定数の宣言
//-----------------------------------------------
	static constexpr float PLAYER_MOVE_SPEED = 2.0f;       //! プレイヤーの移動スピード
	static constexpr float PLAYER_R = 5.0f;                         //! プレイヤーの回転スピード
	static constexpr float PLAYER_ROLLING_SPEED = 1.1f;   //! プレイヤーのローリングスピード 
	static constexpr float PLAYER_HIT_R = 1.5;                   //! プレイヤーの円の当たり半径の半径
	static constexpr int HP_MAX = 100;                               //! HPの最大値
	static constexpr int M_DEFENSE = 20;                           //! 防御力
	//----------------------------------------------- 
	// 列挙体の宣言
	//-----------------------------------------------
};
