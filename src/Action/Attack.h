#pragma once

//! @fule Attack.h
//! @brief 攻撃アクションの当たり判定についてのクラス

class Attack
{
public:
	//! 当たり判定があったら一度だけ判定をとる
	bool HitAttack(CapsuleCollision capsule1, CapsuleCollision capsule2, Animation anim);

	//! 当たり判定があったかを調べるもの
	bool hit = true;

};
