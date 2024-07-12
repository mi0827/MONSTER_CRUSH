#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/Collision/BoxCollision.h"
#include "src/System/Transform.h"
#include "TargetMove.h"

TargetMove::TargetMove()
{
}

TargetMove::~TargetMove()
{
	// ポインタの解放
	//delete m_transform;
	//delete m_target;
}

//---------------------------------------------------------------------------
// 情報をセットする用の関数
//---------------------------------------------------------------------------
void TargetMove::SetInfo(Information* info, TargetInformation* target_info)
{
	// 自身の情報をセット
	m_info = *info;
	// ターゲットのセット
	m_target_info = *target_info;
}

//---------------------------------------------------------------------------
// 移動に関する更新処理用関数
// Vector2 にｚ座標がないのでそこはｚ＝ｙとして使用している
//---------------------------------------------------------------------------
void TargetMove::Update()
{
	// １個目のベクトル（本体前方向のベクトル）
	Vector2 front;
	front.x = 1.0f * cosf(TO_RADIAN(m_info.m_transform->rot.y));
	front.y = 1.0f * sinf(TO_RADIAN(m_info.m_transform->rot.y));

	// 2個目のベクトル（本体からターゲットがどっちの方向にいるかのベクトル）
	// Vector3をいったん置き換える
	// ターゲット座標
	Vector2 taget_poition{ m_target_info.m_target->x,m_target_info.m_target->z };
	// 本体の座標
	Vector2 pos{ m_info.m_transform->pos.x,m_info.m_transform->pos.z };
	Vector2 target = taget_poition - pos;

	// ターゲットとの距離が見えるようにしている
	// 後で消す
	printfDx("距離%3f", target);

	// このベクトルを正規化する（長さを 1.0f にする）
	target.normalize();



	// 上で求めた２つのベクトル（front, target）の内積を取得します
	float front_dot = GetVector2Dot(front, target);

	// この取得した内積の値がプラスだった場合、プレイヤーはNPCの前にいると判断します
	//if (front_dot >= 0.0f) {
		// プレイヤーの方向に向きを変える
		// プレイヤーがNPCから見てみぎにいるのかを判断したい
		// その判断をするためにNPCの右方向のベクトルを作成
	Vector2 right;
	// NPCの右向きの：NPCの向き（m_rot）に90度たした方向
	right.x = 1.0f * cosf(TO_RADIAN(m_info.m_transform->rot.y + 90.0f));
	right.y = 1.0f * sinf(TO_RADIAN(m_info.m_transform->rot.y + 90.0f));

	// 今作った右ベクトルとプレイヤーまでのベクトルの２つのベクトルの内積を取得
	float right_dot = GetVector2Dot(right, target);

	// この内積の値がプラスだったらプレイヤーはNPCからみて右にいる
	if (right_dot > 0.0f) {
		m_info.m_transform->rot.y += m_info.M_ROT_SPEED;
	}
	// マイナスだった場合は左に回転
	if (right_dot < 0.0f) {
		m_info.m_transform->rot.y -= m_info.M_ROT_SPEED;
	}


	// 向いている方向
	m_info.m_transform->pos.x += m_info.M_MOV_SPEED * cosf(TO_RADIAN(m_info.m_transform->rot.y));
	m_info.m_transform->pos.z += m_info.M_MOV_SPEED * sinf(TO_RADIAN(m_info.m_transform->rot.y));
	/*}*/
}

void TargetMove::Target_Hit()
{
}
