#include "DamageAction.h"

//-----------------------------------------------
// ダメージ計算用関数
//-----------------------------------------------
//! @brief ダメージ計算用関数
//! @param ダメージ量
//! @param HP
void DamageCount(int damage_value, int defense_value, int* hp_value)
{
	// ダメージ計算(アルテリオス計算式を使用)
	// どれだけのダメージを食らったかの計算
	int damage = (int)(damage_value / 2 - defense_value / 4);
	// ダメージ分HPを減らす
	*hp_value -= damage;
}
