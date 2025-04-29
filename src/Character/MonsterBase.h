#pragma once



// モンスター専用のキャラベースクラス

//! 
//! @fule MonsterBase.h
//! @brief モンスターのベースクラス


class MonsterBase
{
public:
	//! コンストラクタ
	MonsterBase();

	//! デストラクタ
	~MonsterBase();

	//! 純粋仮想関数
	//! @brief 初期処理
	//! //! @param 移動の際のターゲット座標
	virtual void Init() = 0;
	//! @brief 描画処理
	virtual void Draw() = 0;
	//! @brief 更新処理
	//! @param 移動の時のターゲットの座標
	//! @param ターゲットの半径
	//! @param ターゲットのbodyのカプセル（当たり判定）
	//! @param カメラオブジェクト
	virtual void Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera) = 0;

	//! @brief 生きてる時の更新処理
	//! @param ターゲットの座標
	//! @param ターゲットの半径
	//! @param カメラオブジェクト
	virtual void LiveUpdate(Transform* target_pos, float target_r, Camera* camera) = 0;

	//! @brief 登場演出用の更新処理
	virtual void EntryUpdate() = 0;

	//! @brief HPが一定まで減ったときのレベルアップ処理
	virtual void ReinforceUpdate() = 0;

	//! @brief 攻撃を受けた時の更新処理
	virtual void ComeAttackUpdate() = 0;

	//! @brief 死んだときの更新処理 
	virtual void DieUpdate() = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;

	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ステータスバーの設定用関数
	virtual void StatusBarInit() = 0;

	//! @brief ステータスバーの描画関数
	virtual void StatusBarDraw() = 0;

	//! @brief ダメージを受けた時の処理
	//virtual void DamageUpdate() = 0;

	//! @brief アニメーションの指定のフレームで移動させる処理
	//! @param 移動スピード
	//! @param 移動開始フレーム
	//! @param 移動終了フレーム
	void MoveBetween(float mov_speed, float mov_start_frame, float mov_end_frame);

	//! @brief ローリングアクションをセットする関数
	//! @param ローリングアニメーション番号
	//! @param ターゲットローリングアクションを行う
	void SetRollingAction(int rolling_anim, int target_distance);

	//! @brief ローリングアクション用の関数（回避）
	//! @param ローリングしたときの移動スピード
	void ActionRolling(const int rolling_speed, float rolling_start_frame, float rolling_end_frame);

	//! @brief 攻撃に関することの初期関数
	//! 当たり判定を行ってほしいタイミングを保存する
	//! コンボパターンの初期化
	//! 各コンボパターンの設定
	virtual void SetAttackInfo() = 0;

	//! @brief スタンした時用の更新処理
	//! @param スタン時のダウンアニメーション番号
	//! @param スタン時の起き上がるアニメーション番号
	//! @param スタン値の最大量
	virtual void StunActionUpdate(int down_anim_num, int up_anim_num, int sutn_value_max);

	//! @brief モンスターの状態(フラグ)管理関数
	//! @param モンスターの状態
	virtual void MonsterMode(int mode) = 0;

	//! @brief アニメーション読み込み用関数
	virtual void AnimLoadInit() = 0;

	//! @brief エフェクトの読み込みをまとめる関数
	virtual void EffectLoadInit() = 0;

	//! @brief エフェクトの更新処理
	//! @param 行いたいエフェクト番号
	//! @param 行いたいエフェクトの情報番号
	virtual void EffectUpdate( int effect_num, int effect_info_num) = 0;

	//! @brief 攻撃を受けた時のエフェクトの更新処理
	virtual void DamageEffectUpdate() = 0;

	//! @brief SEの読み込み
	virtual void SELoadInit() = 0;

	//! @brief SEの更新処理
	//! @param 行いたいSE番号
	virtual void SEUpdate(int se_num) = 0;

	//! @brief 攻撃アニメーションに関する情報の設定
	//! @param 攻撃アニメーションの開始番号
	//! @param 攻撃アニメーションの最大数
	//! @param ランダムで攻撃を選ぶ時のはぶいてほしい攻撃番号
	void SetAttackAnimInfo(int attack_anim_start, int attack_anim_max, int except_attack);

	//! @brief アイドルアクション処理
	//! @param アイドルアニメーション番号
	void IdleActionUpdate(int idle_anim_num);

	//! @brief 移動アクション処理
	//! @param 移動用アニメーション番号
	void MoveAction(int ran_anim);

	//! @brief コンボパターンの数を設定
	//! @param コンボパターンの最大数
	void ComboPatternNumberInit(int pattern_max);

	//! @brief 各コンボパターンの設定
	//! @param 何パターン目のコンボか
	//! @param コンボの数の最大数
	//! @param コンボが終わった後のあと隙のフレーム数
	//! @param コンボ用の攻撃番号が保存されている配列をもらう
	void ComboPatternInfoInit(int pattern_num, int combo_num_max, int rear_crevice_frame, int* anim_num);

	//! @brief 攻撃範囲に侵入したかどうかを判断する
	//! @return true : 攻撃範囲に入った、false : 攻撃範囲外
	bool HitAttackArea();

	//! @brief 最初の攻撃を行う用関数
	void FirstAttackAction();

	//! @brief 攻撃関連の更新処理
	void AttackActionComboUpdate();

	//! @brief 一定のダメージを受けた時に相手との距離をとる攻撃をセットする(咆哮攻撃)
	//! @param 咆哮攻撃用のアニメーション番号
	//! @param seの番号
	//! @param カメラシェイクのためのカメラ
	void RoarSet(int anim_num, int se_num, Camera* camera);

	//! @brief 咆哮攻撃中の処理
	//! @param カメラシェイクのためのカメラ
	void RoarAction(Camera* camera);

	//! @brief ジャンプ攻撃開始の処理
	//! @param ジャンプ用のアニメーション番号
	//! @param 相手と離れててほしい距離
	void JumpAction(int jump_anim, int target_distance);

	//! @brief ジャンプ攻撃の更新処理
	//! @param ジャンプ時の移動スピード
	//! @param ジャンプでの移動開始フレーム
	//! @param ジャンプでの移動終了フレーム
	void JumpActionUpdate(float jump_mov_speed, float jump_mov_strat_frame, float jump_mov_end_frame);

	//! @brief コンボ攻撃更新処理
	void ComboUpdate();

	//! @brief 行いたい攻撃アニメーションをランダムで選ぶための関数
	int SetRandAttack();

	//! @brief ベースクラスでの初期処理
	//! @param モンスターのHPマックス
	//! @param ジャンプの上昇スピード
	//! @param ジャンプの下降スピード
	void BaseInit(int hp_num);

	//! @brief モンスターの移動に関するターゲットの設定
	//! @param ターゲットの座標
	//! @param ターゲットのカプセルの当たり判定の半径
	//! @param ターゲットのbodyのカプセル
	void BaseSetTarget(Transform* target_pos, const float m_target_hit_r, CapsuleCollision body);

	//! @brief 移動の更新処理
	//! @param  歩いている状態かのフラグ
	void MoveUpdate(bool* run_flag);

	//! @brief 当たり判定を行ってほしいタイミングをセットする関数
	//! @param 当たり判定をとってほしいフレームのスタート
	//! @param 当たり判定を終わってほしいフレームの終わり
	//! @param 保存したい攻撃番号
	void SetHitTime(int attack_frame_start, int attack_frame_end, int attack_num);

	// 当たり判定をとってよいタイミングかを判断する関数
	//! @brief 攻撃番号
	bool AttackHitGoodTiming(int anim_num);

	//! @brief 当たり判定とダメージを保存する関数
	//! @param 当たり判定用のカプセル
	//! @param 与えるダメージ
	//! @param 攻撃番号
	void SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num);

	//! @brief 攻撃時の当たり判定を設定する用の関数
	//! @param 攻撃アニメーションの最大数
	void SetAttackHitDamage(int attack_anim_max);

public:

	//! モンスターの状態
	enum MonsterMode
	{
		IDLE,       //!< 待機状態
		RUN,       //!< 走り状態
		ATTACK,  //!< 攻撃状態
		STUN,      //!< スタン状態
		ROAR,       //!< 咆哮状態
		DIE,        //!< 死ぬ
	};
	//! モンスターの状態を管理する変数
	int m_monster_mode = 0;

	//-----------------------------------------------
	// 列挙体で管理
	//-----------------------------------------------
	//! モンスターの状態（生死）
	enum Situation
	{
		alive, // 生きてるとき
		die,   // 死んでる
	};
	//! 生きてるかを保存するための変数
	// 最初は生きてる状態から始める
	int m_life_and_death = alive;

	//! モンスターのに使用するフラグ
	//! アイドル状態かのフラグ
	bool m_idle_flag = false;
	//! 1フレーム前の攻撃フラグの状態を保存しておく
	bool m_past_attack_flag = m_attack_flag;
	//! ダメージを受けた時用のフラグ
	bool m_damage_anim_flag = false;
	//! 当たり判定をとっていいかのフラグ
	bool m_can_hit_damage_flag;
	//! スタン状態かのフラグ
	bool m_stun_flag;



private:
	//! 攻撃範囲の大きさ(一旦モンスター全員共通にする)
	int M_ATTACK_HIT_RANGE = 10;

	//! 攻撃アニメーションのスタート番号
	int M_ATTACK_ANIM_START = 0;
	//! 攻撃アニメーションの最大数
	int M_ATTACK_ANIM_MAX = 0;
	//! ランダムで攻撃を選ぶ際にはぶいてほしいアニメーション番号
	int M_ATTACK_ANIM_EXCEPT = 0;




public:
	//------------------------------------------
	// ステータス関連
	//------------------------------------------
	// HPの最大値
	int m_hp_max;
	// HPの残量
	int m_hp_value;
	// HP用のバー
	UIBra m_hp_bra;

	//------------------------------------------
	// Run関連
	//------------------------------------------
	//! 走っていい以下のフラグ
	bool m_run_flag = false;
	//! 壁擦り判定のためにいったん座標を保存しておく変数
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
	//! 移動の際の当たり判定用のサイズ
	Vector3 move_hit_size = { 1.0f ,0.0f,1.0f };
	//! 自身の半径を入れる用の変数
	float m_hit_r = 15.0f;
	//! 自身の移動スピード
	static constexpr float M_MOV_SPEED = 1.0f;
	//! 自身回転速度
	static constexpr float M_ROT_SPEED = 5.0f;
	//! どのくらいのフレームの間は走っているのかをカウントするための関数
	int m_running_frame_count = 0;

private:
	//------------------------------------------
	// アイドル関連
	//------------------------------------------
	//! どれだけのフレーム止まって欲しいかセットしたか同課のフラグ
	bool m_set_stop_frame_flag = false;
	//! アイドル状態で止まっていてほしいフレーム
	int m_stop_frame = 0;
	//! 何フレーム止まっているのかのカウント
	int m_stop_frame_count = 0;
public:

	//------------------------------------------
	// ジャンプ関連
	//------------------------------------------
	//! ジャンプのための移動量用の変数
	Vector3 m_jump_mov{ 0.0f,0.0f,0.0f };
	//! ジャンプの着地地点の座標
	Vector3 m_jump_pos{ 0.0f,0.0f,0.0f };
	//! ジャンプの時に進む距離
	float m_jump_move;
	//! ジャンプフラグ
	bool m_jump_flag = false;

	//------------------------------------------
	// ローリング関連
	//------------------------------------------
	//! ローリングフラグ
	bool m_rolling_flag;


	//------------------------------------------
	// 攻撃関連
	//------------------------------------------
	// 攻撃状態の管理
	enum Attack
	{
		ATTACKSET,
		UNDERATTACK,
	};
	//! 攻撃状態を保存する変数
	int m_attack_info_num = ATTACKSET;
	//! 今のアニメーション番号を保存する用の変数
	int m_now_attack_anim = 0;
	//! モンスターの現在行っている攻撃アニメーション番号を保存する
	int m_now_attack = -1;
	//! コンボが何個目か
	int m_combo_num = 0;
	//! 咆哮攻撃用のフラグ
	bool m_roar_flag = false;
	//! 咆哮攻撃を何度したか
	int m_roar_count = 3;
	//! 攻撃状態かどおかのフラグ
	bool m_attack_flag = false;
	//! モンスターの前方方向の攻撃エリア（このエリアに入ったら攻撃をする）
	CapsuleCollision m_attack_area;
	Vector3 m_attack_area_1;
	Vector3 m_attack_area_2;
	//! どのくらい前方方向かの値
	static constexpr int ATTACK_AREA_DISTANCE = 15;
	//! 攻撃エリアに半径
	static constexpr int ATTACK_AREA_R = 14;

	//------------------------------------------
	// 攻撃の当たり判定関連
	//------------------------------------------
	// 攻撃の時の当たり判定とダメージの構造体
	// 各子クラスで定義する
	struct Attack_Hit_Damage
	{
		//!  攻撃時に使いたい当たり判定
		CapsuleCollision attack_hit;
		//! 攻撃にあったダメージ
		int attack_damage = 0;

		//! 当たり判定をとってほしいタイミング
		//! スタート
		int start_time = 0;
		//! 終わり
		int end_time = 0;

	};
	std::vector< Attack_Hit_Damage*> m_attack_hit_damage;

	//------------------------------------------
	// コンボ関連
	//------------------------------------------
	// コンボを何パターンか用意してそれをランダムでモンスターに選ばせる
	struct ComboPattern
	{
		// コンボが何個繋がるかの数
		int m_combo_num_max;
		// コンボで使うアニメーション番号を保存するために変数
		// 
		std::vector<int> m_combo_parts;
		// コンボの後のあと隙何フレームか
		int m_rear_crevice_frame;
	};
	std::vector<ComboPattern> m_combo_pattern;
	//! コンボのパターンの最大数を保存する
	int m_combo_pattern_max = 0;
	//! 何パターン目のコンボを使用するか
	int m_combo_pattern_num = 0;
	// 現在使用しているコンボパターン保存
	int m_now_combo_pattern_num = 0;
	// 上記で作っているコンボのパターン化ができればこの下のフラグ関連はいらなくなる
	// コンボの最大数
	static constexpr int COMBO_MAX = 3;
	//! コンボ用フラグ
	bool m_combo_flag = false;

	//! 次のアニメーション番号を保存
	int m_next_anim = -1;
	//! コンボをやめてほしい時のフラグ
	bool m_stop_combo_flag = false;


	//------------------------------------------
	// スタン関連
	//------------------------------------------
	// スタンの状態を管理
	enum Stun
	{
		DOWN,          //! 倒れているとき
		UPSTANDBY,  //! 倒れてから起き上がるまでの間
		UP,                //! 起き上がるとき
	};
	//! スタンの状態を保存する変数
	int m_stun_info_num = DOWN;
	//! スタン値の残量
	int m_stun_value;

	//! スタン値の回復量
	static constexpr int RECOVERY_STUN_VALUE = 1;
	//! 何フレーム攻撃を受けていなければスタン値を回復するか
	static constexpr int STUN_VALUE_RECOVERY_FRAME = 60;
	// スタン用のバー
	UIBra m_stun_bra;
protected:
	//! 攻撃を受けていないフレームをカウント
	int m_not_damaged_frame = 0;
public:
	//-----------------------------------------------
	// クラスのオブジェクトの宣言
	//-----------------------------------------------
	//! 自身の情報を入れる変数
	Transform m_transform;
	//! 移動に使うクラス
	TargetMove m_move;
	//! モデルクラスのオブジェクト
	Model m_model;
	//! アニメーションクラスのオブジェクト
	Animation m_animation;
	//! コンボクラス
	Combo m_combo;
	//! エフェクトクラスオブジェクト
	Effect m_effect;
	//! 攻撃を受けたときようのエフェクトクラスオブジェクト
	Effect m_damage_effect;
	//! サウンドクラスのオブジェクト
	Sound m_se;

	//=================
	// 当たり判定
	//=================
	//! カプセルコリジョン
	CapsuleCollision m_left_hand; //!< 左手のあたり判定
	CapsuleCollision m_right_hand; //!< 右手の当たり判定
	CapsuleCollision m_body;       //!< 本体のあたり判定

};
