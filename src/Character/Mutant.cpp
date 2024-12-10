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

#include "Mutant.h"

namespace
{
	// プレイヤーのとのあたり判定で使うboxのあたり判定のサイズ
	const Vector3 M_MOVE_SIZE{ 15.0f,0,15.0f };
	// 上記で作成したサイズの半分のサイズ
	const Vector3 M_MOVE_SIZE_HALF{ M_MOVE_SIZE / 2 };
}


//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
Mutant::Mutant()
{
	// 最初はアイドル状態にしておく
	m_idle_flag = true;
	// 最初はアイドル状態にしておく
	m_monster_mode = IDLE;

	// 初期座標の設定
	m_transform.pos.set(100.0f, 0.0f, 100.0f);
	// モデルのスケールの設定
	m_transform.scale.set(0.2f, 0.2f, 0.2f);
}


//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
Mutant::~Mutant()
{
	Exit();
}

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void Mutant::Init()
{

	// aaaaa

	// モデル画像の読み込み
	m_model.LoadModel("Data/Model/Mutant/Mutant.mv1");

	// アニメーションの初期設定
	AnimLoadInit();
	// 攻撃アニメーションに関する情報の設定
	SetAttackAnimInfo(ATTACK_ANIM_START, ATTACK_ANIM_MAX, attack_rolling);
	// 攻撃アニメーションの数分のあたり判定用の入れ物を確保する
	NEW_Set_Attack_Hit_Damage(ATTACK_ACTION_MAX);

	// ステータスバーの設定
	StatusBarInit();

	// 攻撃に関することの初期関数
	SetAttackInfo();
	// モンスターのステータスの初期設定
	BaseInit(HP_VALUE_MAX/* JUMP_UP_SPEED, JUMP_DOWN_SPEED*/);
	// アニメーションつけるのフラグを上げておく
	m_animation.m_anim_change_flag = true;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Mutant::Update(Transform* target_pos, float target_r)
{

	// プレイヤーではこれがないとバグるが
	// モンスターではこれがあるとバグる
	// モンスターのほうがフラグ管理に失敗した
	MonsterMode(m_monster_mode);

	//clsDx();
	// HPの値が減ったかどうか
	m_hp_bra.Update(m_hp_value);
	m_stun_bra.Update(m_stun_value);


	switch (m_life_and_death)
	{

	case alive: // 生きいるとき
		// 移動先のターゲットの設定
		BaseSetTarget(target_pos, target_r);
		LiveUpdate(target_pos, target_r);
		// モンスターのHPがより多いい時
		if (m_hp_value > 0)
		{
			// 生きてる
			m_life_and_death = alive;
		}
		else // それ以外の時
		{

			// 死んだ
			m_life_and_death = die_anim;
			// プレイヤーの状態をにDIE変更
			m_monster_mode = DIE;
			// すべてのフラグを下げる
			m_idle_flag = false;
			m_run_flag = false;
			m_attack_flag = false;

			// アニメーションの切り替えフラグを上げる
			m_animation.m_anim_change_flag = true;
			// 待機フラグを毎回リセット
			m_idle_flag = true;
			// 死んだアニメーションをつける
			if (m_animation.ChangeFlag(m_idle_flag))
			{
				// アニメーションを死んだのに変更する
				m_animation.ChangeAnimation(&m_model, die_anim, true);
			}
		}

		break;

	case die_anim: // 死んだとき
		DieUpdate();
		break;

	default:
		break;
	}


	// アニメーションの再生
	m_animation.PlayAnimation(&m_model, m_combo_flag);
	// あたり判定の更新処理
	CDUpdate();


}

//-----------------------------------------------
// 生きているときの更新処理
//-----------------------------------------------
void Mutant::LiveUpdate(Transform* target_pos, float target_r)
{

	// スタン状態になるかならないか
	if (m_stun_value <= 0 && m_stun_flag == false)
	{
		// スタン状態に移動
		m_monster_mode = STUN;
		// スタンフラグを上げる
		m_stun_flag = true;
		m_attack_flag = false;
		// アニメーション変更フラグを立てる
		m_animation.m_anim_change_flag = true;
	}



	switch (m_monster_mode)
	{
	case IDLE: // 停止状態 
		IdleActionUpdate(idle_anim);

		break;
	case RUN:
		// 待機フラグを毎回リセット
		m_idle_flag = false;
		// 待機状態または走りの時だけｗ
		// 移動処理
		//if (m_idle_flag == true || m_run_flag == true /*&& m_monster_mode == IDLE*/)
		{
			// モンスターの回転してよいようにする
			move.SetCanRotate(true);
			// 移動処理
			MoveAction(run_anim);
		}

		// 走っている間のフレームを加算する
		m_running_frame_count++;

		// 走っている時間が一定以上になったら
		if (m_running_frame_count >= CHANGE_JUMP_RUNNIG_FRAME)
		{
			// ローリングアクションをセットする
			SetRollingAction(rolling_anim, ROLLING_TARGET_DISTANCE);
			// カウントをリセットする
			m_running_frame_count = 0;
		}

		// 走っている時間が一定以上になったら
		if (m_running_frame_count >= CHANGE_JUMP_RUNNIG_FRAME)
		{
			// ジャンプアクションをセットする
			JumpAction(jump_anim, JUMP_TARGET_DISTANCE);
			// カウントをリセットする
			m_running_frame_count = 0;
		}

		

		break;
	case ATTACK:
		// スタン中はほかの攻撃処理をしてほしくない
		if (m_stun_flag == true)
		{
			break;
		}

		// ジャンプフラグが立っているとき
		if (m_jump_flag)
		{
			// ジャンプの更新処理
			JumpActionUpdate(JUMP_MOV_SPEED, JUMP_STRAT_FRAME, JUMP_END_FRAME);
			// ジャンプアニメーションが終わったときにアニメーションできた座標のずれを力ずくで直す
			if (m_animation.m_contexts[0].is_playing == false)
			{
				m_transform.pos.x += JUMP_DEVIATION_POS * sinf(TO_RADIAN(m_transform.rot.y));
				m_transform.pos.z += JUMP_DEVIATION_POS * cosf(TO_RADIAN(m_transform.rot.y));
			}
		}

		// ローリングアクション時の処理
		if (m_now_attack_anim == rolling_anim)
		{
			ActionRolling(ROLLING_SPEED, ROLLING_STRAT_FRAME, ROLLING_END_FRAME);
		}


		
		// 歩いていい範囲かをプレイヤーの向きとあっていいるかを調べる
		move.m_hit = move.TargetHit();

		// 攻撃用の関数
		// ジャンプとローリングのが行われていないとき
		if (m_jump_flag == false && m_rolling_flag == false)
		{
			// モンスターの移動ができない距離に敵がいたら
			if (!move.m_hit)
			{
				// 攻撃を始める
				AttackActionComboUpdate();
			}
		
		}

		// 攻撃中(アニメーション中)は回転してほしくない
		move.SetCanRotate(false);

		break;
	case STUN:
		// ジャンプ中にスタンされてしまったとき
		if (m_jump_flag == true)
		{
			m_transform.pos.x += JUMP_DEVIATION_POS * sinf(TO_RADIAN(m_transform.rot.y));
			m_transform.pos.z += JUMP_DEVIATION_POS * cosf(TO_RADIAN(m_transform.rot.y));
			// ジャンプフラグを下げる
			m_jump_flag = false;
		}

		StunActionUpdate(stun_down_anim, stun_up_anim, STUN_VALUE_MAX);
		//if (m_anim_ && m_animation.m_contexts[0].is_playing == false)
		//{
		//	// スタンアニメーションでずれた座標を治す
		//	m_transform.pos.z += 23 * cosf(TO_RADIAN(m_transform.rot.y));
		//	m_transform.pos.x += 23 * sinf(TO_RADIAN(m_transform.rot.y));

		//}
		break;
	}
}


//-----------------------------------------------
// 死んだときの更新処理(ゲームシーンで直接呼ぶ)
//-----------------------------------------------
void Mutant::DieUpdate()
{
	// 死んだアニメーションが追わないように
	// 一定のところまで進んだら
	if (m_animation.m_contexts[0].play_time >= 260)
	{
		// 少し前に戻す
		m_animation.m_contexts[0].play_time = 200;
	}


}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Mutant::Draw()
{
	if (m_attack_flag)
	{
		if (AttackHitGoodTiming(m_now_attack))
		{
			//m_right_hand.Draw();
			m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
		}

	}

	if (m_jump_flag)
	{
		DrawCapsule3D(m_jump_pos.VGet(), m_jump_pos.VGet(), 10.0f, 8.0f, GetColor(0, 0, 0), GetColor(0, 0, 0), TRUE);
	}


	// カプセルの描画(当たり判定)
	m_body.Draw();
	/*m_body.Draw();
	m_left_hand.Draw();
	m_right_hand.Draw();*/
	// モデルの描画 (描画を後にしないと当たり判定がちかちかする)
	m_model.DrawModel(&m_transform);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void Mutant::Exit()
{
}

//-----------------------------------------------
// 登場演出用の更新処理
//-----------------------------------------------
void Mutant::EntryUpdate()
{
	// ここですること
	// 登場シーンにあった叫びのアニメーションをつける

	// 登場アニメーションのセット(ループさせない)
	if (m_animation.ChangeFlag(true))
	{
		m_animation.ChangeAnimation(&m_model, shout_anim, true);
	}

	// アニメーションの再生
	m_animation.PlayAnimation(&m_model, m_combo_flag);
}

//-----------------------------------------------
// HPが一定まで減ったときのレベルアップ処理
//-----------------------------------------------
void Mutant::ReinforceUpdate()
{
	// HPが半分になったら
	// ダメ―ジを食らったアニメーメーションをつけて
	// そのあとに叫ぶアニメーションをつける
	// その後で攻撃を増やしたりダメージ量を増やした入りエフェクトを派手にしたり変化を加える

}

//-----------------------------------------------
// あたり判定
//-----------------------------------------------
void Mutant::CDUpdate()
{
	// キャラ本体の当たり判定のカプセル（後で消す）
	// この座標をモデルのノードをでとってくるといいかも
	m_body.CreateNodoCapsule(&m_model, 0, 7, 8.0f);

	// 左手のあたり判定
	m_left_hand.CreateNodoCapsule(&m_model, 13, 19, 3.0f);

	// 右手の当たり判定
	// 爪の部分が当たり判定がない
	m_right_hand.CreateNodoCapsule(&m_model, 9, 11, 5.0f);

	// 攻撃時の当たり当たり判定の保存
	SetHitDamage(m_left_hand, m_attack_damage[attack_punch_1], (attack_punch_1));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_2], (attack_punch_2));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_3], (attack_punch_3));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_4], (attack_punch_4));
	SetHitDamage(m_body, m_attack_damage[attack_rolling], (attack_rolling));
	SetHitDamage(m_body, m_attack_damage[attack_jump], (attack_jump));

}

//-----------------------------------------------
// ステータスバーの設定用関数
//-----------------------------------------------
void Mutant::StatusBarInit()
{
	// HPの残量を設定
	m_hp_value = HP_VALUE_MAX;
	// HPバーの設定
	m_hp_bra.Set({ 25,25 }, { SCREEN_W - 50, 25 }, &m_hp_value, true);
	m_hp_bra.SetColor(255, 100, 50, &m_hp_bra.m_color);
	m_hp_bra.SetColor(128, 128, 128, &m_hp_bra.m_back_color);
	m_hp_bra.SetColor(0, 0, 0, &m_hp_bra.m_line_color);
	m_hp_bra.SetColor(255, 255, 255, &m_hp_bra.m_character_color);
	m_hp_bra.SetName("HP");

	// スタン値の残量を設定
	m_stun_value = STUN_VALUE_MAX;
	// スタンバーの設定
	m_stun_bra.Set({ 25,70 }, { SCREEN_W - 50, 20 }, &m_stun_value, true);
	m_stun_bra.SetColor(255, 255, 0, &m_stun_bra.m_color);
	m_stun_bra.SetColor(128, 128, 128, &m_stun_bra.m_back_color);
	m_stun_bra.SetColor(0, 0, 0, &m_stun_bra.m_line_color);
	m_stun_bra.SetColor(255, 255, 255, &m_stun_bra.m_character_color);
	//m_stun_bra.SetName("STUN");
}

//-----------------------------------------------
// ステータスバー描画用関数
//-----------------------------------------------
void Mutant::StatusBarDraw()
{

	//===================
	// UIの描画
	//===================
	m_hp_bra.Draw();
	m_stun_bra.Draw();

}


//-----------------------------------------------
// 当たり判定を行って欲しいタイミングを保存する関数
// 全部の攻撃当たり判定に設定する(後でやる)
//-----------------------------------------------
void Mutant::SetAttackInfo()
{

	//------------------------------------------
	// コンポパターンが何パターンあるかの設定
	//------------------------------------------
	ComboPatternNumberInit(M_COMBO_PATTERN_MAX);

	//------------------------------------------
	// 各コンボパターンのコンボの設定
	//------------------------------------------
	for (int i = 0; i < M_COMBO_PATTERN_MAX; i++)
	{
		// 各コンボの後隙を保存
		int frame = m_combo_rear_crevice_frame[i];
		// 各コンボパターンの設定
		ComboPatternInfoInit(i, M_COMBO_NUM_MAX, frame, m_combo_pattern[i]);
	}


	//------------------------------------------
	// 当たり判定のタイミングの設定
	//------------------------------------------
	SetHitTime(attack_frame[attack_punch_1].start_frame, attack_frame[attack_punch_1].end_frame, attack_punch_1);
	SetHitTime(attack_frame[attack_punch_2].start_frame, attack_frame[attack_punch_2].end_frame, attack_punch_2);
	SetHitTime(attack_frame[attack_punch_3].start_frame, attack_frame[attack_punch_3].end_frame, attack_punch_3);
	SetHitTime(attack_frame[attack_punch_4].start_frame, attack_frame[attack_punch_4].end_frame, attack_punch_4);
	SetHitTime(attack_frame[attack_rolling].start_frame, attack_frame[attack_rolling].end_frame, attack_rolling);
	SetHitTime(attack_frame[attack_jump].start_frame, attack_frame[attack_jump].end_frame, attack_jump);
}


//-----------------------------------------------
// フラグ管理用関数
// この関数がないと色々なバグが出る
//-----------------------------------------------
void Mutant::MonsterMode(int mode)
{
	switch (mode)
	{
	case IDLE:
		m_idle_flag = true;
		m_run_flag = false;
		m_attack_flag = false;
		break;
	case RUN:
		m_idle_flag = false;
		m_run_flag = true;
		m_attack_flag = false;

		break;
	case ATTACK:

		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = true;
		break;
	case DIE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;

		break;
	case STUN:

		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;

		break;
	}
}

//-----------------------------------------------
// アニメーションの初期処理
//-----------------------------------------------

void Mutant::AnimLoadInit()
{
	// アニメーションの初期設定
	m_animation.InitAnimation(anim_max, idle_anim);
	// アニメーションの読み込み
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/idle2.mv1", idle_anim, 0, 1.0f); //!< アイドル
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Run.mv1", run_anim, 0, 1.0f); //!< ラン
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/die.mv1", die_anim, 0, 1.0f); //!< 死亡
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/shout.mv1", shout_anim, 0, 0.5f); //!< 叫び
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/hit_damage.mv1", hit_damage_anim, 0, 1.0f); //!< ダメージを受けた時
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/stun_down2.mv1", stun_down_anim, 0, 1.5f);  //!< スタンを食らった時のダウン
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/stun_up.mv1", stun_up_anim, 0, 2.0f);          //!< スタンを食らった時の起き上がり


	// もっとモンスターっぽい攻撃を探してこい
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/Punch1.mv1", attack_1_anim, 0, 1.0f); //!< 攻撃１
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/Punch2.mv1", attack_2_anim, 0, 1.0f); //!< 攻撃２
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/Punch3.mv1", attack_3_anim, 0, 1.0f); //!< 攻撃３
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/Punch4.mv1", attack_4_anim, 0, 1.0f); //!< 攻撃４
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/Rolling.mv1", rolling_anim, 0, 1.0f); //!< ローリング
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/JumpAttack.mv1", jump_anim, 0, 1.0f); //!< ジャンプ
	// 最初はデフォルトアニメーションをつけておく
	m_animation.InitAttachAnimation(&m_model, idle_anim, true);


}

