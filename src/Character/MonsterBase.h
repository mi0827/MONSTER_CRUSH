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
	virtual void Update(Transform* traget_pos, float traget_r) = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;


	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ステータスバーの設定用関数
	virtual void Status_Bar_Init() = 0;
	//! @brief ステータスバーの描画関数
	virtual void Status_Bar_Draw() = 0;

	//! @brief ローリングアクション用の関数（回避）
	//! @param ローリングしたときの移動スピード
	virtual void Action_Rolling(const int rolling_speed);

	//! @brief ベースクラスでの初期処理
	//! @param モンスターのHPマックス
	//! @param ジャンプの上昇スピード
	//! @param ジャンプの下降スピード
	void BaseInit(int hp_num ,float up_speed, float down_speed);

	//! @brief モンスターの移動に関するターゲットの設定
	//! @param ターゲットの座標
	//! @param ターゲットのカプセルの当たり判定の半径
	void BaseSetTarget(Transform* target_pos, const float m_target_hit_r);

	//! @brief ベースクラスの更新処理
	//! @param  歩いている状態かのフラグ
	void BaseUpdate(bool* run_flag);




	// 攻撃の時の当たり判定とダメージの構造体
	// 各子クラスで定義する
	struct Attack_Hit_Damage
	{
		//!  攻撃時に使いたい当たり判定
		CapsuleCollision attack_hit;
		//! 攻撃にあったダメージ
		int attack_damage = 0;
	};
	std::vector< Attack_Hit_Damage*> m_attack_hit_damage;

	//! @brief 攻撃時の当たり判定を設定する用の関数
	//! @param 攻撃アニメーションの最大数
	void  NEW_Set_Attack_Hit_Damage(int attack_anim_max);


	//! @brief キャラの壁擦り判定用の関数
	//! @param キャラの座標
	//! @param キャラの動く前の座標
	//! @param キャラの当たり判定のサイズ
	//! @param 当たり判定相手のボックスの情報
	//void MoveHitUpdate(Vector3* monster_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box);
public:

	//-----------------------------------------------
	// 変数の宣言
	//-----------------------------------------------
	//! アイドル状態かのフラグ
	bool m_idle_flag = false;
	//! 走っていい以下のフラグ
	bool m_run_flag = false;

	//! 攻撃状態かどおかのフラグ
	bool m_attack_flag = false;
	//! プレイヤーのモードを管理する変数
	int m_monster_mode = 0;
	//! 今のアニメーション番号を保存する用の変数
	int m_now_attack_anim = 0;


	//! 壁擦り判定のためにいったん座標を保存しておく変数
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
	//! 移動の際の当たり判定用のサイズ
	Vector3 move_hit_size = { 1.0f ,0.0f,1.0f };

	//! 自身の半径を入れる用の変数
	float m_hit_r = 10.0f;
	//! 自身の移動スピード
	static constexpr float M_MOV_SPEED = 1.0f;
	//! 自身回転速度
	static constexpr float M_ROT_SPEED = 5.0f;

	// HPの残量
	int m_hp_value;

	//------------------------------------------
	// ジャンプ関連
	//------------------------------------------
	// ジャンプのための移動量用の変数
	Vector3 m_jump_mov{ 0.0f,0.0f,0.0f };
	// ジャンプフラグ
	bool m_jump_flag = false;
	// 上昇スピード
	float m_up_speed = 0;
	// 下降スピード
	float m_down_speed = 0;

	//------------------------------------------
	// コンボ関連
	//------------------------------------------
	// コンボの最大数
	static constexpr int COMBO_MAX = 3;
	//! コンボ用フラグ
	bool m_combo_flag = false;

	//! 動いてほしいかのフラグ

	//! 次のアニメーション番号を保存
	int m_next_anim = -1;
	//! コンボをやめてほしい時のフラグ
	bool m_stop_combo_flag = false;


	//! モンスターの現在行っている攻撃アニメーション番号を保存する
	int m_now_attack = -1;

	//-----------------------------------------------
	// クラスのオブジェクトの宣言
	//-----------------------------------------------
	//! 自身の情報を入れる変数
	Transform m_transform;
	//! 移動に使うクラス
	TargetMove move;
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
	CapsuleCollision m_left_hand; //!< 左手のあたり判定
	CapsuleCollision m_right_hand; //!< 右手の当たり判定
	CapsuleCollision m_body;       //!< 本体のあたり判定

};
