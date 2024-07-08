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
}

//---------------------------------------------------------------------------
// 移動に関する更新処理用関数
// Vector2 にｚ座標がないのでそこはｚ＝ｙとして使用している
//---------------------------------------------------------------------------
void TargetMove::Update(Transform* transform, const float mov_speed, const float rot_speed, Vector3* target_pos)
{
	// １個目のベクトル（本体前方向のベクトル）
	Vector2 front;
	front.x = 1.0f * cosf(TO_RADIAN(transform->rot.y));
	front.y = 1.0f * sinf(TO_RADIAN(transform->rot.y));

	// 2個目のベクトル（本体からターゲットがどっちの方向にいるかのベクトル）
	// Vector3をいったん置き換える
	// ターゲット座標
	Vector2 taget_poition{ target_pos->x,target_pos->z }; 
	// 本体の座標
	Vector2 pos{ transform->pos.x,transform->pos.z };
	Vector2 target = taget_poition - pos;
	// このベクトルを正規化する（長さを 1.0f にする）
	target.normalize();


	// 上で求めた２つのベクトル（front, target）の内積を取得します
	float front_dot = GetVector2Dot(front, target);

	// この取得した内積の値がプラスだった場合、プレイヤーはNPCの前にいると判断します
	if (front_dot >= 0.0f) {
		// プレイヤーの方向に向きを変える
		// プレイヤーがNPCから見てみぎにいるのかを判断したい
		// その判断をするためにNPCの右方向のベクトルを作成
		Vector2 right;
		// NPCの右向きの：NPCの向き（m_rot）に90度たした方向
		right.x = 1.0f * cosf(TO_RADIAN(transform->rot.y + 90.0f));
		right.y = 1.0f * sinf(TO_RADIAN(transform->rot.y + 90.0f));

		// 今作った右ベクトルとプレイヤーまでのベクトルの２つのベクトルの内積を取得
		float right_dot = GetVector2Dot(right, target);

		// この内積の値がプラスだったらプレイヤーはNPCからみて右にいる
		if (right_dot > 0.0f) {
			transform->rot.y += rot_speed;
		}
		// マイナスだった場合は左に回転
		if (right_dot < 0.0f) {
			transform->rot.y -= rot_speed;
		}


		// 向いている方向
		transform->pos.x += mov_speed * cosf(TO_RADIAN(transform->rot.y));
		transform->pos.z += mov_speed * sinf(TO_RADIAN(transform->rot.y));
	}
}
