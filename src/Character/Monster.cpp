#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"

#include "src/Effect/Effect.h"
#include "src/System/UIBar.h"
#include "src/Sound/Sound.h"
#include "src/Camera.h"

#include "src/Action/Combo.h"
#include "src/System/TargetMove.h"
#include "src/Character/MonsterBase.h"

#include "Monster.h"


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
Monster::Monster()
{
	// 最初はアイドル状態にしておく
	m_idle_flag = true;
	// 最初はアイドル状態にしておく
	m_monster_mode = IDLE;

	// 初期座標の設定
	m_transform.pos.set(200.0f, 0.0f, 200.0f);
	// モデルのスケールの設定
	m_transform.scale.set(0.25f, 0.25f, 0.25f);
}


//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
Monster::~Monster()
{
	Exit();
}

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void Monster::Init()
{
	// モデル画像の読み込み
	m_model.LoadModel("Data/Model/Monster/Monster.mv1");
	// キャラクターのマテリアル調整
	// モデルのマテリアルを確保
	// マテリアルのアンビエントをあげると自分にかかっている影が明るくなる
	m_model.SetMaterialAmbient({ 1.0f,1.0f,1.0f,0.0f });

	// アニメーションの初期設定
	AnimLoadInit();
	// 攻撃アニメーションに関する情報の設定
	SetAttackAnimInfo(ATTACK_ANIM_START, ATTACK_ANIM_MAX, attack_rolling);
	// 攻撃アニメーションの数分のあたり判定用の入れ物を確保する
	SetAttackHitDamage(ATTACK_ACTION_MAX);

	// エフェクトの初期化
	EffectLoadInit();
	// SEの初期化
	SELoadInit();

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
void Monster::Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera)
{

	// モンスターの状態によってのフラグ管理
	MonsterMode(m_monster_mode);

	// ステータスバーの値が減ったかどうか
	m_hp_bra.Update(m_hp_value);
	if (m_stun_flag == false)
	{
		m_stun_bra.Update(m_stun_value);
	}


	switch (m_life_and_death)
	{

	case alive: // 生きいるとき
		// 移動先のターゲットの設定
		BaseSetTarget(target_pos, target_r, body);
		LiveUpdate(target_pos, target_r, camera);
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
	}

	// アニメーションの再生
	m_animation.PlayAnimation(&m_model, m_combo_flag);
	// あたり判定の更新処理
	CDUpdate();
}

//-----------------------------------------------
// 生きているときの更新処理
//-----------------------------------------------
void Monster::LiveUpdate(Transform* target_pos, float target_r, Camera* camera)
{

	// スタン状態になるかならないかの判断
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
	// スタン値が条件以内でなおかつ攻撃を受けていないときにスタン値を回復させる
	else if (m_stun_value > 0 && m_stun_value < STUN_VALUE_MAX && m_damage_anim_flag == false)
	{
		m_not_damaged_frame++;
		if (m_not_damaged_frame % STUN_VALUE_RECOVERY_FRAME == 0)
		{
			m_stun_value += RECOVERY_STUN_VALUE;
		}
	}

	// 咆哮攻撃の処理
	if (m_stun_flag == false)
	{
		RoarSet(shout_anim, roar_se_info, camera);
	}


	switch (m_monster_mode)
	{
	case IDLE: // 停止状態 
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// モンスターの状態を攻撃状態にする
			m_monster_mode = IDLE;
		}
		IdleActionUpdate(idle_anim);
		// カウントをリセットする
		m_running_frame_count = 0;
		break;
	case RUN: // 歩いている状態
		// 待機フラグを毎回リセット
		m_idle_flag = false;
		// 待機状態または走りの時だけｗ
		// 移動処理
		// モンスターの回転してよいようにする
		m_move.SetCanRotate(true);
		// 移動処理
		MoveAction(run_anim);

		// 走っている間のフレームを加算する
		m_running_frame_count++;

		// 走っている時間が一定以上になったら
		if (m_running_frame_count >= CHANGE_JUMP_RUNNIG_FRAME)
		{
			// ジャンプアクションをセットする
			JumpAction(jump_anim, (int)JUMP_TARGET_DISTANCE);
		}
		// 走っている時間が一定以上になったら
		if (m_running_frame_count >= CHANGE_JUMP_RUNNIG_FRAME)
		{
			// ローリングアクションをセットする
			SetRollingAction(rolling_anim, (int)ROLLING_TARGET_DISTANCE);
		}

		// ランフラグが立っている間だけ
		if (m_run_flag == true && m_rolling_flag == false && m_jump_flag == false)
		{
			// SEの再生
			SEUpdate(run_se_info);
		}
		else
		{
			// 再生中のSEを止める
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}

		// モンスターの状態が攻撃状態になっていたら
		if (m_monster_mode == ATTACK)
		{
			// 再生中のSEを止める
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}
		break;

	case ATTACK: // 攻撃状態
		// スタン中はほかの攻撃処理をしてほしくない
		if (m_stun_flag == true)
		{
			m_monster_mode = STUN;
			break;
		}

		// 咆哮攻撃中はほかの処理をしてほしくない
		if (m_roar_flag == true)
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
			// ローリングアクションのあとにプレイヤーが攻撃範囲にはいていなかったら
			if (m_rolling_flag == false && HitAttackArea() == false)
			{
				break;
			}
		}

		// 攻撃用の関数
		// ジャンプとローリングのが行われていないとき
		if (m_jump_flag == false && m_rolling_flag == false)
		{
			// モンスターの移動ができない距離に敵がいたら
			// 攻撃を始める
			AttackActionComboUpdate();
		}

		// 攻撃のあったエフェクトの再生
		// 攻撃時のエフェクトは攻撃とタイミングを合わせる
		if (m_animation.m_contexts[0].play_time >= m_effect_info[m_now_attack].effect_start_anim_frame &&
			m_effect.m_play_effect_flag == true
			)
		{
			if (m_now_attack == attack_bigpunch || m_now_attack == attack_punch || m_now_attack == attack_upperpunch)
			{
				EffectUpdate(big_punch_attack_effect, m_now_attack);
			}
			else if (m_now_attack == attack_breath)
			{
				// 再生中のエフェクトを停止する
				EffectUpdate(breath_attack_effect, m_now_attack);
			}
		}

		//　エフェクトの座標の設定
		if (m_effect_info[m_now_attack].nodo_index == -1)
		{
			m_effect.SetEffectRotPos(m_transform.pos, m_effect_info[m_now_attack].pos, m_transform.rot);
		}
		else
		{
			// 指定のノードの座標をとってくる
			Vector3 pos = m_model.GetNodePos(m_effect_info[m_now_attack].nodo_index);
			// 指定のノードの向きをとってくる
			Vector3 rot = m_model.GetNodeRot(m_effect_info[m_now_attack].nodo_index);
			rot = rot + m_transform.rot;
			m_effect.SetEffectRotPosNode(pos, m_effect_info[m_now_attack].pos, rot);
		}


		// SEが設定されていない攻撃の時は再生しない
		if (m_rolling_flag == false && m_jump_flag == false)
		{
			// 攻撃にあったサウンドを再生
			if (m_animation.m_contexts[0].play_time >= m_se_info[m_now_attack].se_start_frame)
			{
				SEUpdate(m_now_attack);
			}
		}

		// 攻撃アニメーションが終わったから
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// 次のエフェクトが再生できるようにする
			m_effect.m_play_effect_flag = true;
			// 現在再生中の攻撃エフェクトを終わらせる
			m_effect.StopEffect();
			// 次のSEを再生できるようにする
			m_se.m_playing_flag = true;
			// 再生中のSEを終わらせる
			m_se.StopSound();
		}
		// 攻撃中(アニメーション中)は回転してほしくない
		m_move.SetCanRotate(false);

		break;
	case STUN: // スタン状態
		// ジャンプ中にスタンされてしまったとき
		if (m_jump_flag == true)
		{
			m_transform.pos.x += JUMP_DEVIATION_POS * sinf(TO_RADIAN(m_transform.rot.y));
			m_transform.pos.z += JUMP_DEVIATION_POS * cosf(TO_RADIAN(m_transform.rot.y));
			// ジャンプフラグを下げる
			m_jump_flag = false;
		}
		// スタンの更新処理
		StunActionUpdate(stun_down_anim, stun_up_anim, STUN_VALUE_MAX);

		break;
	case ROAR: // 咆哮状態
		// 咆哮攻撃時の処理
		RoarAction(camera);
		break;
	}
}


//-----------------------------------------------
// 死んだときの更新処理(ゲームシーンで直接呼ぶ)
//-----------------------------------------------
void Monster::DieUpdate()
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
void Monster::Draw()
{
	// モデルの描画 (描画を後にしないと当たり判定がちかちかする)
	m_model.DrawModel(&m_transform);
}


//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void Monster::Exit()
{
	m_se.StopSound();
}

//-----------------------------------------------
// 登場演出用の更新処理
//-----------------------------------------------
void Monster::EntryUpdate()
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
	// 咆哮用のSEの再生
	SEUpdate(roar_se_info);

}

//-----------------------------------------------
// HPが一定まで減ったときのレベルアップ処理
//-----------------------------------------------
void Monster::ReinforceUpdate()
{
	// HPが半分になったら
	// ダメ―ジを食らったアニメーメーションをつけて
	// そのあとに叫ぶアニメーションをつける
	// その後で攻撃を増やしたりダメージ量を増やした入りエフェクトを派手にしたり変化を加える

}

//-----------------------------------------------
// 攻撃を受けた時の更新処理
//-----------------------------------------------
void Monster::ComeAttackUpdate()
{
	m_damage_effect.m_play_effect_flag = true;

	// 再生中のエフェクトがあったら終了させる
	m_damage_effect.StopEffect();
	//ダメージを受けた時のエフェクト
	DamageEffectUpdate();

	// 攻撃を受けた時だけSEが重なってほしいため他とは違う方法で再生
	m_se.m_playing_flag = true;
	// ダメージ受けた時用のSEの再生
	m_se.PlaySound_(m_se_info[damage_se_info].se_num, m_se_info[damage_se_info].play_type, m_se_info[damage_se_info].loop);
	// SEが再生されたので再生可能状態を変更する
	m_se.m_playing_flag = false;

}

//-----------------------------------------------
// あたり判定
//-----------------------------------------------
void Monster::CDUpdate()
{
	// キャラ本体の当たり判定のカプセル（後で消す）
	// この座標をモデルのノードをでとってくるといいかも
	m_body.CreateNodoCapsule(&m_model, 5, 81, 12.0f);
	// 左手のあたり判定
	m_left_hand.CreateNodoCapsule(&m_model, 13, 25, 8.0f);
	// 右手の当たり判定
	m_right_hand.CreateNodoCapsule(&m_model, 37, 49, 8.0f);
	// 左足
	m_left_feet.CreateNodoCapsule(&m_model, 67, 64, 6.0f);
	// 右足
	m_right_feet.CreateNodoCapsule(&m_model, 69, 72, 6.0f);

	// 大パンチ攻撃専用の当たり判定
	m_big_punch_hit.CreateNodoCapsule(&m_model, 25, 49, 15.0f);

	// カプセルの座標１
	Vector3 top_pos;
	top_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 50,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 50);

	// カプセルの座標２ 
	Vector3 under_pos;
	under_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 20,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 20);
	// ブレス攻撃の当たり判定の作成
	m_breath_hit.CreateCapsuleCoordinatePos(top_pos, under_pos, 10);

	// 攻撃時の当たり判定の保存
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch], (attack_punch));
	SetHitDamage(m_big_punch_hit, m_attack_damage[attack_bigpunch], (attack_bigpunch));
	SetHitDamage(m_right_hand, m_attack_damage[attack_upperpunch], (attack_upperpunch));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick], (attack_kick));
	SetHitDamage(m_right_hand, m_attack_damage[attack_takle], (attack_takle));
	SetHitDamage(m_breath_hit, m_attack_damage[attack_breath], (attack_breath));
	SetHitDamage(m_body, m_attack_damage[attack_rolling], (attack_rolling));
	SetHitDamage(m_body, m_attack_damage[attack_jump], (attack_jump));

}

//-----------------------------------------------
// ステータスバーの設定用関数
//-----------------------------------------------
void Monster::StatusBarInit()
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
void Monster::StatusBarDraw()
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
void Monster::SetAttackInfo()
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
	SetHitTime(attack_frame[attack_punch].start_frame, attack_frame[attack_punch].end_frame, attack_punch);
	SetHitTime(attack_frame[attack_kick].start_frame, attack_frame[attack_kick].end_frame, attack_kick);
	SetHitTime(attack_frame[attack_bigpunch].start_frame, attack_frame[attack_bigpunch].end_frame, attack_bigpunch);
	SetHitTime(attack_frame[attack_breath].start_frame, attack_frame[attack_breath].end_frame, attack_breath);
	SetHitTime(attack_frame[attack_takle].start_frame, attack_frame[attack_takle].end_frame, attack_takle);
	SetHitTime(attack_frame[attack_upperpunch].start_frame, attack_frame[attack_upperpunch].end_frame, attack_upperpunch);
	SetHitTime(attack_frame[attack_rolling].start_frame, attack_frame[attack_rolling].end_frame, attack_rolling);
	SetHitTime(attack_frame[attack_jump].start_frame, attack_frame[attack_jump].end_frame, attack_jump);
}


//-----------------------------------------------
// フラグ管理用関数
// この関数がないと色々なバグが出る
//-----------------------------------------------
void Monster::MonsterMode(int mode)
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
		m_stun_flag = false;
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
		m_roar_flag = false;

		break;

	case ROAR:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_roar_flag = true;
		break;
	}
}

//-----------------------------------------------
// アニメーションの初期処理
//-----------------------------------------------
void Monster::AnimLoadInit()
{
	// アニメーションの初期設定
	m_animation.InitAnimation(anim_max, idle_anim);
	// アニメーションの読み込み
	m_animation.LoadAnimation("Data/Model/Monster/Animation/idle.mv1", idle_anim, 0, 1.0f);             //!< アイドル
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Run.mv1", run_anim, 0, 1.0f);               //!< ラン
	m_animation.LoadAnimation("Data/Model/Monster/Animation/die.mv1", die_anim, 0, 1.0f);               //!< 死亡
	m_animation.LoadAnimation("Data/Model/Monster/Animation/shout.mv1", shout_anim, 0, 0.5f);           //!< 叫び
	m_animation.LoadAnimation("Data/Model/Monster/Animation/hit_damage.mv1", hit_damage_anim, 0, 1.0f); //!< ダメージを受けた時
	m_animation.LoadAnimation("Data/Model/Monster/Animation/stun_down.mv1", stun_down_anim, 0, 1.5f);   //!< スタンを食らった時のダウン
	m_animation.LoadAnimation("Data/Model/Monster/Animation/stun_up.mv1", stun_up_anim, 0, 2.0f);       //!< スタンを食らった時の起き上がり

	// 攻撃アニメーション
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Punch.mv1", punch_attack_anim, 0, 1.5f);      //!< パンチ攻撃
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Kick.mv1", kick_attack_anim, 0, 1.0f);            //!< キック攻撃
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/BigPunch.mv1", bigpunch_attack_anim, 0, 1.3f);   //!< 大パンチ攻撃
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Breath.mv1", breath_attack_anim, 0, 1.5f);     //!< ブレス攻撃
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/tackle.mv1", tackle_attack_anim, 0, 1.0f);      //!< タックル攻撃
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/UpperPunch.mv1", upperpunch_attack_anim, 0, 1.5f); //!< アッパー攻撃
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Rolling.mv1", rolling_anim, 0, 1.0f);           //!< ローリング
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/JumpAttack.mv1", jump_anim, 0, 1.0f);           //!< ジャンプ


	// 最初はデフォルトアニメーションをつけておく
	m_animation.InitAttachAnimation(&m_model, idle_anim, true);
}

//-----------------------------------------------
// エフェクトの初期処理
//-----------------------------------------------
void Monster::EffectLoadInit()
{
	// エフェクト初期化
   // エフェクトの最大数を設定
	m_effect.NewArraySecure(effect_max);
	// エフェクトの読み込み
	m_effect.LoadEffect("Data/Model/Monster/Effect/Punch2.efkefc", punch_attack_effect, 1.0f);      // パンチ攻撃時のエフェクト
	m_effect.LoadEffect("Data/Model/Monster/Effect/Punch.efkefc", big_punch_attack_effect, 1.0f);  // 大パンチ攻撃時のエフェクト
	m_effect.LoadEffect("Data/Model/Monster/Effect/Breath.efkefc", breath_attack_effect, 1.0f);      // ブレス攻撃時のエフェクト
	// ダメージを受けた時のエフェクト


	m_damage_effect.NewArraySecure(damage_effect_max);
	m_damage_effect.LoadEffect("Data/Model/Monster/Effect/blood.efkefc", damage_effect, 4.0f);
}

//-----------------------------------------------
// エフェクトの更新処理
//-----------------------------------------------
void Monster::EffectUpdate(int effect_num, int effect_info_num)
{
	// エフェクトが再生可能状態なら
	if (m_effect.m_play_effect_flag == true)
	{
		// エフェクトの再生
		m_effect.PlayEffect(effect_num, m_transform.pos);
		// エフェクトが再生されたので再生してはいけないようにする
		m_effect.m_play_effect_flag = false;
	}

	// エフェクトのサイズを合わせる
	m_effect.SetEffectSize(m_effect_info[effect_info_num].size);
	// エフェクトの向きを合わせる
	m_effect.SetEffectRot(m_effect_info[effect_info_num].rot.x, m_effect_info[effect_info_num].rot.y + m_transform.rot.y, m_effect_info[effect_info_num].rot.z);
}

//-----------------------------------------------
// ダメージ受けた時専用エフェクトの更新処理
//-----------------------------------------------
void Monster::DamageEffectUpdate()
{
	// エフェクトが再生可能状態なら
	if (m_effect.m_play_effect_flag == true)
	{
		// エフェクトの再生
		m_damage_effect.PlayEffect(damage_effect, m_transform.pos);
		// エフェクトが再生されたので再生してはいけないようにする
		m_damage_effect.m_play_effect_flag = false;
	}
	Vector3 pos = m_model.GetNodePos(m_damage_effect_info.nodo_index);
	m_damage_effect.SetEffectRotPos(pos, m_damage_effect_info.pos, m_transform.rot);
	// エフェクトのサイズを合わせる
	m_damage_effect.SetEffectSize(m_damage_effect_info.size);
	// エフェクトの向きを合わせる
	m_damage_effect.SetEffectRot(m_damage_effect_info.rot.x, m_damage_effect_info.rot.y + m_transform.rot.y, m_damage_effect_info.rot.z);
}

//-----------------------------------------------
// SEの初期処理
//-----------------------------------------------
void Monster::SELoadInit()
{
	// SE分確保
	// SEの最大数を設定
	m_se.NewArraySecureSound(se_max);
	// SEの読み込み
	m_se.LoadSound("Data/Model/Monster/SE/punch.mp3", punch_attack_se_1);   // パンチ攻撃
	m_se.LoadSound("Data/Model/Monster/SE/big_punch.mp3", big_punch_attack_se);   // 大パンチ
	m_se.LoadSound("Data/Model/Monster/SE/breath.mp3", breath_attack_se);   // ブレス攻撃
	m_se.LoadSound("Data/Model/Monster/SE/Damage.mp3", damage_se);	     // ダメージを受けた時
	m_se.LoadSound("Data/Model/Monster/SE/Roar.mp3", roar_se);				      // 咆哮時
	m_se.LoadSound("Data/Model/Monster/SE/Run3.mp3", run_se);				   	  // 足音

}

//-----------------------------------------------
// SEの更新処理
//-----------------------------------------------
void Monster::SEUpdate(int se_info_num)
{
	// SEが再生されていかつSE再生可能状態なら
	if (m_se.PlayingSound() == false && m_se.m_playing_flag)
	{
		// SEの再生
		m_se.PlaySound_(m_se_info[se_info_num].se_num, m_se_info[se_info_num].play_type, m_se_info[se_info_num].loop);
		// SEが再生されたので再生可能状態を変更する
		m_se.m_playing_flag = false;
	}
}

