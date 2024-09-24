#include "WinMain.h"
#include "System/Vector3.h"
#include "System/Vector2.h"
#include "src/System/Spotlight.h"
#include "src/System/Transform.h"
#include "Camera.h"


constexpr float CAMERA_LENGTH = 50.0f;          // プレイヤーからカメラまでの距離
// カメラの回転スピード
constexpr float MOUSE_CAMERA_ROT_SPEED = 0.2f;  // マウス用
constexpr float PAD_CAMERA_ROT_SPEED = 3.0f;    // パッド用
constexpr float UP_ANGLE_MAX = 30.0f;           // カメラの上アングルの最大
constexpr float LOWER_ANGLE = -5.0f;            // カメラの下アングルの最低（地面に埋まらない程度）
constexpr float BOX_SIZE = 4.0f;                   // ボックスのサイズ
constexpr float BOX_SIZE_HALF = (BOX_SIZE / 2.0f); // 半数のサイズ

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
}

// 初期処理
void Camera::Init()
{
}

//---------------------------------------------------------------------------------
//	// プレイ画面の初期設定
//---------------------------------------------------------------------------------
void Camera::PlayField_Init()
{
	//m_field_size.set(SCREEN_W , SCREEN_H);                                                                                          // 描画する画面のサイズの設定
	//m_screen_field = MakeScreen((int)m_field_size.x, (int)m_field_size.y);                                             // 描画画面を書き込むための初期設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -50.0f), VGet(0.0f, 0.0f, 1.0f));  // カメラの設定
}

//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void Camera::Update(Vector3* target_pos)
{
	m_before_pos.set(m_pos); //< 移動前の座標の設定
	// プレイヤーの後ろに付いて動く
	m_look.set(target_pos->x, target_pos->y + 5.0f, target_pos->z);
	// マウスの移動量
	m_mouse_move_x = (float)GetMouseMoveX();
	m_mouse_move_y = (float)GetMouseMoveY();

	// カメラの向きを変える
	// （２Dのマウスの移動量をそのまま角度に渡すと大きいので少し小さくしています）
	m_rot.x += m_mouse_move_y * MOUSE_CAMERA_ROT_SPEED;
	m_rot.y += m_mouse_move_x * MOUSE_CAMERA_ROT_SPEED;

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

	//	ゲームパッドの右スティックの値を使って向き変数（ m_rot ）の値を変更
	// 左ステックでプレイヤーの向きや座標の更新
	// ゲームパッドの情報を取得（XINPUT の情報）
	XINPUT_STATE input;
	// ゲームパッドの情報を丸ごと取得
	GetJoypadXInputState(pad_no, &input);
	// 移動用ベクトル用変数
	Vector3 rot;
	// 左スティックの値を設定
	rot.y = input.ThumbRX;
	rot.x = input.ThumbRY;
	// -32768 ～ 32767 を-1.0f　～　1.0fにします
	rot /= 32768.0f;
	// この移動用ベクトルの大きさがある程度大きい時だけ移動させようと思います
	if (rot.GetLength() > 0.5f)
	{
		m_rot.y += rot.y * PAD_CAMERA_ROT_SPEED;
		m_rot.x -= rot.x * PAD_CAMERA_ROT_SPEED;
		// m_rot += rot * PAD_CAMERA_ROT_SPEED;  // その移動ベクトル分座標移動
	}

	// まずは回転前のベクトルを用意します
	// カメラが見るプレイヤー方向のベクトルを作成します
	VECTOR base_dir = VGet(0.0f, 0.0f, -CAMERA_LENGTH);

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
// カメラが壁に埋まらないようにする(うまくいかない)
//---------------------------------------------------------------------------------
void Camera::Hit_Object(Vector3* obj_pos, Vector3* obj_size)
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
void Camera::Draw_Set()
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
	
}

//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void Camera::Exit()
{
}

//---------------------------------------------------------------------------------
//	移したい目標をまわるようにカメラの移動処理
//---------------------------------------------------------------------------------
void Camera::MoveCamera(Vector3* target_pos, int direction, float speed)
{
	// プレイヤーの後ろに付いて動く
	m_look.set(target_pos->x, target_pos->y + 5.0f, target_pos->z);
	
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
	VECTOR base_dir = VGet(0.0f, 0.0f, -CAMERA_LENGTH);

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
