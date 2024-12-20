#include "WinMain.h"
#include "src/System/Random.h"
#include "System/Vector3.h"
#include "System/Vector2.h"
#include "src/System/Spotlight.h"
#include "src/System/Transform.h"
#include "Camera.h"


//	この座標にマウスを固定しようと思います
#define FIXED_X		(SCREEN_W/2)	//	Ｘ座標
#define FIXED_Y		(SCREEN_H/2)	//	Ｙ座標




//---------------------------------------
// コンストラクタ(初期化)
//---------------------------------------
Camera::Camera()
{
	//! カメラ座標の初期設定
	m_pos.set(0.0f, 0.0f, -20.0f);
	//! カメラの向きは全部０度で開始
	m_rot.set(0.0f, 0.0f, 0.0f);

	//! カメラが見る座標
	m_look.set(0.0f, 0.0f, 0.0f); // すべて０．０ｆで初期化

	//! マウスの移動量の初期化最小は0.0fから
	m_mouse_move_x = 0.0f;
	m_mouse_move_y = 0.0f;
	//! パネルの大きさ(カメラを中心として扱うため半分の大きさを使う)
	// 今は使えてない
	m_hit_box_size.set(BOX_SIZE_HALF - (float)0.1, BOX_SIZE_HALF - (float)0.1, BOX_SIZE_HALF - (float)0.1);
	m_before_pos.set(m_pos); //< 移動前の座標の設定

	// カメラの見る位置と距離の設定
	SetCamera();

}

// 初期処理
void Camera::Init()
{
}

//---------------------------------------------------------------------------------
//	// プレイ画面の初期設定
//---------------------------------------------------------------------------------
void Camera::PlayFieldInit()
{
	//m_field_size.set(SCREEN_W , SCREEN_H);                                                                                          // 描画する画面のサイズの設定
	//m_screen_field = MakeScreen((int)m_field_size.x, (int)m_field_size.y);                                             // 描画画面を書き込むための初期設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -50.0f), VGet(0.0f, 0.0f, 1.0f));  // カメラの設定
}



//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void Camera::MouseCamera(Vector3* target_pos)
{
	//m_before_pos.set(m_pos); //< 移動前の座標の設定
	// プレイヤーの後ろに付いて動く
	m_look.set(target_pos->x, target_pos->y + m_look_height, target_pos->z);


	// カメラが地面に埋まらないようにしている
	if (m_rot.x <= LOWER_ANGLE)
	{
		m_rot.x = LOWER_ANGLE;
	}

	// カメラが真上にいかないようにしている
	if (m_rot.x >= UP_ANGLE_MAX)
	{
		m_rot.x = UP_ANGLE_MAX;
	}

	//==========================================
	// ゲームパッド用の処理
	//==========================================
	//	ゲームパッドの右スティックの値を使って向き変数（ m_rot ）の値を変更
	// 左ステックでプレイヤーの向きや座標の更新
	// ゲームパッドの情報を取得（XINPUT の情報）
	//XINPUT_STATE input;
	//// ゲームパッドの情報を丸ごと取得
	//GetJoypadXInputState(pad_no, &input);
	//// 移動用ベクトル用変数
	//Vector3 rot;
	//// 左スティックの値を設定
	//rot.y = input.ThumbRX;
	//rot.x = input.ThumbRY;
	//// -32768 ～ 32767 を-1.0f　～　1.0fにします
	//rot /= 32768.0f;
	//// この移動用ベクトルの大きさがある程度大きい時だけ移動させようと思います
	//if (rot.GetLength() > 0.5f)
	//{
	//	m_rot.y += rot.y * PAD_CAMERA_ROT_SPEED;
	//	m_rot.x -= rot.x * PAD_CAMERA_ROT_SPEED;
	//	// m_rot += rot * PAD_CAMERA_ROT_SPEED;  // その移動ベクトル分座標移動
	//}

	//	マウスの移動量
	Vector2 move;

	//	固定されたマウスのから移動用をゲット
	move.x = GetFixedMouseMoveX();
	move.y = GetFixedMouseMoveY();

	// 回転はそのままだと大きすぎるので小さく
	//移動量を小さくします
	move *= m_camera_mouse_sensi;
	m_rot.y += move.x;
	m_rot.x += move.y;


	// まずは回転前のベクトルを用意します
	// カメラが見るプレイヤー方向のベクトルを作成します
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// 行列を用意します
	// X軸回転行列
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	// Y軸回転行列
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));

	// X軸回転とY軸回転をさせたいので２つの行列を１個にまとめます
	MATRIX mat = MMult(mat_x, mat_y);
	// 元のベクトルをＸ軸回転とＹ軸回転させます
	// 簡単に言ったら一定の距離の棒を作っている
	VECTOR change_dir = VTransform(base_dir, mat);

	// カメラの位置を見ている座標から一定の位置に再設定
	m_pos = m_look + change_dir;
}

//---------------------------------------------------------------------------------
//	ターゲットカメラの更新処理
//---------------------------------------------------------------------------------
void Camera::TargetCamera(Vector3* target_pos1, Vector3* target_pos2)
{
	// 見たいターゲットの設定（今回は奥のターゲットを見る）
	m_look.set(target_pos1->x, target_pos1->y + m_look_height, target_pos1->z);

	// 手前のターゲットも見たいので
	m_look_2.set(target_pos2->x, target_pos2->y, target_pos2->z);

	Vector3 start;
	start.set(m_pos);
	Vector3 goal;
	goal.set(0.0f, 0.0f, 0.0f);
	Vector3 m_cross;
	m_cross.set(0.0f, 0.0f, 0.0f);

	// 基準のベクトル作成
	// ｚの値をいじると振り向きのがたがたがましになる
	Vector3 base(0.0f, 0.0f, 50.0f);
	// 線の本体の向きに合わせたいので回転行列を作成
	MATRIX mat = MGetRotY(TO_RADIAN(m_rot.y));
	// 上で作成した基準のベクトルを行列で変換
	Vector3 change = GetVector3VTransform(base, mat);
	// ゴール座標は開始座標から変更したベクトル文先のとこ
	goal = start + change;
	// スタート座標とゴール座標が確定したので開始座標かそれぞれのベクトルを作成
	// 線の開始座標からゴール座標へのベクトル
	Vector3 line_dir = goal - start;
	line_dir.y = 0;
	// 線の開始座標からプレイヤー座標へのベクトル
	Vector3 target_dir = *target_pos2 - start;
	target_dir.y = 0;
	// 外積を使った判断をしたいので上で作った２つのベクトルの外積を求めます
	m_cross = GetVector3Cross(line_dir, target_dir);
	// 上で作成したラインを見えるようにする
	{
		// そのままの座標だと線が地面に埋まってしまうのですこしあげています
		Vector3 start = start + Vector3(0.0f, 1.0f, 0.0f);
		Vector3 goal = goal + Vector3(0.0f, 1.0f, 0.0f);
		// 開始座標とゴール座標を結んで線の描画
		DrawLine3D(start.VGet(), goal.VGet(), GetColor(255, 255, 0));

		// 開始座標の場所とくろいたま
		DrawSphere3D(start.VGet(), 0.3f, 100, GetColor(0, 0, 0), GetColor(0, 0, 0), TRUE);
		// ゴール座標の黄色い玉
		DrawSphere3D(goal.VGet(), 0.3f, 100, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);
	}

	// 一定の範囲に入ったら振り向きをやめる
	if (m_cross.y > RANGE) {
		// 外積のＹの値がプラスの時はプレイヤーは線の右にいます
		m_rot.y += TARGET_ROT_SPEED;
	}
	else
		if (m_cross.y < -RANGE)
		{
			// 外積のＹの値がマイナスの時はプレイヤーは線の左にいます	
			m_rot.y -= TARGET_ROT_SPEED;
		}
	// まずは回転前のベクトルを用意します
   // カメラが見るプレイヤー方向のベクトルを作成します
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// 行列を用意します
	// X軸回転行列
//	MATRIX mat_x = MGetRotX(TO_RADIAN(target1->rot.y));
	// Y軸回転行列
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));

	// X軸回転とY軸回転をさせたいので２つの行列を１個にまとめます
//	MATRIX mat = MMult(mat_x, mat_y);
	// 元のベクトルをＸ軸回転とＹ軸回転させます
	// 簡単に言ったら一定の距離の棒を作っている
	VECTOR change_dir = VTransform(base_dir, mat_y);

	// カメラの位置を見ている座標から一定の位置に再設定
	m_pos = m_look + change_dir;
}

//---------------------------------------------------------------------------------
// カメラが壁に埋まらないようにする(うまくいかない)
//---------------------------------------------------------------------------------
void Camera::UseCameraUpdate(bool change_camera, Vector3* target_pos1, Vector3* target_pos2)
{
	if (change_camera)
	{
		MouseCamera(target_pos1);
	}
	else
	{
		TargetCamera(target_pos1, target_pos2);
	}
}

//---------------------------------------------------------------------------------
// カメラが壁に埋まらないようにする(うまくいかない)
//---------------------------------------------------------------------------------
void Camera::HitObject(Vector3* obj_pos, Vector3* obj_size)
{
	if (m_before_pos.x + m_hit_box_size.x >= obj_pos->x - obj_size->x && m_before_pos.x - m_hit_box_size.x <= obj_pos->x + obj_size->x)
	{
		m_pos.z = m_before_pos.z;
		m_pos.x = m_before_pos.x;
	}
	if (m_before_pos.z + m_hit_box_size.z >= obj_pos->z - obj_size->z && m_before_pos.z - m_hit_box_size.z <= obj_pos->z + obj_size->z)
	{
		m_pos.z = m_before_pos.z;
		m_pos.x = m_before_pos.x;
	}
}

//---------------------------------------------------------------------------------
//	描画前のカメラのセット
//---------------------------------------------------------------------------------
// めんどくさいけど頑張って工事しろ
void Camera::DrawSet()
{

}

//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void Camera::Draw()
{
	

	//	カメラの設定
	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));
	// カメラ座標と見る座標を渡してカメラの設定
	SetCameraPositionAndTarget_UpVecY(m_pos.VGet(), m_look.VGet());

	// Effekseerに3D表示の設定をDXライブラリの3D表示の設定に同期させる。
	Effekseer_Sync3DSetting();
}

//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void Camera::Exit()
{
}

//---------------------------------------------------------------------------------
//	カメラ移動の際のマウスの感度を設定
//---------------------------------------------------------------------------------
void Camera::SetCameraSensi(float mouse_sensi)
{
	// マウス感度にふさわしい値に修正してから保存する
	m_camera_mouse_sensi = mouse_sensi * MOUSE_SENSI_CORRECT;
}

//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void Camera::SetCamera(float height, float length)
{
	// カメラの見るY座標のプラス分の設定
	m_look_height = height;
	// カメラから目標までの距離の設定
	m_length = length;
}


//---------------------------------------------------------------------------------
//	移したい目標をまわるようにカメラの移動処理
//---------------------------------------------------------------------------------
void Camera::MoveCamera(Vector3* target_pos, int direction, float speed)
{
	// プレイヤーの後ろに付いて動く
	m_look.set(target_pos->x, target_pos->y + m_look_height, target_pos->z);

	if (direction == 0)
	{
		m_rot.y -= speed;
	}
	else // 左周り
	{
		m_rot.y += speed;
	}

	// まずは回転前のベクトルを用意します
	// カメラが見るプレイヤー方向のベクトルを作成します
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// 行列を用意します
	// X軸回転行列
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	// Y軸回転行列
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));

	// X軸回転とY軸回転をさせたいので２つの行列を１個にまとめます
	MATRIX mat = MMult(mat_x, mat_y);
	// 元のベクトルをＸ軸回転とＹ軸回転させます
	// 簡単に言ったら一定の距離の棒を作っている
	VECTOR change_dir = VTransform(base_dir, mat);

	// カメラの位置を見ている座標から一定の位置に再設定
	m_pos = m_look + change_dir;
}

//---------------------------------------------------------------------------------
// カメラシェイクを行う
//---------------------------------------------------------------------------------
void Camera::CameraShakeLimited(float power, float time)
{
	// 振動のパワーが0の時
	if (m_power == 0.0f)
	{
		// 振動パワーを設定
		m_power = power;
		m_shake_time = power / time;
	}

	// 揺れの強さを徐々に弱くする
	m_power -= m_shake_time;
	// 0より下回らないようにする
	m_power = max(0.0f, m_power);

	// 揺れがあるとき
	if (m_power > 0.0f)
	{
		// 揺らす
		m_shake_pos.x = GetRandomF(-m_power, m_power);
		m_shake_pos.y = GetRandomF(-m_power, m_power);
		m_shake_pos.z = GetRandomF(-m_power, m_power);
	}
	else
	{
		// そうじゃないときは揺れなし
		m_shake_pos.clear();
	}
	// ずれを含めた座標
	m_pos += m_shake_pos;
}


//---------------------------------------------------------------------------------
// 向きの設定用関数
//---------------------------------------------------------------------------------
void Camera::SetCameraRot(Vector3 rot)
{
	m_rot.y = rot.y;
	// まずは回転前のベクトルを用意します
	// カメラが見るプレイヤー方向のベクトルを作成します
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// 行列を用意します
	// X軸回転行列
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	// Y軸回転行列
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));

	// X軸回転とY軸回転をさせたいので２つの行列を１個にまとめます
	MATRIX mat = MMult(mat_x, mat_y);
	// 元のベクトルをＸ軸回転とＹ軸回転させます
	// 簡単に言ったら一定の距離の棒を作っている
	VECTOR change_dir = VTransform(base_dir, mat);

	// カメラの位置を見ている座標から一定の位置に再設定
	m_pos = m_look + change_dir;
}

//-------------------------------------------------------------------------------- -
// カメラん向きを取得する関数
//---------------------------------------------------------------------------------
Vector3 Camera::GetCameraRot()
{
	return m_rot;
}
