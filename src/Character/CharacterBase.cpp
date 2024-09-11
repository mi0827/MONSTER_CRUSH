#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

#include "src/Action/Combo.h"

#include "src/Model/Model.h"
#include "src/System/Move.h"

#include "src/System/UIBar.h"

#include "CharacterBase.h"

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
CharacterBase::CharacterBase()
{

}

//---------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------
CharacterBase::~CharacterBase()
{
	m_attack_hit_damage.clear();
}

//---------------------------------------------------------------------------
// 移動用のあたり判定
//---------------------------------------------------------------------------
void CharacterBase::MoveHitUpdate(BoxCollision* box)
{
	// 当たり判定の
	m_move_hit_box.CreateBox(m_transform.pos, m_move_hit_size);
	m_move.Move_Hit(&m_transform.pos, &m_before_pos, &m_move_hit_size, box);
}

//---------------------------------------------------------------------------
// 当たり判定をとってほしいタイミングを保存するための関数
//---------------------------------------------------------------------------
void CharacterBase::SetHitTime(int attack_frame_start, int attack_frame_end, int attack_num)
{
	// 当たり判定をとってほしい最初のフレームの保存
	m_attack_hit_damage[attack_num]->start_time = attack_frame_start;
	// 終わってほしいフレームの保存
	m_attack_hit_damage[attack_num]->end_time = attack_frame_end;
}

bool CharacterBase::AttackHitGoodTiming(int attack_num)
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
// 当たり判定とダメージを保存する関数
//---------------------------------------------------------------------------
void CharacterBase::SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num)
{
	// 本来は二つまとめて保存できたができなくなっているためひとつづつしている
	// 当たり判定の保存
	m_attack_hit_damage[attack_num]->attack_hit = attack_hit;

	// ダメージの保存
	m_attack_hit_damage[attack_num]->attack_damage = attack_damage;
}

//---------------------------------------------------------------------------
// ベースクラスでの初期処理
//---------------------------------------------------------------------------
void CharacterBase::BaseInit(float r, int HP)
{
	// 座標、向き、回転の初期化
	m_transform.pos.clear();
	// 当たり判定の半径の設定
	m_hit_r = r;
	// HPの設定
	m_hp_value = HP;
}

//---------------------------------------------------------------------------
// 攻撃時の当たり判定を設定する用の関数
//---------------------------------------------------------------------------
void CharacterBase::NEW_Set_Attack_Hit_Damage(int attack_anim_max)
{
	// 攻撃アニメーションの数確保する
	for (int i = 0; i < attack_anim_max; i++)
	{
		Attack_Hit_Damage* attack_hit_damage = new Attack_Hit_Damage;
		m_attack_hit_damage.push_back(attack_hit_damage);
		// 初期値は適当なものを入れておく
		m_attack_hit_damage[i]->attack_hit =  m_body;
		m_attack_hit_damage[i]->attack_damage = 0;
	}
}

//---------------------------------------------------------------------------
//  ベースクラスでの更新処理
//---------------------------------------------------------------------------
void CharacterBase::BaseUpdate(bool* m_check_move, Vector3* camera_rot, const float* mov_speed)
{
	// 移動の処理
	m_move.Update(m_check_move, camera_rot, &m_transform.rot, mov_speed, &m_transform.pos, m_pad_no, m_mov);
}
