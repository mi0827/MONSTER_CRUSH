#include "src/WinMain.h"

#include "src/Model/Model.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"
#include "src/Hit/Hit.h"
#include "src/Animation/Animation.h"
#include "src/Action/Attack.h"


bool HitAttack(CapsuleCollision capsule1, CapsuleCollision capsule2)
{
	// 指定のカプセル同士の当たり判定があったとき
	if (CheckCapsuleHit(capsule1, capsule2))
	{
		// 当たり判定があってなおかつ初めてこのif分に入ったときだけ
		// trueを返してダメージ計算をさせる
		return true;
	}
	return false;
}