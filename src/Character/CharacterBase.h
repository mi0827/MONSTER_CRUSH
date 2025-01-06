#pragma once
//! @file CharacterBase.h
//! @brief キャラクターのベースクラス
class CharacterBase
{
public:

	//-----------------------------------------------
	//! 関数の宣言
	//-----------------------------------------------
	//! @brief コンストラクタ
	CharacterBase();

	//! @brief デストラクタ
	~CharacterBase();

	//! 純粋仮想関数
	//! @brief 初期処理
	virtual void Init() = 0;

	//! @brief 更新処理
	//! @param カメラの向き
	virtual void Update(Vector3* camera_rot) = 0;
	//! @brief 生きてる時の更新処理
	//! @param カメラの向き
	virtual void LiveUpdate(Vector3* camera_rot) = 0;

	//! @brief ベースクラスでの初期処理
	//! @param 敵の移動の際に使う半径の設定
	//! @param HPの最大量
	void BaseInit(float r, int HP);

	//! @brief 死んだときの更新処理 
	virtual void DieUpdate() = 0;

	//! @brief 描画処理
	virtual void Draw() = 0;

	//! @brief 終了処理
	virtual void Exit() = 0;

	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ステータスバーの設定用関数
	virtual void StatusBarInit() = 0;

	//! @brief ステータスバーの描画関数
	virtual void StatusBarDraw() = 0;

	//! @brief 当たり判定を行って欲しいタイミングを保存させるための関数
	virtual void SetHitTimeInit() = 0;

	//! @brief アニメーションの読み込みをまとめる関数
	virtual void AnimLoadInit() = 0;

	//! @brief エフェクトの読み込みをまとめる関数
	virtual void EffectLoadInit() = 0;

	//! @brief エフェクトの更新処理
	//! @param 行いたいエフェクト番号
	//! @param 行いたいエフェクトの情報番号
	virtual void EffectUpdate(int effect_num, int effect_info_num) = 0;

	//! @brief SEの読み込み
	virtual void SELoadInit() = 0;

	//! @brief SEの更新処理
	//! @param 行いたいSEの情報が保管されている番号
	virtual void SEUpdate(int se_info_num) = 0;

	//! @brief プレイヤーの状態(フラグ)管理関数
	//! @param プレイヤーの状態
	virtual void PlayerMode(int mode) = 0;

	//! @brief 攻撃を受けた時の更新処理
	virtual void ComeAttackUpdate() = 0;

	//! @brief プレイヤーの移動用関数
	//! @param Runアニメーションの保存されているアニメーション番号
	//! @param プレイヤーの移動スピード
	//! @param カメラの向き
	void MoveAction(int run_anim_no, const float move_speed,Vector3* camera_rot);

	//! @brief キャラの壁擦り判定用の関数
	//! @param キャラの座標
	//! @param 当たり判定相手のボックスの情報
	void MoveHitUpdate(BoxCollision* box);

	//! @brief キャラクターの座標を設定する関数
	//! @param 設定したい座標
	void SetCharacterPos(Vector3 pos);

	//! @brief キャラクターの向きを設定する関数
	//! @param 設定したい向き
	void SetCharacterRot(Vector3 rot);

	//! @brief 攻撃に関する情報をセットするための関数
	//! @param 攻撃アニメーションが始まるアニメーション番号
	//! @param 最初に攻撃したいアニメーション番号1
	//! @param 最初に攻撃したいアニメーション番号2
	//! @param コンボ攻撃の最大コンボ数
	void SetAttackInfo(const int attack_start_anim, int attack_1, int attack_2, const int combo_max);

	//! @brief 最初の攻撃を行うための関数
	void AttackFirst();

	//! @brief 攻撃の更新処理
	void AttackUpdate();

	//! @brief 当たり判定を行ってほしいタイミングをセットする関数
	//! @param 当たり判定をとってほしいフレームのスタート
	//! @param 当たり判定を終わってほしいフレームの終わり
	//! @param 保存したい攻撃番号
	//! @param ヒットストップを行っていいか
	void SetHitTiming(int attack_frame_start, int attack_frame_end,  bool can_hitstop, int attack_num);

	// 当たり判定をとってよいタイミングかを判断する関数
	//! @brief 攻撃番号
	bool AttackHitGoodTiming(int anim_num);

	//! @brief 当たり判定とダメージを保存する関数
	//! @param 当たり判定用のカプセル
	//! @param 与えるダメージ
	//! @param 攻撃番号
	void SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num);

	//! @brief ローリングアクションを開始されるための関数
	void RollingActionStart();

	//! @brief ローリングアクションの更新処理
	//! @param ローリングが保存されているアニメーション番号
	//! @param ローリング時の移動スピード
	void RollingActionUpdate(int rolling_anim_no,const int rolling_speed);

	//! @brief カウンターアクション
	//! @param カウンターアニメーションが保存されている番号
	void CounterAction(int counter_anim_no);

	//! @brief コンボアクションの更新処理
	void ComboActionUpdate();

	//! @brief 攻撃を受けたかをチェックするための関数
	void CheckHitDamage();

	//! @biref 攻撃を受けて時のアニメーション処理
	//! @param ダメージを受けたアニメーションが保存されている番号
	void HitDamageUpdate(int hit_damage_anim);


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
		//! ヒットストップを行っていい攻撃かどうか
		bool can_hit_stop;
	};
	std::vector< Attack_Hit_Damage*> m_attack_hit_damage;
	//! @brief 攻撃時の当たり判定を設定する用の関数
	//! @param 攻撃アニメーションの最大数
	void  NEWSetAttackHitDamage(int attack_anim_max);

	//! @brief ベースクラスでの更新処理
	//! @param １：移動中かそうでないかのフラグ
	//! @param ２：カメラの向き
	//! @param ４：移動スピード
	//! @detail 移動に関するものの更新処理
	void BaseUpdate(bool* m_check_move, Vector3* camera_rot, const float* mov_speed);




	//-----------------------------------------------
	//! ゲームパッドの処理
	//-----------------------------------------------
	// ゲームパッド用の変数
	int m_pad_input = 0;

	PAD_NO m_pad_no = PAD_NO::PAD_NO1;				// 何番のパッドを使っているのか
	// ゲームパッドの番号をセットする
	// DXライブラリー側でのパッドの番号も取得
	void SetPadNo(PAD_NO no, int dx_no)
	{
		m_pad_no = no;
		m_pad_input = dx_no;
	}
	// ゲームパッドの番号をとってくる
	PAD_NO GetPadNo() const
	{
		return m_pad_no;
	}

public:


	//-----------------------------------------------
	//! クラスのオブジェクトを定義
	//-----------------------------------------------
	//! 座標、向き、サイズ
	Transform m_transform;
	//! 移動
	Move m_move;
	//! 移動の際の当たり判定に使う
	BoxCollision m_move_hit_box;
	//! サウンドクラスのオブジェクト
	Sound m_se;
	//! モデルクラスのオブジェクト
	Model m_model;
	//! アニメーションクラスのオブジェクト
	Animation m_animation;
	//! エフェクトクラスオブジェクト
	Effect m_effect;
	//! コンボクラス
	Combo m_combo;
	//! カプセルコリジョン
	//=================
	// 当たり判定
	//=================
	CapsuleCollision m_right_hand; //!< 右手の当たり判定
	CapsuleCollision m_left_hand;   //!< 右手の当たり判定
	CapsuleCollision m_body;         //!< 本体の当たり判定
	CapsuleCollision m_left_feet;    //!< 左足の当たり判定
	CapsuleCollision m_right_feet;  //!< 右足の当たり判定


	//! 壁擦り判定のためにいったん座標を保存しておく変数
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
	//! 移動の際の当たり判定用のサイズ
	Vector3 m_move_hit_size = { 5.0f ,5.0f,5.0f };

	//===============
	// 状態管理用フラグ
	//=============== 
	 //-----------------------------------------------
	// 列挙体で管理
	//-----------------------------------------------
	//! プレイヤーの状態
	enum Situation
	{
		alive, // 生きてるとき
		die,   // 死んでる
	};
	//! 生きてるかを保存するための変数
	// 最初は生きてる状態から始める
	int m_life_and_death = alive;
	//! プレイヤーの状態
	enum PlayerMode
	{
		IDLE,      // 待機
		RUN,       // 走り状態
		ROLLING, // 回避
		COUNTER, // カウンター
		ATTACK,    // 攻撃
		HIT_DAMAGE, // 攻撃を受けた
		DIE        // 死亡
	};
	//! プレイヤーのモードを管理する変数
	int m_player_mode = 0;

	//! アイドル状態かのフラグ
	bool m_idle_flag = false;
	//! 走っていい以下のフラグ
	bool m_run_flag = false;
	//! ローリングフラグ
	bool m_rolling_flag = false;
	//! 攻撃状態かどおかのフラグ
	bool m_attack_flag = false;
	//! 攻撃を受けた時のフラグ
	bool m_damage_flag = false;
	//! 回避後のカウンター攻撃用のフラグ
	bool m_counter_flag = false;

	//! 攻撃を受けた時のアニメーションを設定するための変数
	//! なんかいもダメージを受けた時のアニメーションセットに入ってほしくないので作った
	//! 本来はもっといい方法があると思う見つけたらそちらに変更
	bool m_damage_anim_flag = false;
	//! 当たり判定をとっていいかのフラグ
	bool m_can_hit_damage_flag;

	

	//------------------------------------------
	// アニメーション、攻撃関連
	//------------------------------------------
	//! 今のアニメーション番号を保存する用の変数
	int m_now_attack_anim = 0;
	//! 攻撃アニメーションの始まりを保存する変数
	int m_ATTAK_START_ANIM_NO;
	//! 初めにしたい攻撃のアニメーション番号を保存する変数
	int m_first_attack_anim_1;
	int m_first_attack_anim_2;

	// 調査
	int aaa = 0;
	int bbb = 0;


	//! コンボの最大コンボ数用の変数
	int m_COMBO_MAX;
	//! コンボが何回続いているかのカウント
	int m_combo_count = 0;
protected:
	//------------------------------------------
	// コンボ関連
	//------------------------------------------
	//! コンボ用フラグ
	bool m_combo_flag = false;
	//! コンボの際マウスのどちらを押されたか
	int m_mouse_flag = 0; // MOUSE_INPUT_RIGHT : 右ボタン、MOUSE_INPUT_LEFT : 左ボタン 
	
	//! 次のアニメーション番号を保存
	int m_next_anim = -1;
	//! コンボをやめてほしい時のフラグ
	bool m_stop_combo_flag = false;


	//!  ゲームパッド用移動用ベクトル用変数
	Vector3 m_mov = { 0.0f,0.0f,0.0f };

public:
	//! 敵との移動当たり判定に使う
	float m_hit_r = 0.0f;

	//! プレイヤーの現在行っている攻撃番号を保存する(当たり判定に使用する)
	int m_now_attack = 0;

	//! HPの残量
	int m_hp_value = 0;
};