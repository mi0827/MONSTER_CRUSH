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
void MonsterBase::BaseInit(int hp_num,float up_speed, float down_speed)
{
	// HP設定
	m_hp_value = hp_num;
	// ジャンプ関連の設定
	m_up_speed = up_speed;
	m_down_speed = down_speed;
}


//---------------------------------------------------------------------------
// モンスターの移動に関するターゲットの設定
//---------------------------------------------------------------------------
void MonsterBase::BaseSetTarget(Transform* target_pos, const float m_target_hit_r)
{
	// 移動の際のターゲットのの設定
	move.SetTargetInfo(target_pos, m_target_hit_r);
	// 自身の情報を設定
	move.SetInfo(&m_transform, m_hit_r, M_MOV_SPEED, M_ROT_SPEED);

	
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
// 当たり判定をとってほしいタイミングを保存するための関数
//---------------------------------------------------------------------------
void MonsterBase::SetHitTime(int attack_frame_start, int attack_frame_end, int attack_num)
{
	// 当たり判定をとってほしい最初のフレームの保存
	m_attack_hit_damage[attack_num]->start_time = attack_frame_start;
	// 終わってほしいフレームの保存
	m_attack_hit_damage[attack_num]->end_time = attack_frame_end;
}


//---------------------------------------------------------------------------
//  当たり判定を行っていいタイミングを返するための関数
//---------------------------------------------------------------------------
bool MonsterBase::AttackHitGoodTiming(int attack_num)
{
	// スタートのフレーム
	int start_time = m_attack_hit_damage[attack_num]->start_time;
	// 終わってほしいフレーム
	int end_time = m_attack_hit_damage[attack_num]->end_time;
	// アニメーションの現在のフレーム
	int play_anim_time = m_animation.m_contexts[0].play_time;
	if (play_anim_time >= start_time && end_time <= play_anim_time)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//---------------------------------------------------------------------------
// 当たり判定をとってほしいタイミングを保存するための関数
//---------------------------------------------------------------------------
void MonsterBase::SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num)
{
	// 本来は二つまとめて保存できたができなくなっているためひとつづつしている
	// 当たり判定の保存
	m_attack_hit_damage[attack_num]->attack_hit = attack_hit;

	// ダメージの保存
	m_attack_hit_damage[attack_num]->attack_damage = attack_damage;
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
