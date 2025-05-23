#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/Collision/BoxCollision.h"
#include "src/Model/Model.h"
#include "src/Collision/CapsuleCollision.h"
#include "Move.h"


Move::Move()
{
}


Move::~Move()
{
}


//---------------------------------------------------------------------------
// 移動に関する更新処理用関数
//---------------------------------------------------------------------------
void Move::Update(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, const float* mov_speed, Vector3* player_pos, PAD_NO m_pad_no, Vector3 m_mov)
{	
	// 振り向き処理
	LookingUpdate(player_rot, camera_rot);
	// キャラとカメラの向きの回転の差
	//m_difference_rot = player_rot->y - camera_rot->y;
	
	// モデルのY軸回転に制限をかける
	
	// 移動中はダッシュする
    // ゲームパッドの入力状態をとる
	//	ゲームパッドの左スティックの値を使って座標（ m_pos ）の値を変更
	// 左ステックでプレイヤーの向きや座標の更新
	// ゲームパッドの情報を取得（XINPUT の情報）
	XINPUT_STATE input = { {'\0'}, '\0','\0',0,0,0,0 };
	// ゲームパッドの情報を丸ごと取得
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	
	// 左スティックの値を設定
	m_mov.x = input.ThumbLX;
	m_mov.z = input.ThumbLY;
	// -32768 〜 32767 を-1.0f　〜　1.0fにします
	m_mov /= 32768.0f;
	if (m_mov.GetLength() > 0.5f)
	{
		MoveGamePad(m_check_move, &m_mov, camera_rot, player_rot, player_pos, mov_speed);
	}
	
	// WASDキーでプレイヤーの移動
	// 右上移動
	if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_W))
	{
		MoveDiagonallyUpRight(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
	}
	else //左上移動
		if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_W))
		{
			MoveDiagonallyUpLeft(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
		}
		else // 右下移動
			if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_S))
			{
				MoveObliqueLowerRight(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
			}
			else // 左下移動
				if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_S))
				{
					MoveObliqueLowerLeft(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
				}
				else // 上移動
					if (CheckHitKey(KEY_INPUT_W))
					{
						MoveFront(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
					}
					else // 下移動
						if (CheckHitKey(KEY_INPUT_S))
						{
							MoveDhindo(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
						}
						else // 左移動
							if (CheckHitKey(KEY_INPUT_A))
							{
								MoveLeft(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
							}
							else // 右移動
								if (CheckHitKey(KEY_INPUT_D))
								{
									MoveRight(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
								}
}

//---------------------------------------------------------------------------
// キャラクターの壁擦り用関数(ボックス)
//---------------------------------------------------------------------------
void Move::MoveHit(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
{
	if (before_pos->x + hit_size->x >= box->m_box.hit_pos.x - box->m_box.half_size.x && before_pos->x - hit_size->x <= box->m_box.hit_pos.x + box->m_box.half_size.x)
	{
		// 縦方向だけ戻す
		player_pos->z = before_pos->z;
	}
	if (before_pos->z + hit_size->z >= box->m_box.hit_pos.z - box->m_box.half_size.z && before_pos->z - hit_size->z <= box->m_box.hit_pos.z + box->m_box.half_size.z)
	{
		// 縦方向だけ戻す
		player_pos->x = before_pos->x;
	}
}


//---------------------------------------------------------------------------
// キャラクターの壁擦り用関数(円)
//---------------------------------------------------------------------------
void Move::MoveHitCapsule(Vector3* player_pos, float r, CapsuleCollision* capsule)
{
	// それぞれの更新処理が終わったのでプレイヤーとNPCの位置関係から一定距離近づかないようにします
	// １：プレイヤーとNPCの距離を求める
	float distance = GetVector3Distance(*player_pos, capsule->m_capsule.pos1);
	// ２：基準の距離を求める（それぞれの半径）
	float radius = r + capsule->m_capsule.radius;

	// ３：２キャラの距離が基準の長さよりも短かったら
	if (distance < radius) {
		// 4：どれくらい中に入っているか（この長さ分だけ円が重なっている
		float in_lengef = radius - distance;
		// ５：どのずらすか
		Vector3 dir = *player_pos - capsule->m_capsule.pos1;

		// ６：このベクトルの長さを中に入っていいる長さに半分にします
		dir.SetLength(in_lengef);
		// 7：この分だけプレイヤー座標を移動させる
		*player_pos += dir;

	}
}

//---------------------------------------------------------------------------
// ゲームパッドの移動用関数(ゲームパッド用)
//---------------------------------------------------------------------------
void Move::MoveGamePad(bool* m_check_move, Vector3* mov, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{

	*m_check_move = true; // 動いていい
	// 向いている方向に座標移動
	// 当たり判定がある時は座標移動をしない
	//	そのベクトルを回転させるための行列を作成します。
	MATRIX mat_y = MGetRotY(TO_RADIAN(camera_rot->y));
	//	その行列を使ってベクトルを回転させます。
	*mov = GetVector3VTransform(*mov, mat_y);
	// 移動ベクトルのｘｚのから向きを求めます
	player_rot->y = TO_DEGREE(atan2f(mov->x, mov->z));
	// 移動ベクトルの大きさを PLAYER_MOV_SPEED のおおきさにします
	mov->SetLength(*mov_speed);
	// その移動ベクトル分座標移動
	*player_pos += *mov;

}

//---------------------------------------------------------------------------
// 前移動
//---------------------------------------------------------------------------
void Move::MoveFront(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = 0;
	// 補完フラグを上げる
	m_complementation_flag = true;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 後ろ移動
//---------------------------------------------------------------------------
void Move::MoveDhindo(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = 180;
	// 補完フラグを上げる
	m_complementation_flag = true;

	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 左移動
//---------------------------------------------------------------------------
void Move::MoveLeft(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = -90;
	// 補完フラグを上げる
	m_complementation_flag = true;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 右移動
//---------------------------------------------------------------------------
void Move::MoveRight(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = 90;
	// 補完フラグを上げる
	m_complementation_flag = true;
	
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動	
	Vector3 move;
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 右斜め上への移動
//---------------------------------------------------------------------------
void Move::MoveDiagonallyUpRight(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = 45;
	// 補完フラグを上げる
	m_complementation_flag = true;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動		
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 左斜め上への移動
//---------------------------------------------------------------------------
void Move::MoveDiagonallyUpLeft(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = -45;
	// 補完フラグを上げる
	m_complementation_flag = true;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動		
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 右斜め下への移動
//---------------------------------------------------------------------------
void Move::MoveObliqueLowerRight(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = 135;
	// 補完フラグを上げる
	m_complementation_flag = true;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 左斜め下への移動
//---------------------------------------------------------------------------
void Move::MoveObliqueLowerLeft(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 向てほしい方向セット
	m_face_num = 225;
	// 補完フラグを上げる
	m_complementation_flag = true;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// キャラクターの振り向き処理
//---------------------------------------------------------------------------
void Move::LookingUpdate(Vector3* chara_rot, Vector3* camera_rot)
{
	// 振り向きフラグが上がっているとき
	if (m_complementation_flag)
	{
		// キャラのY軸回転行列
		m_character_mat = MGetRotY(TO_RADIAN(chara_rot->y));
		// プレイヤーの前方向のベクトル
		m_character_dir = VGet(m_character_mat.m[2][0], m_character_mat.m[2][1], m_character_mat.m[2][2]);
		// 向かせたい方向のY軸回転行列
		m_face_mat = MGetRotY(TO_RADIAN(camera_rot->y + m_face_num)); // カメラから見た時の右方向
		// 向かせたい方向ベクトル
		m_face_dir = VGet(m_face_mat.m[2][0], m_face_mat.m[2][1], m_face_mat.m[2][2]);
		// キャラの前方ベクトルと向いてほしい方向ベクトルの二つのベクトルの内積を求める
		inner_product = GetVector3Dot(m_character_dir, m_face_dir);
		// キャラ前方方向ベクトルと向かせたい方向の二つのベクトルの外積を求める
		cross_product = GetVector3Cross(m_character_dir, m_face_dir);
		// 外積によって方向を変える
		if (cross_product.y > 0)
		{
			if (inner_product <= 0.99)
			{
				chara_rot->y += 10.0f;
			}
			else
			{
				// 振り向き処理をやめる
				m_complementation_flag = false;
			}
		}
		else
		{
			if (inner_product <= 0.99)
			{
				chara_rot->y -= 10.0f;
			}
			else
			{
				// 振り向き処理をやめる
				m_complementation_flag = false;
			}
		}
	}
	
}
