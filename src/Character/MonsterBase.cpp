#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"

#include "src/System/TargetMove.h"
#include "src/Action/Combo.h"
#include "src/Character/MonsterBase.h"


// モンスター専用のキャラベースクラス

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
MonsterBase::MonsterBase()
{

}

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
MonsterBase::~MonsterBase()
{

}

//---------------------------------------------------------------------------
// ローリングアクションの中の移動処理
//---------------------------------------------------------------------------
void MonsterBase::Action_Rolling(const int rolling_speed)
{
	// ローリング中の移動処理
// 向いている方向に PLAYER_ROLLING_SPEED 分移動する
	m_transform.pos.z += rolling_speed * cosf(TO_RADIAN(m_transform.rot.y));
	m_transform.pos.x += rolling_speed * sinf(TO_RADIAN(m_transform.rot.y));
}

//---------------------------------------------------------------------------
// ベースクラスの初期処理
//---------------------------------------------------------------------------
void MonsterBase::BaseInit(Transform* target_pos, const float m_target_hit_r, int hp_num)
{
	// 移動の際のターゲットのの設定
	move.SetTargetInfo(target_pos, m_target_hit_r);
	// 自身の情報を設定
	move.SetInfo(&m_transform, m_hit_r, M_MOV_SPEED, M_ROT_SPEED);

	m_hp_value = hp_num;
}

//---------------------------------------------------------------------------
// ベースクラスの更新処理
//---------------------------------------------------------------------------
void MonsterBase::BaseUpdate(bool* run_flag)
{
	// 移動処理
	move.Update(run_flag);
}

//---------------------------------------------------------------------------
// 攻撃時の当たり判定を設定する用の関数
//---------------------------------------------------------------------------
void MonsterBase::NEW_Set_Attack_Hit_Damage(int attack_anim_max)
{

	// 攻撃アニメーションの数確保する
	for (int i = 0; i < attack_anim_max; i++)
	{
		Attack_Hit_Damage* attack_hit_damage = new Attack_Hit_Damage;
		m_attack_hit_damage.push_back(attack_hit_damage);
		// 初期値は適当なものを入れておく
		m_attack_hit_damage[i]->attack_hit = m_body;
		m_attack_hit_damage[i]->attack_damage = 0;
	}
}

//---------------------------------------------------------------------------
// 移動用のあたり判定
//---------------------------------------------------------------------------
//void MonsterBase::MoveHitUpdate(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
//{
//	move.Move_Hit(player_pos, before_pos, hit_size, box);
//}
