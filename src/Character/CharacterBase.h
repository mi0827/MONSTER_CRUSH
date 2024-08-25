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

	//! @brief キャラの壁擦り判定用の関数
	//! @param キャラの座標
	//! @param 当たり判定相手のボックスの情報
	void MoveHitUpdate( BoxCollision* box);

	//! @brief 描画処理
	virtual void Draw() = 0;

	//! @brief 終了処理
	virtual void Exit() = 0;

	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ステータスバーの設定用関数
	virtual void Status_Bar_Init() = 0;
	//! @brief ステータスバーの描画関数
	virtual void Status_Bar_Draw() = 0;

	//! @brief ベースクラスでの初期処理
	//! @param 敵の移動の際に使う半径の設定
	//! @param HPの最大量
	void BaseInit(float r,int HP);


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
	Vector3 m_move_hit_size = { 1.0f ,0.0f,1.0f };
	//-----------------------------------------------
	// 変数の定義
	//-----------------------------------------------
	//! アイドル状態かのフラグ
	bool m_idle_flag = false;
	//! 走っていい以下のフラグ
	bool m_run_flag = false;
	//! ローリングフラグ
	bool m_rolling_flag = false;
	//! 攻撃状態かどおかのフラグ
	bool m_attack_flag = false;
	//! プレイヤーのモードを管理する変数
	int m_player_mode = -1;
	//! 今のアニメーション番号を保存する用の変数
	int m_now_attack_anim = -1;


	//!  ゲームパッド用移動用ベクトル用変数
	Vector3 m_mov = { 0.0f,0.0f,0.0f };

	//! 敵との移動当たり判定に使う
	float m_hit_r = 0.0f;
	
	
	//! プレイヤーの現在行っている攻撃アニメーション番号を保存する
	int m_now_attack = -1;
	// HPの残量
	int m_hp_value;
};