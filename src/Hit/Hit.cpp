#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"
#include "src/Model/Model.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"
#include "Hit.h"

////---------------------------------------------------------------------------------
////	円同士の当たり判定
////---------------------------------------------------------------------------------
//bool CheckCircleHit(Vector2& center1, float radius1, Vector2& center2, float radius2)
//{
//	float length = GetVector2Distance(center1, center2);
//	float radius = radius1 + radius2;
//	if (length <= radius) {
//		return true;
//	}
//	return false;
//}
////---------------------------------------------------------------------------------
////	点と円の当たり判定
////---------------------------------------------------------------------------------
//bool CheckPointCircleHit(Vector2& point, Vector2& center, float radius)
//{
//	float length = GetVector2Distance(point, center);
//	if (length <= radius) {
//		return true;
//	}
//	return false;
//}
////---------------------------------------------------------------------------------
////	四角同士の当たり判定
////---------------------------------------------------------------------------------
//bool CheckBoxHit(Vector2& box_pos1, Vector2& box_size1, Vector2& box_pos2, Vector2& box_size2)
//{
//	if (box_pos1.x + box_size1.x >= box_pos2.x && box_pos1.x <= box_pos2.x + box_size2.x) {
//		if (box_pos1.y + box_size1.y >= box_pos2.y && box_pos1.y <= box_pos2.y + box_size2.y) {
//			return true;
//		}
//	}
//	return false;
//}

//---------------------------------------------------------------------------------
//	点と四角の当たり判定
//---------------------------------------------------------------------------------
bool CheckPointBoxHit(Vector2& point, Vector2& box_pos, Vector2& box_size)
{
	if (point.x >= box_pos.x && point.x <= box_pos.x + box_size.x) {
		if (point.y >= box_pos.y && point.y <= box_pos.y + box_size.y) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------------
//	球同士の当たり判定
//---------------------------------------------------------------------------------
bool CheckBallHit(Vector3 center1, float radius1, Vector3 center2, float radius2)
{
	// 直線距離を出す
	float length = GetVector3Distance(center1, center2);
	// 半径同士を足した値を出す
	float radius = radius1 + radius2;
	// 座標同士の距離が半径同士の距離より短くなったら
	if (length <= radius) {
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------
//	ボックス同士の当たり判定
//	（座標 pos を中心にサイズ size 分プラスマイナスしたボックス：２Ｄのような左上からの大きさでの判定ではないので注意）
//---------------------------------------------------------------------------------
bool CheckBoxHit3D(Vector3 box_pos1, Vector3 box_size1, Vector3 box_pos2, Vector3 box_size2)
{
	float right1 = box_pos1.x + box_size1.x;
	float left1 = box_pos1.x - box_size1.x;
	float up1 = box_pos1.y + box_size1.y;
	float down1 = box_pos1.y - box_size1.y;
	float front1 = box_pos1.z + box_size1.z;
	float back1 = box_pos1.z - box_size1.z;

	float right2 = box_pos2.x + box_size2.x;
	float left2 = box_pos2.x - box_size2.x;
	float up2 = box_pos2.y + box_size2.y;
	float down2 = box_pos2.y - box_size2.y;
	float front2 = box_pos2.z + box_size2.z;
	float back2 = box_pos2.z - box_size2.z;

	if (right1 >= left2 && left1 <= right2)
	{
		if (up1 >= down2 && down1 <= up2)
		{
			if (front1 >= back2 && back1 <= front2)
			{
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------------
//	ボックス同士の当たり判定
//	（座標 pos を中心にサイズ size 分プラスマイナスしたボックス：２Ｄのような左上からの大きさでの判定ではないので注意）
//---------------------------------------------------------------------------------
bool CheckBoxHit(BoxCollision box1, BoxCollision box2)
{
	float right1 = box1.m_box.hit_pos.x + box1.m_box.half_size.x;
	float left1 = box1.m_box.hit_pos.x - box1.m_box.half_size.x;
	float up1 = box1.m_box.hit_pos.y + box1.m_box.half_size.y;
	float down1 = box1.m_box.hit_pos.y - box1.m_box.half_size.y;
	float front1 = box1.m_box.hit_pos.z + box1.m_box.half_size.z;
	float back1 = box1.m_box.hit_pos.z - box1.m_box.half_size.z;

	float right2 = box2.m_box.hit_pos.x + box2.m_box.half_size.x;
	float left2 = box2.m_box.hit_pos.x - box2.m_box.half_size.x;
	float up2 = box2.m_box.hit_pos.y + box2.m_box.half_size.y;
	float down2 = box2.m_box.hit_pos.y - box2.m_box.half_size.y;
	float front2 = box2.m_box.hit_pos.z + box2.m_box.half_size.z;
	float back2 = box2.m_box.hit_pos.z - box2.m_box.half_size.z;

	if (right1 >= left2 && left1 <= right2)
	{
		if (up1 >= down2 && down1 <= up2)
		{
			if (front1 >= back2 && back1 <= front2)
			{
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------------
//	カプセル同士の当たり判定
//	（座標 pos を中心にサイズ size 分プラスマイナスしたボックス：２Ｄのような左上からの大きさでの判定ではないので注意）
//---------------------------------------------------------------------------------
bool CheckCapsuleHit(CapsuleCollision capsule1, CapsuleCollision capsule2)
{
	return HitCheck_Capsule_Capsule(capsule1.m_capsule.pos1.VGet(), capsule1.m_capsule.pos2.VGet(), capsule1.m_capsule.radius,
		capsule2.m_capsule.pos2.VGet(), capsule2.m_capsule.pos2.VGet(), capsule2.m_capsule.radius);
}

//---------------------------------------------------------------------------------
//	円同士の当たり判定
//---------------------------------------------------------------------------------
bool CheckCircleHit(Vector3 pos1, int radius1, Vector3 pos2, int radius2)
{
	// 互いの円の半径同士を足した距離を出す
	int  radius_distance = radius1 + radius2;
	float pos_x = (pos1.x) - (pos2.x);
	// マイナスになるとどこでも当たっている判定になるのでプラスにする
	if (pos_x < 0)
	{
		pos_x * -1;
	}
	
	float pos_z = pos1.z - pos2.z;
	// マイナスになるとどこでも当たっている判定になるのでプラスにする
	if (pos_z < 0)
	{
		pos_z * -1;
	}
	// 互いの座標の距離が上で求めた距離より短くなったら円に入ったことになる
	// X座標の場合を調べる
	if (pos_x <= radius_distance)
	{
		// Z座標の場合を調べる
		if (pos_z <= radius_distance)
		{
			// 条件を満たしたので当たっている
			return true;
		}
	}
	// 条件に満たなかったので当たっていない
	return false;
}
