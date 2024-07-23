#pragma once

/**
* @file Move.h
* @ brief キャラの移動用のクラス
*/
class Move
{
public:

	Move();
	~Move();

	//!
	//! @fn
	//! @brief 更新処理
	//! @param 今動いていいかの変数
	//! @param カメラの向き
	//! @param プレイヤーの向いている向き
	//! @param 移動スピード
	//! @param プレイヤーの座標
	//! @param パッドの番号
	//! @param スティックの値
	//! @detail 移動に関するものの更新処理
	//! 
	void Update(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, const float* mov_speed, Vector3* player_pos, PAD_NO m_pad_no, Vector3 m_mov);

	//!
	//! @fn
	//! @brief 移動に関する当たり判定用の関数
	//! @param 自分 (キャラ) の座標(今の座標)
	//! @param 自分（キャラ）の座標(当たり判定前の)
	//! @param 自分（キャラ）の当たり判定のサイズ（中心から見たサイズ）
	//! @param 当たり判定をとりたいボックスの情報
	//! @detail 移動の壁擦り用関数
	void Move_Hit(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box);

	//! 
	//! @brief 移動に関する当てり判定用の関数
	// !@param 自分 (キャラ) の座標(今の座標)
	//! @param 自分（キャラ）の座標(当たり判定前の)
	//! @param 自分（キャラ）の当たり判定のサイズ（中心から見たサイズ）
	//! @param 当たり判定をとりたいボックスの情報
	//! @detail 移動の壁擦り用関数
	void Move_Hit_Capsule(Vector3* player_pos, float r, CapsuleCollision* capsule);
	
	//! @fn
	//! @brief ゲームパッドでの移動用関数
	//! @param 動いていいのかの変数
	//! @param スティックの値
	//! @param キャラの向いている向き
	//! @param キャラの座標
	//! @param 移動スピード
	//! @detail ゲームパッドでの移動用関数
	//! 
	void Move_GamePad(bool* m_check_move, Vector3* mov, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);

	//!
	//! @fn
	//! @brief カメラの方向に対して真っ直ぐへの移動用関数
	//! @param 動いていいのかの変数
	//! @param カメラの向き
	//! @param キャラの向いている向き
	//! @param キャラの座標
	//! @param 移動スピード
	//! @detail カメラの方向に対して真っ直ぐの移動用関数
	//! 
	void Move_Front(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);

	//!
	//! @fn
	//! @brief カメラの方向に対して反対への移動用関数
	//! @param 動いていいのかの変数
	//! @param カメラの向き
	//! @param キャラの向いている向き
	//! @param 移動スピード
	//! @detail カメラの方向に対して反対の移動用関数
	//! 
	void Move_Dhindo(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);


	//! 
	//! @fn
	//! @brief カメラの方向に対して左への移動用関数
	//! @param 動いていいのかの変数
	//! @param カメラの向き
	//! @param キャラの向いている向き
	//! @param 移動スピード
	//! @detail カメラの方向に対して左への移動用関数
	//! 
	void Move_Left(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);


	//!
	//! @fn
	//! @brief カメラの方向に対して右への移動用関数
	//! @param 動いていいのかの変数
	//! @param カメラの向き
	//! @param キャラの向いている向き
	//! @param 移動スピード
	//! @detail カメラの方向に対して右への移動用関数
	//! 
	void Move_Right(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);


	//! @fn
	//! @brief カメラの方向に対して右斜め上への移動用関数
	//! @param 動いていいのかの変数
	//! @param カメラの向き
	//! @param キャラの向いている向き
	//! @param 移動スピード
	//! @detail カメラの方向に対して右斜め上への移動用関数
	void Move_Diagonally_Up_Right(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);

	//! @fn
	//! @brief カメラの方向に対して左斜め上への移動用関数
	//! @param １：動いていいのかの変数
	//! @param ２：カメラの向き
	//! @param ３：キャラの向いている向き
	//! @param ４：移動スピード
	//! @detail カメラの方向に対して左斜め上への移動用関数
	void Move_Diagonally_Up_Left(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);

	//! @brief カメラの方向に対して右斜め下への移動用関数
	//! @param 動いていいのかの変数
	//! @param カメラの向き
	//! @param キャラの向いている向き
	//! @param 移動スピード
	//! @detail カメラの方向に対して右斜め下への移動用関数
	void Move_Oblique_Lower_Right(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);


	//! @brief カメラの方向に対して左斜め下への移動用関数
	//! @param 動いていいのかの変数
	//! @param カメラの向き
	//! @param キャラの向いている向き
	//! @param 移動スピード
	//! @detail カメラの方向に対して左斜め下への移動用関数
	void Move_Oblique_Lower_Left(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed);

public:
	////! プレイヤーの座標を入れておくもの
	//Vector3 m_player_pos = { 0.0f,0.0f,0.0f };

	////! プレイヤーの移動スピードを入れておくもの
	//// static const float player_mov_speed;

	////! プレイヤーの向きを入れておくもの
	//Vector3 m_player_rot = { 0.0f,0.0f,0.0f };

	////! カメラの向きを入れておくもの
	//Vector3 m_camera_rot = { 0.0f,0.0f,0.0f };
};