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
	//! @param キャラの動く前の座標
	//! @param キャラの当たり判定のサイズ
	//! @param 当たり判定相手のボックスの情報
	void MoveHitUpdate(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box);

	//! @brief 描画処理
	virtual void Draw() = 0;

	//! @brief 終了処理
	virtual void Exit() = 0;

	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ステータスバーの設定用関数
	virtual void Suatus_Bar_Init() = 0;
	//! @brief ステータスバーの描画関数
	virtual void Status_Bar_Draw() = 0;

	//! @brief ベースクラスでの初期処理
	//! @param 敵の移動の際に使う半径の設定
	void BaseInit(float r);



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
	//-----------------------------------------------
	//! 変数の定義
	//-----------------------------------------------
	//!  ゲームパッド用移動用ベクトル用変数
	Vector3 m_mov = { 0.0f,0.0f,0.0f };

	//! 敵との移動当たり判定に使う
	float m_hit_r = 0.0f;

};