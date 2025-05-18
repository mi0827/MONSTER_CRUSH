#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/System/Move.h"
#include "src/Action/Combo.h"
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
// プレイヤーの移動用関数
//---------------------------------------------------------------------------
void CharacterBase::MoveAction(int run_anim_no, const float move_speed, Vector3* camera_rot)
{
	// 毎回リセット
	m_run_flag = false;

	//// 移動前の座標一旦保存しておく
	//m_before_pos = m_transform.pos;

	// ベースクラスの更新処理
	// 移動の処理が中に入っている
	BaseUpdate(&m_run_flag, camera_rot, &move_speed);

	// run_flag が上がってるときかつ
	// プレイヤーモードがRUN以外の時
	if (m_run_flag && m_player_mode != RUN)
	{
		// アニメーションの切り替えフラグを上げる
		m_animation.m_anim_change_flag = true;
	}

	// アニメーション変更が可能な時に
	if (m_animation.ChangeFlag(m_run_flag))
	{
		// 走りアニメーションに変更
		m_animation.ChangeAnimation(&m_model, run_anim_no, true);
		// アニメーションが変わったから
		// プレイヤーモードの切り替えをする
		m_player_mode = RUN;
	}
	
}

//---------------------------------------------------------------------------
// 移動用のあたり判定
//---------------------------------------------------------------------------
void CharacterBase::MoveHitUpdate(BoxCollision* box)
{
	// 当たり判定の
	m_move_hit_box.CreateBox(m_transform.pos, m_move_hit_size);
	m_move.MoveHit(&m_transform.pos, &m_before_pos, &m_move_hit_size, box);
}


//---------------------------------------------------------------------------
// キャラクターの座標設定
//---------------------------------------------------------------------------
void CharacterBase::SetCharacterPos(Vector3 pos)
{
	m_transform.pos.set(pos);
}


//---------------------------------------------------------------------------
// キャラクターの向きの設定
//---------------------------------------------------------------------------
void CharacterBase::SetCharacterRot(Vector3 rot)
{
	m_transform.rot.set(rot);
}

//---------------------------------------------------------------------------
//  攻撃に関する情報をセットするための関数
//---------------------------------------------------------------------------
void CharacterBase::SetAttackInfo(const int attack_start_anim, int attack_anim_1, int attack_anim_2, const int combo_max)
{
	// 攻撃アニメーションが始まる番号を設定するための関数
	m_ATTAK_START_ANIM_NO = attack_start_anim;
	// 攻撃アニメーションの番号の保存
	m_first_attack_anim_1 = attack_anim_1;
	m_first_attack_anim_2 = attack_anim_2;
	// 最大コンボ数の保存
	m_COMBO_MAX = combo_max;

}

//---------------------------------------------------------------------------
// 最初の攻撃を行うための関数
//---------------------------------------------------------------------------
void CharacterBase::AttackFirst()

{
	// 指定のマウスボタンが押されたら
	if (PushMouseInput(MOUSE_INPUT_LEFT) || IsPadOn(PAD_ID::PAD_B))
	{
		// attack_flag が上がってるときかつ
		// プレイヤーモードがATTACK以外の時
		if (m_attack_flag && m_player_mode != ATTACK)
		{
			// アニメーションの切り替えフラグを上げる
			m_animation.m_anim_change_flag = true;

		}
		// 攻撃モードにしておく
		m_player_mode = ATTACK;
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = m_first_attack_anim_1;
		m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);


		// 現在の攻撃番号を保存する
		m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;

		// コンボの回数をリセット
		m_combo_count = 0;

		// コンボをしていいようにする
		m_combo.ComboStartJudgent();
		// m_stop_combo_flag = false;
	}
	// 指定のマウスボタンが押されたら
	if (PushMouseInput(MOUSE_INPUT_RIGHT) || IsPadOn(PAD_ID::PAD_Y))
	{
		// attack_flag が上がってるときかつ
		// プレイヤーモードがATTACK以外の時
		if (m_attack_flag && m_player_mode != ATTACK)
		{
			// アニメーションの切り替えフラグを上げる
			m_animation.m_anim_change_flag = true;


		}
		// 攻撃モードにしておく
		m_player_mode = ATTACK;
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = m_first_attack_anim_2;

		m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);


		// 現在の攻撃番号を保存する
		m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;

		// コンボの回数をリセット
		m_combo_count = 0;

		// コンボをしていいようにする
		m_combo.ComboStartJudgent();
		
		//m_stop_combo_flag = false;

	}
}

//---------------------------------------------------------------------------
// 最初の攻撃を行うための関数
//---------------------------------------------------------------------------
void CharacterBase::AttackUpdate()
{
	// コンボ関数を呼ぶ
	ComboActionUpdate();
}

//---------------------------------------------------------------------------
// 当たり判定をとってほしいタイミングを保存するための関数
//---------------------------------------------------------------------------
void CharacterBase::SetHitTiming(float attack_frame_start, float attack_frame_end, bool can_hitstop, int attack_num )
{
	// 当たり判定をとってほしい最初のフレームの保存
	m_attack_hit_damage[attack_num]->start_time = (int)attack_frame_start;
	// 終わってほしいフレームの保存
	m_attack_hit_damage[attack_num]->end_time = (int)attack_frame_end;
	// ヒットストップを行っていいかどうかの保存
	m_attack_hit_damage[attack_num]->can_hit_stop = can_hitstop;
}


//---------------------------------------------------------------------------
// 当たり判定をタイミングを返するための関数
//---------------------------------------------------------------------------
bool CharacterBase::AttackHitGoodTiming(int attack_num)
{
	// スタートのフレーム
	int start_time = m_attack_hit_damage[attack_num]->start_time;
	// 終わってほしいフレーム
	int end_time = m_attack_hit_damage[attack_num]->end_time;
	// アニメーションの現在のフレーム
	float play_anim_time = m_animation.m_contexts[0].play_time;
	// starttime以上　かつ　endtime以下の時
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
// ローリングアクションを開始されるための関数
//---------------------------------------------------------------------------
void CharacterBase::RollingActionStart()
{
	// 指定のキーが押された時
	if (PushHitKey(KEY_INPUT_SPACE)|| IsPadOn(PAD_ID::PAD_A))
	{
		// アクションモードをローリングにする
		m_player_mode = ROLLING;
	}
}

//---------------------------------------------------------------------------
// ローリングアクションの更新処理
//---------------------------------------------------------------------------
void CharacterBase::RollingActionUpdate(int rolling_anim_no, const int rolling_speed)
{
	// ローリングアニメーションのセット
	// ローリングフラグが上がっていないとき
	if (!m_rolling_flag)
	{
		// ローリングアニメーションをセットする
		m_animation.ChangeAnimation(&m_model, rolling_anim_no, false);
		// ローリングフラグをあげる
		m_rolling_flag = true;
	}

	// ローリング中に攻撃ボタンを押したらカウンター攻撃に派生する
	if (PushMouseInput(MOUSE_INPUT_LEFT) || PushMouseInput(MOUSE_INPUT_RIGHT)|| IsPadOn(PAD_ID::PAD_B)|| IsPadOn(PAD_ID::PAD_Y))
	{
		m_counter_flag = true;
	}

	// ローリング中の移動処理
	// 向いている方向に PLAYER_ROLLING_SPEED 分移動する
	m_transform.pos.z += rolling_speed * cosf(TO_RADIAN(m_transform.rot.y));
	m_transform.pos.x += rolling_speed * sinf(TO_RADIAN(m_transform.rot.y));

	// ローリングアニメーションが終わったら(終わりだとうまく入らなかったから終わる少し前にした)
	// またはダメージを食らったフラグが上がったいたら
	if (m_animation.m_contexts[0].play_time >= m_animation.m_contexts[0].animation_total_time - 10)
	{
		// アニメーションのチェンジフラグを上げる
		m_animation.m_anim_change_flag = true;
		// ローリングフラグを下げる
		m_rolling_flag = false;
		if (m_counter_flag)
		{
			m_player_mode = COUNTER;
		}
		else
		{
			// 一旦アクションモードをIDLEにしておく
			m_player_mode = IDLE;
		
		}
	}
}


//---------------------------------------------------------------------------
// カウンターアクション
//---------------------------------------------------------------------------
void CharacterBase::CounterAction(int counter_anim_no)
{
	// 攻撃モードにしておく
	m_player_mode = ATTACK;
	// 攻撃アニメーション番号の保存
	m_now_attack_anim = counter_anim_no;
	m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);

	// 現在の攻撃番号を保存する
	m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;
}

//---------------------------------------------------------------------------
// コンボアクションの更新処理
//---------------------------------------------------------------------------
void CharacterBase::ComboActionUpdate()
{
	// コンボ可能か判断用関数
// 指定のキー操作がされた場合
	switch (m_combo.m_combo_mode)
	{
	
	case m_combo.COMBO_START: // コンボスタート
		m_combo.ComboJudgmentKey
		(
			&m_combo_flag,
			&m_mouse_flag,
			MOUSE_INPUT_LEFT,
			PAD_ID::PAD_B,
			m_animation.m_contexts[0].play_time,
			m_animation.m_contexts[0].animation_total_time,
			&m_combo_count
		);
		m_combo.ComboJudgmentKey
		(
			&m_combo_flag,
			&m_mouse_flag,
			MOUSE_INPUT_RIGHT,
			PAD_ID::PAD_Y,
			m_animation.m_contexts[0].play_time,
			m_animation.m_contexts[0].animation_total_time,
			&m_combo_count
		);
		break;
		
	case m_combo.COMBO_STANDBY: // コンボスタンバイ

		// 今のアニメーション番号から一つ次のアニメーション
		if (m_mouse_flag == MOUSE_INPUT_LEFT)
		{
			m_next_anim = m_first_attack_anim_1 + m_combo_count;
			aaa = m_next_anim;
			m_combo.ChangeComboMode(m_combo.COMBP_IN);
			break;
		}
		if (m_mouse_flag == MOUSE_INPUT_RIGHT)
		{
			m_next_anim = m_first_attack_anim_2 + m_combo_count;
			bbb = m_next_anim;
			m_combo.ChangeComboMode(m_combo.COMBP_IN);
			break;
		}
		
	case m_combo.COMBP_IN: // コンボ中
		// コンボがアニメーションの最大と同じになったら(指定された)
		if (m_combo_count >= m_COMBO_MAX)
		{
			// コンボの回数をリセット
			m_combo_count = 0;

			// 当たり判定の設定がバックっているので一下げる
			m_next_anim--;

			m_combo.ChangeComboMode(m_combo.COMBO_FINISH);
			break;

		}
		// コンボ用のアニメーションをつける
		m_animation.ActionComboChangeAnimation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// 現在の攻撃アニメーションを保存
			m_now_attack_anim = m_next_anim;
			// 現在の攻撃番号を保存する
			m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;

			m_combo.ChangeComboMode(m_combo.COMBO_START);
		}
		break;
	case m_combo.COMBO_FINISH: // コンボ終了

		// コンボフラグを下げる
		m_combo_flag = false;

		break;

	}
}

//---------------------------------------------------------------------------
// 攻撃を受けたかをチェックするための関数
//---------------------------------------------------------------------------
void CharacterBase::CheckHitDamage()
{
	// ダメージを食らったフラグが立っていたら
	if (m_damage_flag)
	{
		// プレイヤーの状態をダメージを受けた状態にする
		m_player_mode = HIT_DAMAGE;
		// アニメーションの切り替えフラグを上げる
		m_animation.m_anim_change_flag = true;
	}
}

//---------------------------------------------------------------------------
// 攻撃を受けて時のアニメーション処理
//---------------------------------------------------------------------------
void CharacterBase::HitDamageUpdate(int hit_damage_anim)
{
	// 攻撃を受けた時かつ攻撃を受けた時のアニメーションが付いてないとき(フラグが下がってるとき)
	if (m_damage_flag && m_damage_anim_flag == false)
	{
		// 攻撃を受けた時のアニメーションが付いたのでフラグを上げる
		m_damage_anim_flag = true;
		// ダメージアニメーションをつける
		if (m_animation.ChangeFlag(m_damage_anim_flag))
		{
			// アニメーションを停止に変更する
			m_animation.ChangeAnimation(&m_model, hit_damage_anim, true);

		}
	}
	// 攻撃を受けたらゲームパッドを揺らす
	if (GetJoypadNum() >= 1)
	{
		PadVidation(DX_INPUT_PAD1, 1000, 3, -1);
	}
	// ダメージを食らったアニメーションが終わりにかかったら
	if (m_animation.m_contexts[0].play_time >= m_animation.m_contexts[0].animation_total_time - 10)
	{
		// 一旦ここでダメージ受けたフラグを下げておく
		m_damage_flag = false;
		// 攻撃を受けた時のアニメーションが終わるのでフラグを下げる
		m_damage_anim_flag = false;
		// プレイヤーのモードをIDLE状態にする
		m_player_mode = IDLE;
		// アイドル状態にしたいのでアイドルフラグを立てる
		m_idle_flag = true;
		// アニメーション変更フラグも立てておく
		m_animation.m_anim_change_flag = true;
	}
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
void CharacterBase::NEWSetAttackHitDamage(int attack_anim_max)
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
//  ベースクラスでの更新処理
//---------------------------------------------------------------------------
void CharacterBase::BaseUpdate(bool* m_check_move, Vector3* camera_rot, const float* mov_speed)
{
	// 移動の処理
	m_move.Update(m_check_move, camera_rot, &m_transform.rot, mov_speed, &m_transform.pos, m_pad_no, m_mov);
	// モデルのY軸回転に制限をかけている
	//m_model.RotationYLimits();
}


