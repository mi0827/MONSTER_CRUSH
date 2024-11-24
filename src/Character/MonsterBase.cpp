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
void MonsterBase::ActionRolling(const int rolling_speed)
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
	if (start_time <= play_anim_time   && play_anim_time <= end_time)
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
// 攻撃アニメーションに関する情報の設定
//---------------------------------------------------------------------------
void MonsterBase::SetAttackAnimInfo(int attack_anim_start, int attack_anim_max, int except_attack)
{
	// 攻撃アニメーションのスタートの保存
	m_ATTACK_ANIM_START = attack_anim_start;
	// 攻撃アニメーションの最大数の保存
	m_ATTACK_ANIM_MAX = attack_anim_max;
	// ランダムで攻撃を選ぶ際にはぶいてほしい攻撃番号の保存
	m_ATTACK_ANIM_EXCEPT = except_attack;
}

//---------------------------------------------------------------------------
// 移動処理関数
//---------------------------------------------------------------------------
void MonsterBase::MoveActionUpdate(int ran_anim)
{
	// 毎回リセット
	m_run_flag = false;

	// 移動前の座標一旦保存しておく
	m_before_pos = m_transform.pos;

	// ベースクラスの更新処理
	// 移動の処理が中に入っている
	BaseUpdate(&m_run_flag);

	// run_flag が上がってるときかつ
	// プレイヤーモードがRUN以外の時
	if (m_run_flag && m_monster_mode != RUN)
	{
		// アニメーションの切り替えフラグを上げる
		m_animation.m_anim_change_flag = true;
	}

	// アニメーション変更が可能な時に
	if (m_animation.ChangeFlag(m_run_flag)) {
		// 走りアニメーションに変更
		m_animation.ChangeAnimation(&m_model, ran_anim, true);
		// アニメーションが変わったから
		// プレイヤーモードの切り替えをする
		m_monster_mode = RUN;
	}
}


//---------------------------------------------------------------------------
// 最初の攻撃を行う用関数
//---------------------------------------------------------------------------
void MonsterBase::FirstAttackAction()
{
	// attack_flag が上がってるときかつ
	// プレイヤーモードがATTACK以外の時
	if (m_attack_flag && m_monster_mode != ATTACK)
	{
		// アニメーションの切り替えフラグを上げる
		m_animation.m_anim_change_flag = true;
	}
	// 攻撃モードにしておく
	m_monster_mode = ATTACK;
	// 最初の攻撃もランダムに設定する
	// 攻撃アニメーション以外を排除しておく
	int attack = GetRand(m_ATTACK_ANIM_EXCEPT) + m_ATTACK_ANIM_START;
	m_animation.ChangeAnimation(&m_model, attack, false);
	// 攻撃アニメーション番号の保存
	m_now_attack_anim = attack;

	// 現在の攻撃番号を保存する
	m_now_attack = m_now_attack_anim - m_ATTACK_ANIM_START;

	m_stop_combo_flag = true;
}

//---------------------------------------------------------------------------
// 攻撃関連の更新処理
//---------------------------------------------------------------------------
void MonsterBase::AttackActionUpdate()
{
	// コンボをしていいフラグがったている時だけ
	if (m_stop_combo_flag)
	{
		// コンボ関数を呼ぶ
		ComboUpdate();
	}
}

//---------------------------------------------------------------------------
// 攻撃関連の更新処理
//---------------------------------------------------------------------------
void MonsterBase::JumpAction(int jump_anim, int target_distance)
{
	// ターゲットとの距離
	float distance = move.Get_Target_Distance();
	// ターゲットとの距離が一定以上になったら
	if (target_distance <= distance)
	{
		// ジャンプ攻撃をしてほしいのでランフラグを下す
		m_run_flag = false;

		// attack_flag が上がってるときかつ
	   // プレイヤーモードがATTACK以外の時
		if (m_attack_flag && m_monster_mode != ATTACK)
		{
			// アニメーションの切り替えフラグを上げる
			m_animation.m_anim_change_flag = true;
		}
		// 攻撃モードにしておく
		m_monster_mode = ATTACK;

		m_animation.ChangeAnimation(&m_model, jump_anim, false);
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = jump_anim;
		// 現在の攻撃番号を保存する
		m_now_attack = m_now_attack_anim - m_ATTACK_ANIM_START;

		m_stop_combo_flag = true;
		// ジャンプ処理は
		//jump_num = STANDBY;
	}
	else
	{
		// ジャンプフラグを下げる
		m_jump_flag = false;
	}
}

//---------------------------------------------------------------------------
// ジャンプ攻撃の更新処理
//---------------------------------------------------------------------------
void MonsterBase::JumpActionUpdate(float down_speed)
{
	// モンスターのアニメーションがジャンプしそうに瞬間から着地アニメーションが始まるまでの間
	if (m_animation.m_contexts[0].play_time >= 80.0f && m_animation.m_contexts[0].play_time < 110.0f)
	{
		// ジャンプしてから下に下がるスピードをゼロにする
		m_down_speed = 0.0f;
		m_jump_flag = true;
	}

	if (m_animation.m_contexts[0].play_time >= 110.0f)
	{
		// 降下スピードをリセット
		m_down_speed = down_speed;

		// フラグが立っている時かつ地面につくアニメーションの時
		if (m_jump_flag && m_animation.m_contexts[0].play_time >= 140.0f)
		{
			// 移動先の座標の設定ターゲットの座標からモンスターのbodyの半径分
			m_transform.pos.x = move.m_target_info.m_target->pos.x - m_body.m_capsule.radius;
			m_transform.pos.z = move.m_target_info.m_target->pos.z - m_body.m_capsule.radius;
			// ジャンプフラグを下げる
			//m_jump_flag = false; // 落ちる処理へ
		}
	}


}

void MonsterBase::SetJumpPos(Vector3 pos)
{
	m_jump_pos = pos;
}


//---------------------------------------------------------------------------
// コンボ攻撃更新処理
//---------------------------------------------------------------------------
void MonsterBase::ComboUpdate()
{
	// コンボを行っていい状態なのはかを保存する変数
	bool combo_jug;
	// TargetMoveがターゲットと接しているそうでないかで変わる
	// 接していず移動可能状態になれば
	if (move.m_hit)
	{
		// コンボをできる状態でない
		combo_jug = true;
	}
	// 接していて止まっている場合
	if (!move.m_hit)
	{
		// コンボできる状態
		combo_jug = true;
	}

	// コンボ可能か判断用関数
	m_combo.ComboJudgmentCondition
	(
		&m_combo_flag,
		combo_jug,
		m_animation.m_contexts[0].play_time,
		m_animation.m_contexts[0].animation_total_time
	);

	// コンボフラグが上がっているとき
	if (m_combo_flag)
	{
		// 次の攻撃アニメーションをランダムでセット
		m_next_anim = SetRandAttack();

		// コンボ用のアニメーションをつける
		m_animation.ActionComboChangeAnimation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// 現在の攻撃アニメーションを保存
			m_now_attack_anim = m_next_anim;
			// 現在の攻撃番号を保存する
			m_now_attack = m_now_attack_anim - m_ATTACK_ANIM_START;
		}
	}
}


//---------------------------------------------------------------------------
// 行いたい攻撃アニメーションをランダムで選ぶための関数
//---------------------------------------------------------------------------
int MonsterBase::SetRandAttack()
{
	// 次に行ってほしいアニメーションを入れる変数
	int next_anim = 0;

	// アニメーションが決まる名で無限ループ
	while (true)
	{
		// 次のアニメーションをランダムで入れる
		// 攻撃アニメーションスタートから攻撃アニメーションの最大までで
		next_anim = GetRand(m_ATTACK_ANIM_MAX) + m_ATTACK_ANIM_START;
		// 次に行いたいアニメーションと今のアニメーションがかぶったら
		if (next_anim == m_now_attack_anim || next_anim == 0)
		{
			// またランダムで入れなおす
			next_anim = GetRand(m_ATTACK_ANIM_MAX) + m_ATTACK_ANIM_START;
		}
		break;
	}

	// 次に行ってほしい攻撃アニメーションを返す
	// 今はプレイヤーのカウンターがうまくいっているかを見るために固定にしている
	return next_anim;
}
