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
	// キャラとカメラの向きの回転の差
	m_difference_rot = player_rot->y - camera_rot->y;

	// 移動中はダッシュする
		// ゲームパッドの入力状態をとる
		//	ゲームパッドの左スティックの値を使って座標（ m_pos ）の値を変更
		// 左ステックでプレイヤーの向きや座標の更新
		// ゲームパッドの情報を取得（XINPUT の情報）
	XINPUT_STATE input = { {'\0'}, '\0','\0',0,0,0,0 };
	// ゲームパッドの情報を丸ごと取得
	// GetJoypadXInputState(DX_INPUT_PAD1, &input);
	// GetJoypadXInputState((int) pad_no, &input);
	//GetJoypadXInputState((int)((PAD_NO)pad_no), &input);
	switch (m_pad_no)
	{
	case PAD_NO::PAD_NO1:
		GetJoypadXInputState(DX_INPUT_PAD1, &input);
		break;
	case PAD_NO::PAD_NO2:
		GetJoypadXInputState(DX_INPUT_PAD2, &input);
		break;
	case PAD_NO::PAD_NO3:
		GetJoypadXInputState(DX_INPUT_PAD3, &input);
		break;
	case PAD_NO::PAD_NO4:
		GetJoypadXInputState(DX_INPUT_PAD4, &input);
		break;
	}
	// 左スティックの値を設定
	m_mov.x = input.ThumbLX;
	m_mov.z = input.ThumbLY;
	// -32768 ～ 32767 を-1.0f　～　1.0fにします
	m_mov /= 32768.0f;

	if (m_mov.GetLength() > 0.5f)
	{
		Move_GamePad(m_check_move, &m_mov, camera_rot, player_rot, player_pos, mov_speed);
	}

	// 押された瞬間に振り向きようの補完用カウントを初期化
	if (/*PushHitKey((KEY_INPUT_W)) ||*/ PushHitKey((KEY_INPUT_A)) || PushHitKey((KEY_INPUT_S)) || PushHitKey((KEY_INPUT_D)))
	{
		// 補完の値をリセットする
		m_rot_complementation = 0;
		// 一度押されたら振り向き保管を入れながらゆっくりキャラの向きを変える
		m_complementation_flag = true;
	}

	// WASDキーでプレイヤーの移動
	// 右上移動
	if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_W))
	{
		Move_Diagonally_Up_Right(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
	}
	else //左上移動
		if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_W))
		{
			Move_Diagonally_Up_Left(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
		}
		else // 右下移動
			if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_S))
			{
				Move_Oblique_Lower_Right(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
			}
			else // 左下移動
				if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_S))
				{
					Move_Oblique_Lower_Left(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
				}
				else // 上移動
					if (CheckHitKey(KEY_INPUT_W))
					{
						Move_Front(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
					}
					else // 下移動
						if (CheckHitKey(KEY_INPUT_S))
						{
							Move_Dhindo(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
						}
						else // 左移動
							if (CheckHitKey(KEY_INPUT_A))
							{
								Move_Left(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
							}
							else // 右移動
								if (CheckHitKey(KEY_INPUT_D))
								{
									Move_Right(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
								}


	
	// 補完状態なら
	if (m_complementation_flag)
	{
		m_rot_complementation += 5.0f;

		// 補完の値を増やす
		if (m_rot_flag)
		{
			// 補完されている値をプレイヤーの向きを変更する
			player_rot->y = camera_rot->y + m_rot_complementation;
		}
		else
		{
			// 補完されている値をプレイヤーの向きを変更する
			player_rot->y = camera_rot->y - m_rot_complementation;
		}

		// 補完の値が一定の値以上になったら
		if (m_rot_complementation >= m_rot_complementation_max)
		{
			//補完完了したのでフラグを下げる
			m_complementation_flag = false;
			//m_rot_complementation = 0;
		}
	}

}

//---------------------------------------------------------------------------
// キャラクターの壁擦り用関数(ボックス)
//---------------------------------------------------------------------------
void Move::Move_Hit(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
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
void Move::Move_Hit_Capsule(Vector3* player_pos, float r, CapsuleCollision* capsule)
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
void Move::Move_GamePad(bool* m_check_move, Vector3* mov, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
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
void Move::Move_Front(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	//　画面奥：カメラのある方向の逆の方向
	player_rot->y = camera_rot->y;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 後ろ移動
//---------------------------------------------------------------------------
void Move::Move_Dhindo(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 画面手前（カメラのある方向）
	m_rot_flag = true;
	m_rot_complementation_max = 180.0f;
	//player_rot->y = camera_rot->y + 180.0f;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 左移動
//---------------------------------------------------------------------------
void Move::Move_Left(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{

	// 画面から見て：左
	
	// 画面から見て：右
	if (m_difference_rot <= 90 && m_difference_rot >= -90)
	{
		m_rot_flag = false;
		m_rot_complementation_max = 90.0f;
	}
	else
	{
		m_rot_flag = true;
		m_rot_complementation_max = 90.0f;
	}
	if (m_complementation_flag == false)
	{
		player_rot->y = camera_rot->y - 90;
	}
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 右移動
//---------------------------------------------------------------------------
void Move::Move_Right(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 画面から見て：右
	if (m_difference_rot < 90 && m_difference_rot > -90)
	{
		m_rot_flag = true;
		m_rot_complementation_max = 90.0f;
	}
	else
	{
		m_rot_flag = false;
		m_rot_complementation_max = 90.0f;
	}
	if (m_complementation_flag == false)
	{
		player_rot->y = camera_rot->y +90;
	}
	
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
void Move::Move_Diagonally_Up_Right(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 画面から見て：右
	m_rot_flag = true;
	//player_rot->y = camera_rot->y + 45;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動		
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 左斜め上への移動
//---------------------------------------------------------------------------
void Move::Move_Diagonally_Up_Left(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 画面から見て：左
	m_rot_flag = false;
	// player_rot->y = camera_rot->y - 45;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動		
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 右斜め下への移動
//---------------------------------------------------------------------------
void Move::Move_Oblique_Lower_Right(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 画面手前（カメラのある方向）
	player_rot->y = camera_rot->y + 135.0f;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// 左斜め下への移動
//---------------------------------------------------------------------------
void Move::Move_Oblique_Lower_Left(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// 画面手前（カメラのある方向）
	player_rot->y = camera_rot->y + 225.0f;
	// 動いていい
	*m_check_move = true;
	// 向いている方向に座標移動
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}
