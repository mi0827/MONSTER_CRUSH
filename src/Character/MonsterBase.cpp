#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"

#include "src/System/TargetMove.h"
#include "src/Action/Combo.h"
#include "src/System/UIBar.h"
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
	m_attack_hit_damage.clear();
	m_combo_pattern.clear();
}

//---------------------------------------------------------------------------
// アニメーションの指定のフレームで移動させる処理
//---------------------------------------------------------------------------
void MonsterBase::MoveBetween(float mov_speed, float mov_strat_frame, float mov_end_frame)
{
	if (m_animation.m_contexts[0].play_time >= mov_strat_frame
		&& m_animation.m_contexts[0].play_time <= mov_end_frame)
	{
		m_transform.pos.x += mov_speed * sinf(TO_RADIAN(m_transform.rot.y));
		m_transform.pos.z += mov_speed * cosf(TO_RADIAN(m_transform.rot.y));
	}
}

//---------------------------------------------------------------------------
// ローリングアクションをセット
//---------------------------------------------------------------------------
void MonsterBase::RollingAction(int rolling_anim)
{
	// アニメーション番号を保存
	m_now_attack_anim = rolling_anim;
	// 攻撃番号も保存
	m_now_attack = m_now_attack_anim + m_ATTACK_ANIM_START;
	// アニメーションの変更
	m_animation.ChangeAnimation(&m_model,m_now_attack,false);

}

//---------------------------------------------------------------------------
// ローリングアクションの中の移動処理
//---------------------------------------------------------------------------
void MonsterBase::ActionRolling(const int rolling_speed, float rolling_start_frame, float rolling_end_frame)
{
	// ローリング中の移動処理
	// 向いている方向に PLAYER_ROLLING_SPEED 分移動する
	MoveBetween(rolling_speed, rolling_start_frame, rolling_end_frame);
}

//---------------------------------------------------------------------------
// スタンした時用の更新処理
//---------------------------------------------------------------------------
void MonsterBase::StunActionUpdate(int down_anim_num, int up_anim_num, int sutn_value_max)
{

	switch (m_stun_info_num)
	{
	case DOWN: // スタンして倒れたはじめ
		// モンスターの立っているフラグをすべて下げる
		// m_attack_flag = false;
		// m_run_flag = false;
		// ダウン中のアニメーションをつける
		if (m_animation.ChangeFlag(m_stun_flag))
		{
			m_animation.ChangeAnimation(&m_model, down_anim_num, false);
			// アニメーション切り替え用のフラグを切り替える
			m_animation.m_anim_change_flag = false;
			// 状態を次に変更
			m_stun_info_num = UPSTANDBY;
		}
		break;
	case UPSTANDBY: // スタン中
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// アニメーション切り替え用のフラグを切り替える
			m_animation.m_anim_change_flag = true;
			// 起き上がるアニメーションに変更
			m_animation.ChangeAnimation(&m_model, up_anim_num, false);
			// スタンアニメーションでずれた座標を治す
			m_transform.pos.z += 22.0f * cosf(TO_RADIAN(m_transform.rot.y));
			m_transform.pos.x += 22.0f * sinf(TO_RADIAN(m_transform.rot.y));
			// 状態を次の変更
			m_stun_info_num = UP;
		}

		break;
	case UP: // 起き上がるとき
		// アニメーションの再生速度に合わせてスタン値を回復させる
		static float stun_recovery_value = sutn_value_max / m_animation.m_contexts[0].animation_total_time;
		m_stun_value = stun_recovery_value * m_animation.m_contexts[0].play_time;

		if (m_animation.m_contexts[0].is_playing == false)
		{
			// スタン状態の解除
			m_stun_flag = false;
			// アイドル状態にするためにアイドルフラグを立てる
			m_idle_flag = true;
			// モンスターの状態をアイドルに戻す
			m_monster_mode = IDLE;
			// スタン状態をリセット
			m_stun_info_num = DOWN;
			// アニメーション切り替え用のフラグを切り替える
			m_animation.m_anim_change_flag = true;
			// ほんとは徐々に増やしていきたかったけどそれがうまくいかない
			m_stun_value = 150;
		}
		break;
	}

}


//---------------------------------------------------------------------------
// ベースクラスの初期処理
//---------------------------------------------------------------------------
void MonsterBase::BaseInit(int hp_num)
{
	// HP設定
	m_hp_value = hp_num;
	// ジャンプ関連の設定
	/*m_up_speed = up_speed;
	m_down_speed = down_speed;*/
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
void MonsterBase::MoveUpdate(bool* run_flag)
{
	// 前のフレームで攻撃フラグが立っていてそのフラグが今回下がったら
	// プレイヤーが攻撃範囲外に出たということなので
	// モンスターはプレイヤーを見失ったことになる
	// ランダムで一定のフレームの間きょろきょろしてから動き始める
	// 移動をはじめて一定のフレームで攻撃フラグが立たなかったら距離を詰める攻撃をする

	// 1フレーム前の攻撃フラグの状態を保存しておく
	//m_past_attack_flag = m_attack_flag;


	// 移動処理
	move.Update(run_flag);
	// ランフラグが下りたら
	if (!move.m_hit)
	{
		// モンスターの状態を攻撃に変更
		m_monster_mode = ATTACK;
	}
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
	if (start_time <= play_anim_time && play_anim_time <= end_time)
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
// アイドルアクションの更新処理
//---------------------------------------------------------------------------
void MonsterBase::IdleActionUpdate(int idle_anim_num)
{
	//Player_Mode(IDLE);
			// アニメーション変更が可能な時に
	if (m_animation.ChangeFlag(m_idle_flag))
	{
		// アニメーションを停止に変更する
		m_animation.ChangeAnimation(&m_model, idle_anim_num, true);
		m_animation.m_anim_change_flag = false;
	}

	// 一定時間たったらプレイヤーを追いかけ始める
	// 止まってほしいフレームが設定されていない場合
	if (!m_set_stop_frame_flag)
	{
		// 止まっていてほしいフレーム数を設定
		m_stop_frame = GetRand(20) + 60;
		// 設定が終わったらからフラグをあげる
		m_set_stop_frame_flag = true;
	}
	else // 止まってほしいフレームを設定し終わっていたら
	{
		// 止まってほしいフレームを減らしていく
		m_stop_frame--;
		// フレームが０以下になったら
		if (m_stop_frame <= 0)
		{
			// セットした分がなくなったのでフラグを下げる
			m_set_stop_frame_flag = false;
			// 止まってほしい時間が終わったのでプレイヤーを追わせる
			m_monster_mode = RUN;
		}
	}



}

//---------------------------------------------------------------------------
// 移動処理関数
//---------------------------------------------------------------------------
void MonsterBase::MoveAction(int ran_anim)
{
	// 毎回リセット
	m_run_flag = false;

	// 移動前の座標一旦保存しておく
	m_before_pos = m_transform.pos;

	// ベースクラスの更新処理
	// 移動の処理が中に入っている
	MoveUpdate(&m_run_flag);

	// run_flag が上がってるときかつ
	// プレイヤーモードがRUN以外の時
	if (m_run_flag && m_animation.m_anim_change_flag == false)
	{
		// 走りアニメーションに変更
		m_animation.ChangeAnimation(&m_model, ran_anim, true);
		// アニメーションの切り替えフラグを上げる
		m_animation.m_anim_change_flag = true;
	}

}

//---------------------------------------------------------------------------
// コンボパターンの数を設定
//---------------------------------------------------------------------------
void MonsterBase::ComboPatternNumberInit(int pattern_max)
{

	// コンボのパターンの最大数を保存
	m_combo_pattern_max = pattern_max;

	// コンボを入れる入れ物をパターン分用意する
	for (int i = 0; i < pattern_max; i++)
	{
		/*ComboPattern* combo = new ComboPattern;
		m_combo_pattern.push_back(combo);*/

	}
	m_combo_pattern.resize(pattern_max);


}


//---------------------------------------------------------------------------
// 各コンボパターンの設定
//---------------------------------------------------------------------------
void MonsterBase::ComboPatternInfoInit(int pattern_num, int combo_num_max, int rear_crevice_frame, int* anim_num)
{
	// 配列の長さからコンボの長さの保存
	m_combo_pattern[pattern_num].m_combo_num_max = combo_num_max;
	// コンボを保存する配列の確保
	m_combo_pattern[pattern_num].m_combo_parts.resize(combo_num_max);
	// コンボが終わった後のあと隙の設定
	m_combo_pattern[pattern_num].m_rear_crevice_frame = rear_crevice_frame;
	// コンボに使用するアニメーション番号を保存する
	for (int i = 0; i < m_combo_pattern[pattern_num].m_combo_num_max; i++)
	{
		// ここで攻撃番号を入れる
		m_combo_pattern[pattern_num].m_combo_parts[i] = anim_num[i];
	}
}

//---------------------------------------------------------------------------
// 最初の攻撃を行う用関数
//---------------------------------------------------------------------------
void MonsterBase::FirstAttackAction()
{
	// 攻撃フラグを上げる
	m_attack_flag = true;
	//// 攻撃モードにしておく
	//m_monster_mode = ATTACK;
	// アニメーションの切り替えフラグを上げる
	m_animation.m_anim_change_flag = true;

	// どの攻撃パターンを使用するかをランダムで各確保する
	int m_combo_pattern_num = GetRand(m_combo_pattern_max - 1);

	// コンボの番号を０にする
	m_combo_num = 0;
	// コンボのに使用する攻撃番号を保存
	m_now_attack = m_combo_pattern[m_combo_pattern_num].m_combo_parts[m_combo_num];
	// 攻撃アニメーション番号の保存
	m_now_attack_anim = m_now_attack + m_ATTACK_ANIM_START;
	// アニメーションの変更
	m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);

	// 現在の攻撃番号を保存する
	//m_now_attack = m_now_attack_anim - m_ATTACK_ANIM_START;

	m_stop_combo_flag = true;
}

//---------------------------------------------------------------------------
// 攻撃関連の更新処理
//---------------------------------------------------------------------------
void MonsterBase::AttackActionComboUpdate()
{
	switch (m_attack_info_num)
	{
	case ATTACKSET:
		// どのコンボを使うかをランダムでセット
		FirstAttackAction();
		// 次の攻撃状態に移動
		m_attack_info_num = UNDERATTACK;
		break;
	case UNDERATTACK:
		// 現在行われるアニメーションがの再生が終わったら
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// コンボ番号を増やす
			m_combo_num++;
			// コンボのに使用する攻撃番号を保存
			if (m_combo_pattern[m_combo_pattern_num].m_combo_parts[m_combo_num] != -1)
			{
				m_now_attack = m_combo_pattern[m_combo_pattern_num].m_combo_parts[m_combo_num];
			}
			else
			{
				// 次の攻撃番号が-1ならコンボが続かないからbreakして
				// モンスターの状態をIdle状態に変更する
				m_monster_mode = IDLE;
				// アニメーション変更フラグを上げる
				m_animation.m_anim_change_flag = true;
				// 最初の攻撃状態に移動
				m_attack_info_num = ATTACKSET;
				break;
			}

			// 攻撃アニメーション番号の保存
			m_now_attack_anim = m_now_attack + m_ATTACK_ANIM_START;
			// アニメーションの変更
			m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);
		}

		break;
	}
}

//---------------------------------------------------------------------------
// 攻撃関連の更新処理
//---------------------------------------------------------------------------
void MonsterBase::JumpAction(int jump_anim, int target_distance)
{
	// プレイヤーとの距離をもらってくる
	// ターゲットとの距離
	float distance = move.GetTargetDistance();
	// 行ってい以上の距離の時
	// ターゲットとの距離が一定以上だったら
	if (distance <= target_distance )
	{
		// 攻撃フラグを立てる
		m_jump_flag = true;
		// 攻撃モードにしておく
		m_monster_mode = ATTACK;
	}
	if (m_jump_flag)
	{
		// 攻撃番号の保存
		m_now_attack = jump_anim - m_ATTACK_ANIM_START;
		// ジャンプアニメーションをつける
		m_animation.ChangeAnimation(&m_model, jump_anim, false);
	}
}

//---------------------------------------------------------------------------
// ジャンプ攻撃の更新処理
//---------------------------------------------------------------------------
void MonsterBase::JumpActionUpdate(float jump_mov_speed,float jump_mov_strat_frame, float jump_mov_end_frame)
{
	// ジャンプ時のモンスターの向きに合わせてジャンプする
	// ここの移動量が決まっていない
	// ジャンプアニメーションの着地に合わせて移動をやめる
	// 飛んでいる間だけ
	MoveBetween(jump_mov_speed, jump_mov_strat_frame, jump_mov_end_frame);


	// ジャンプアニメーションが終わったらRunモードにへんこうする
	if (m_animation.m_contexts[0].is_playing == false)
	{
		// ジャンプが終わったのでフラグを下げる
		m_jump_flag = false;

		// Run状態に変更
		m_monster_mode = RUN;
		// アニメーション変更フラグを立てておく
		m_animation.m_anim_change_flag = true;


	}

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
