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
	m_transform.pos.set(150.0f, 0.0f, 150.0f);
	// モデルのスケールの設定
	m_transform.scale.set(0.25f, 0.25f, 0.25f);
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
	// モデル画像の読み込み
	m_model.LoadModel("Data/Model/Mutant/Mutant.mv1");
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
void Mutant::Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera)
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
void Mutant::LiveUpdate(Transform* target_pos, float target_r, Camera* camera)
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
			JumpAction(jump_anim, JUMP_TARGET_DISTANCE);
		}
		// 走っている時間が一定以上になったら
		if (m_running_frame_count >= CHANGE_JUMP_RUNNIG_FRAME)
		{
			// ローリングアクションをセットする
			SetRollingAction(rolling_anim, ROLLING_TARGET_DISTANCE);
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
		// 攻撃のエフェクトがキック以外の時
		// 攻撃時のエフェクトは攻撃とタイミングを合わせる
		if (m_animation.m_contexts[0].play_time >= m_effect_info[m_now_attack].effect_start_anim_frame &&
			m_effect.m_play_effect_flag == true
			/*m_rolling_flag == false*/)
		{
			if (m_now_attack == attack_punch_1)
			{
				// パンチ攻撃の時のエフェクト
				EffectUpdate(punch_attack_effect, m_now_attack);
			}
			else
			{
				//ソードで攻撃するときのエフェクト
				EffectUpdate(sword_attack_effect, m_now_attack);
			}
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
	// 攻撃タイミングだけ当たり判定を描画
	//if (m_attack_flag)
	//{
	//	if (AttackHitGoodTiming(m_now_attack))
	//	{
	//		//m_right_hand.Draw();
	//		m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
	//	}
	//}

	// カプセルの描画(当たり判定)
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
	m_se.StopSound();
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
	// 咆哮用のSEの再生
	SEUpdate(roar_se_info);

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
// 攻撃を受けた時の更新処理
//-----------------------------------------------
void Mutant::ComeAttackUpdate()
{
	m_effect.m_play_effect_flag = true;

	// 再生中のエフェクトがあったら終了させる
	m_effect.StopEffect();
	//ダメージを受けた時のエフェクト
	EffectUpdate(damage_effect, damage_effect_info);

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
void Mutant::CDUpdate()
{
	// キャラ本体の当たり判定のカプセル（後で消す）
	// この座標をモデルのノードをでとってくるといいかも
	m_body.CreateNodoCapsule(&m_model, 0, 7, 12.0f);

	// 左手のあたり判定
	m_left_hand.CreateNodoCapsule(&m_model, 13, 19, 6.0f);

	// 右手の当たり判定
	// 爪の部分が当たり判定がない
	m_right_hand.CreateNodoCapsule(&m_model, 9, 11, 8.0f);

	// 攻撃時の当たり当たり判定の保存
	SetHitDamage(m_left_hand, m_attack_damage[attack_punch_1], (attack_punch_1));
	SetHitDamage(m_right_hand, m_attack_damage[attack_sword_1], (attack_sword_1));
	SetHitDamage(m_right_hand, m_attack_damage[attack_sword_2], (attack_sword_2));
	SetHitDamage(m_right_hand, m_attack_damage[attack_sword_3], (attack_sword_3));
	SetHitDamage(m_right_hand, m_attack_damage[attack_sword_4], (attack_sword_4));
	SetHitDamage(m_right_hand, m_attack_damage[attack_sword_5], (attack_sword_5));
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
	SetHitTime(attack_frame[attack_sword_1].start_frame, attack_frame[attack_sword_1].end_frame, attack_sword_1);
	SetHitTime(attack_frame[attack_sword_2].start_frame, attack_frame[attack_sword_2].end_frame, attack_sword_2);
	SetHitTime(attack_frame[attack_sword_3].start_frame, attack_frame[attack_sword_3].end_frame, attack_sword_3);
	SetHitTime(attack_frame[attack_sword_4].start_frame, attack_frame[attack_sword_4].end_frame, attack_sword_4);
	SetHitTime(attack_frame[attack_sword_5].start_frame, attack_frame[attack_sword_5].end_frame, attack_sword_5);
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

	// 攻撃アニメーション
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/Punch1.mv1", punch_attack_1_anim, 0, 1.5f); //!< パンチ攻撃１
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/SwordAttack_1.mv1", sword_attack_1_anim, 0, 1.0f); //!< ソード攻撃１
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/SwordAttack_2.mv1", sword_attack_2_anim, 0, 1.3f); //!< ソード攻撃２
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/SwordAttack_3.mv1", sword_attack_3_anim, 0, 1.5f); //!< ソード攻撃３
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/SwordAttack_4.mv1", sword_attack_4_anim, 0, 1.0f); //!< ソード攻撃４
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/SwordAttack_5.mv1", sword_attack_5_anim, 0, 1.5f); //!< ソード攻撃５
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/Rolling.mv1", rolling_anim, 0, 1.0f); //!< ローリング
	m_animation.LoadAnimation("Data/Model/Mutant/Animation/Attack/JumpAttack.mv1", jump_anim, 0, 1.0f); //!< ジャンプ


	// 最初はデフォルトアニメーションをつけておく
	m_animation.InitAttachAnimation(&m_model, idle_anim, true);
}

//-----------------------------------------------
// エフェクトの初期処理
//-----------------------------------------------
void Mutant::EffectLoadInit()
{
	// エフェクト初期化
   // エフェクトの最大数を設定
	m_effect.NewArraySecure(effect_max);
	// エフェクトの読み込み
	m_effect.LoadEffect("Data/Model/Mutant/Effect/Sword2_1.efkefc", sword_attack_effect, 1.0f); // 剣での攻撃時のエフェクト
	m_effect.LoadEffect("Data/Model/Mutant/Effect/attack1.efkefc", punch_attack_effect, 1.0f); // パンチ攻撃時のエフェクト
	m_effect.LoadEffect("Data/Model/Mutant/Effect/blood.efkefc", damage_effect, 4.0f);        // ダメージを受けた時のエフェクト
	m_effect.LoadEffect("Data/Model/Mutant/Effect/roar.efkefc", roar_effect, 1.0f);              // 咆哮時のエフェクト
}

//-----------------------------------------------
// エフェクトの更新処理
//-----------------------------------------------
void Mutant::EffectUpdate(int nodo_index, int effect_num, int effect_info_num)
{
	// エフェクトが再生可能状態なら
	if (m_effect.m_play_effect_flag == true)
	{
		// エフェクトの再生
		m_effect.PlayEffect(effect_num, m_transform.pos);
		// エフェクトが再生されたので再生してはいけないようにする
		m_effect.m_play_effect_flag = false;
	}

	// エフェクトによって座標を合わせる
	//m_effect.SetEffectPos(attack_effect[m_now_attack].pos);
	if (nodo_index == -1)
	{
		m_effect.SetEffectRotPos(m_transform.pos, m_effect_info[effect_info_num].pos, m_transform.rot);
	}
	
	// エフェクトのサイズを合わせる
	m_effect.SetEffectSize(m_effect_info[effect_info_num].size);
	
	// エフェクトの向きを合わせる
	// プレイヤーの向きにも合わせる
	m_effect.SetEffectRot(m_effect_info[effect_info_num].rot.x, m_effect_info[effect_info_num].rot.y + m_transform.rot.y, m_effect_info[effect_info_num].rot.z);
}

//-----------------------------------------------
// SEの初期処理
//-----------------------------------------------
void Mutant::SELoadInit()
{
	// SE分確保
	// SEの最大数を設定
	m_se.NewArraySecureSound(se_max);
	// SEの読み込み
	m_se.LoadSound("Data/Model/Mutant/SE/PunchAttack_2.mp3", punch_attack_se);                   // パンチ攻撃
	m_se.LoadSound("Data/Model/Mutant/SE/LightningSwordAttack_1.mp3", sword_attack_se_1);   // 剣攻撃１
	m_se.LoadSound("Data/Model/Mutant/SE/LightningSwordAttack_2.mp3", sword_attack_se_2);   // 剣攻撃２
	m_se.LoadSound("Data/Model/Mutant/SE/Damage.mp3", damage_se);									 // ダメージを受けた時
	m_se.LoadSound("Data/Model/Mutant/SE/Roar.mp3", roar_se);												  // 咆哮時
	m_se.LoadSound("Data/Model/Mutant/SE/Run3.mp3", run_se);													 // 足音

}

//-----------------------------------------------
// SEの更新処理
//-----------------------------------------------
void Mutant::SEUpdate(int se_info_num)
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

