#pragma once

//! 
//! @fule CharacterBase.h
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



	//! @brief プレイヤーの状態(フラグ)管理関数
    //! @param プレイヤーの状態
	virtual void PlayerMode(int mode) = 0;


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



	//! @brief ベースクラスでの初期処理
	//! @param 敵の移動の際に使う半径の設定
	//! @param HPの最大量
	void BaseInit(float r, int HP);




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
	//! @brief 攻撃時の当たり判定を設定する用の関数
	//! @param 攻撃アニメーションの最大数
	void  NEW_Set_Attack_Hit_Damage(int attack_anim_max);

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


	//! モデルクラスのオブジェクト
	Model m_model;
	//! アニメーションクラスのオブジェクト
	Animation m_animation;
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


	//-----------------------------------------------
	// 変数の定義
	//-----------------------------------------------
	

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
		ATTACK,    // 攻撃
		HIT_DAMAGE, // 攻撃を受けた
		DIE        // 死亡
	};

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
	//! 攻撃を受けた時のアニメーションを設定するための変数
	//! なんかいもダメージを受けた時のアニメーションセットに入ってほしくないので作った
	//! 本来はもっといい方法があると思う見つけたらそちらに変更
	bool m_damage_anim_flag = false;
	//! プレイヤーのモードを管理する変数
	int m_player_mode = 0;
	//! 今のアニメーション番号を保存する用の変数
	int m_now_attack_anim = 0;



	//!  ゲームパッド用移動用ベクトル用変数
	Vector3 m_mov = { 0.0f,0.0f,0.0f };

	//! 敵との移動当たり判定に使う
	float m_hit_r = 0.0f;

	//! プレイヤーの現在行っている攻撃アニメーション番号を保存する
	int m_now_attack = 0;

	//! HPの残量
	int m_hp_value = 0;
};