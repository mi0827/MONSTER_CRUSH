#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"

#include "src/System/UIBar.h"

#include "src/Character/CharacterBase.h"
#include "SamplePlayer.h"



//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
SamplePlayer::SamplePlayer()
{
	// 最初はアイドル状態にしておく
	m_idle_flag = true;
	// 最初はアイドル状態にしておく
	m_player_mode = IDLE;

	// プレイヤーの初期座標
	SetCharacterPos({ 100,0,100 });
	// 向きの設定
	SetCharacterRot({ 0,180.0f,0 });
	// モデルのスケールの設定
	m_transform.scale.set(0.1f, 0.1, 0.1);
	// 移動の際の当たり判定のサイズの設定
	m_move_hit_size = { 3.0f,5.0f,3.0f };
}


//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
SamplePlayer::~SamplePlayer()
{
	// 終了処理をする
	Exit();
}


//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void SamplePlayer::Init()
{

	// ベースクラスで行っている初期化を呼ぶ
	CharacterBase::BaseInit(PLAYER_HIT_R, HP_MAX);
	// プレイヤーの初期座標
	m_transform.pos.set(100, 0, 100);

	// モデル画像の読み込み
	m_model.LoadModel("Data/Model/Player/Player.mv1");
	// アニメーションの初期設定
	Anima_Load_Init();

	// 攻撃アニメーションの数分の当たり判定の入れ物を確保する
	NEW_Set_Attack_Hit_Damage(ATTACK_ACTION);

	// 当たり判定をとってほしいタイミングのせってい
	SetHitTimeInit();

	// ステータスバーの設定
	StatusBarInit();
}


//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void SamplePlayer::Update(Vector3* camera_rot)
{

	// HPの値が減ったかどうか
	m_hp.Update(&m_hp_value);



	// 生きてるか死んでるかで処理を変える
	switch (m_life_and_death)
	{
	case alive: // 生きてる時の処理

		// 生きてる時の更新処理
		LiveUpdate(camera_rot);
		// プレイヤーのHPが０より多いい時
		if (m_hp_value > 0)
		{
			// 生きてる
			m_life_and_death = alive;
		}
		else // それ以外の時
		{
			// 死んだ
			m_life_and_death = die;
			// プレイヤーの状態をにDIE変更
			m_player_mode = DIE;
			// アニメーションの切り替えフラグを上げる
			m_animation.m_anim_change_flag = true;

			// 死んだアニメーションをつける
			if (m_animation.Change_Flag(true))
			{
				// アニメーションを停止に変更する
				m_animation.Change_Animation(&m_model, die, true);
			}
		}

		break;

	case die: // 死んだときの処理
		DieUpdate();
		break;

	default:
		break;
	}


	// アニメーションの再生
	m_animation.Play_Animation(&m_model, m_combo_flag);

	// あたり判定の更新処理
	CDUpdate();

	// フラグ管理用関数
	PlayerMode(m_player_mode);

	// 地面に埋まってしまった時のための処理
	if (m_transform.pos.y <= 0)
	{
		m_transform.pos.y = 0;
	}

}


//-----------------------------------------------
// 生きているときの更新処理
//-----------------------------------------------
void SamplePlayer::LiveUpdate(Vector3* camera_rot)
{

	// プレイヤーが攻撃を受けたかのチェック
	CheckHitDamage();

	// 待機状態または走りの時だけｗ
	// 移動処理
	if (m_idle_flag == true || m_run_flag == true)
	{
		// ローリング処理中以外 かつ 敵からのダメージを食らったフラグが立ってなかったら
		if (m_rolling_flag == false && m_damage_flag == false)
		{
			Move_Update(camera_rot);
		}

	}

	// ダメージを食らったフラグが立っていなかったら
	if (m_damage_flag == false)
	{
		// ローリングの切り替え
		SetRolling();
	}


	// キャラクターの状態によって行動を変える
	switch (m_player_mode)
	{
	case IDLE: // アイドルの時
		if (m_idle_flag)
		{
			//Player_Mode(IDLE);
			// アイドル状態なのでアイドルフラグを立てる
			m_idle_flag = true;

			// アニメーション変更が可能な時に
			if (m_animation.Change_Flag(m_idle_flag))
			{
				// アニメーションを停止に変更する
				m_animation.Change_Animation(&m_model, idle, true);
			}
		}
		// 最初の攻撃を判断する
		AttackFirst();
		break;
	case RUN: // 移動中 
		// 待機フラグを毎回リセット
		m_idle_flag = false;
		// run_flagfフラグがさっがたら
		if (m_run_flag == false)
		{
			// 待機フラグを上げる
			m_idle_flag = true;
			// アニメーション変更が行えるようにする
			m_animation.m_anim_change_flag = true;
			// プレイヤーの状態をIDLEに変更
			m_player_mode = IDLE;
		}

		// 最初の攻撃を判断する
		AttackFirst();

		break;
	case ROLLING: // ローリングアクションをしている時
		ActionRolling();

		break;
	case ATTACK: // 攻撃を繰り出している時

		// コンボフラグが立っていなくて
		// 攻撃アニメーションの再生が終わっていたら
		// 待機モードにしておく
		if (m_combo_flag == false && m_animation.m_contexts[0].is_playing == false)
		{
			m_player_mode = IDLE;
		}
		// 攻撃用の関数
		AttackUpdate();
		break;

	case HIT_DAMAGE: // ダメージを受けた時
		HitDamageUpdate();
		break;
	}
}

//-----------------------------------------------
// 死んだ（負けた）時の更新処理
//-----------------------------------------------
void SamplePlayer::DieUpdate()
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
void SamplePlayer::Draw()
{

	//===================
	// カプセルの描画（仮）（後で消す）
	//===================
	// 攻撃フラグをが立っていたら
	//if (m_attack_flag)
	//{
	//	// 攻撃の当たり判定行っていいときだけ
	//	if (AttackHitGoodTiming(m_now_attack))
	//	{
	//		// 当たり判定を描画
	//		m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
	//	}
	//}


	m_body.Draw();
	m_right_hand.Draw();
	m_left_hand.Draw();
	m_right_feet.Draw();
	m_left_feet.Draw();
	// モデルの描画 (描画を後にしないと当たり判定がちかちかする)
	m_model.DrawModel(&m_transform);



	//m_move_hit_box.Draw(255, 255);
}


//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void SamplePlayer::Exit()
{

}


//-----------------------------------------------
// あたり判定
//-----------------------------------------------
void SamplePlayer::CDUpdate()
{
	// キャラ本体の当たり判定のカプセル（後で消す）
	m_body.CreateNodoCapsule(&m_model, 6, 65, 3.5f);
	// 右手のあたり判定
	m_right_hand.CreateNodoCapsule(&m_model, 32, 46, 2.5f);
	// 左手の当たり判定
	m_left_hand.CreateNodoCapsule(&m_model, 8, 22, 2.5f);
	// 右足の当たり判定
	m_right_feet.CreateNodoCapsule(&m_model, 60, 64, 2.5f);
	// 左足の当たり判定
	m_left_feet.CreateNodoCapsule(&m_model, 55, 59, 2.5f);


	// 攻撃時の当たり当た判定の保存とダメージの設定
	SetHitDamage(m_left_hand, m_attack_damage[attack_punch_1], (attack_punch_1));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_2], (attack_punch_2));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_3], (attack_punch_3));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_1], (attack_kick_1));
	SetHitDamage(m_left_feet, m_attack_damage[attack_kick_2], (attack_kick_2));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_3], (attack_kick_3));

}


//-----------------------------------------------
// ステータスバーの設定用関数
//-----------------------------------------------
void SamplePlayer::StatusBarInit()
{
	// HPの設定
	m_hp.Set({ 50, SCREEN_H - 50 }, { 500,25 }, &m_hp_value, true);
	m_hp.SetColor(50, 255, 50, &m_hp.m_color);
	m_hp.SetColor(128, 128, 128, &m_hp.m_back_color);
	m_hp.SetColor(0, 0, 0, &m_hp.m_line_color);
	m_hp.SetColor(255, 0, 0, &m_hp.m_character_color);
	m_hp.SetName("HP");
}


//-----------------------------------------------
// ステータスバー描画用関数
//-----------------------------------------------
void SamplePlayer::StatusBarDraw()
{

	m_hp.Update(&m_hp_value);
	//===================
	// UIの描画
	//===================
	m_hp.Draw();
}


//-----------------------------------------------
// 当たり判定を行って欲しいタイミングを保存する関数
// 全部の攻撃に当たり判定に設定する(あとでする)
//-----------------------------------------------
void SamplePlayer::SetHitTimeInit()
{
	SetHitTime(attack_frame[attack_punch_1].start_frame, attack_frame[attack_punch_1].end_frame, attack_punch_1);
	SetHitTime(attack_frame[attack_punch_2].start_frame, attack_frame[attack_punch_2].end_frame, attack_punch_2);
	SetHitTime(attack_frame[attack_punch_3].start_frame, attack_frame[attack_punch_3].end_frame, attack_punch_3);
	SetHitTime(attack_frame[attack_kick_1].start_frame, attack_frame[attack_kick_1].end_frame, attack_kick_1);
	SetHitTime(attack_frame[attack_kick_2].start_frame, attack_frame[attack_kick_2].end_frame, attack_kick_2);
	SetHitTime(attack_frame[attack_kick_3].start_frame, attack_frame[attack_kick_3].end_frame, attack_kick_3);
}


//-----------------------------------------------
// アニメーションの初期処理
//-----------------------------------------------
void SamplePlayer::Anima_Load_Init()
{
	// アニメーションの初期設定
	m_animation.Init_Animation(anim_max, idle);
	// アニメーションの読み込み
	m_animation.Load_Animation("Data/Model/Player/Animation/Player_Idle.mv1", idle, 1, 1.0f); //!< アイドル
	m_animation.Load_Animation("Data/Model/Player/Animation/Player_Run.mv1", run, 1, 1.0f);   //!< 走り
	m_animation.Load_Animation("Data/Model/Player/Animation/rolling.mv1", rolling, 1, 2.0f);     //!< ローリング
	m_animation.Load_Animation("Data/Model/player/Animation/hit_damage.mv1", hit_damage, 1, 1.0f); //!< ダメージを食らった時
	m_animation.Load_Animation("Data/Model/Player/Animation/die.mv1", die, 1, 1.0f);            //!< 死んだとき
	m_animation.Load_Animation("Data/Model/Player/Animation/Attack/Punch.mv1", attack_anim_1, 1, 1.0f);  //!< 攻撃１
	m_animation.Load_Animation("Data/Model/Player/Animation/Attack/Punch2.mv1", attack_anim_2, 1, 2.0f); //!< 攻撃２
	m_animation.Load_Animation("Data/Model/Player/Animation/Attack/Punch3.mv1", attack_anim_3, 1, 2.0f); //!< 攻撃３
	m_animation.Load_Animation("Data/Model/Player/Animation/Attack/Kick2.mv1", attack_kick_anim_1, 1, 1.5f); //<! キック攻撃
	m_animation.Load_Animation("Data/Model/Player/Animation/Attack/Kick3.mv1", attack_kick_anim_2, 1, 1.5f); //<! キック攻撃
	m_animation.Load_Animation("Data/Model/Player/Animation/Attack/Kick4.mv1", attack_kick_anim_3, 1, 1.5f); //<! キック攻撃
	// 最初はデフォルトアニメーションをつけておく
	m_animation.Init_Attach_Animation(&m_model, idle, true);
}

//-----------------------------------------------
// プレイヤーの移動用関数
//-----------------------------------------------
void SamplePlayer::Move_Update(Vector3* camera_rot)
{
	// 毎回リセット
	m_run_flag = false;

	// 移動前の座標一旦保存しておく
	m_before_pos = m_transform.pos;

	// ベースクラスの更新処理
	// 移動の処理が中に入っている
	BaseUpdate(&m_run_flag, camera_rot, &PLAYER_MOVE_SPEED);

	// run_flag が上がってるときかつ
	// プレイヤーモードがRUN以外の時
	if (m_run_flag && m_player_mode != RUN)
	{
		// アニメーションの切り替えフラグを上げる
		m_animation.m_anim_change_flag = true;
	}

	// アニメーション変更が可能な時に
	if (m_animation.Change_Flag(m_run_flag)) {
		// 走りアニメーションに変更
		m_animation.Change_Animation(&m_model, run, true);
		// アニメーションが変わったから
		// プレイヤーモードの切り替えをする
		m_player_mode = RUN;
	}


}

//-----------------------------------------------
// フラグ管理用関数
// この関数がないと色々なバグが出る
//-----------------------------------------------
void SamplePlayer::PlayerMode(int mode)
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
	case HIT_DAMAGE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_damage_flag = true;
		break;
	case DIE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_damage_flag = false;
		break;
	}
}

//-----------------------------------------------
// 最初の攻撃を判断する
//-----------------------------------------------
void SamplePlayer::AttackFirst()
{
	// 指定のマウスボタンが押されたら
	if (PushMouseInput(MOUSE_INPUT_LEFT) /*& MOUSE_INPUT_LEFT*/)
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
		m_animation.Change_Animation(&m_model, attack_anim_1, false);
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = attack_anim_1;

		// 現在の攻撃番号を保存する
		m_now_attack = m_now_attack_anim - ATTACK_ANIM_STAR;

		// コンボの回数をリセット
		m_combo_count = 0;

		m_stop_combo_flag = true;
	}
	// 指定のマウスボタンが押されたら
	if (PushMouseInput(MOUSE_INPUT_RIGHT) /*& MOUSE_INPUT_LEFT*/)
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
		m_animation.Change_Animation(&m_model, attack_kick_anim_1, false);
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = attack_kick_anim_1;

		// 現在の攻撃番号を保存する
		m_now_attack = m_now_attack_anim - ATTACK_ANIM_STAR;

		// コンボの回数をリセット
		m_combo_count = 0;

		// コンボをしていいようにする
		m_stop_combo_flag = true;
	}
}

//-----------------------------------------------
// 攻撃に関する更新処理
//-----------------------------------------------
void SamplePlayer::AttackUpdate()
{
	// コンボをしていいフラグがったている時だけ
	if (m_stop_combo_flag)
	{
		// コンボ関数を呼ぶ
		ComboUpdate();
	}
}

//-----------------------------------------------
// ローリングアクション用のキーが押されたかの判断用関数
//-----------------------------------------------
void SamplePlayer::SetRolling()
{
	// if (m_player_mode != ROLLING) {}
	// 指定のキーが押された時
	if (PushHitKey(KEY_INPUT_SPACE))
	{
		// アクションモードをローリングにする
		m_player_mode = ROLLING;
	}
}

//-----------------------------------------------
// ローリングアクション関する更新処理
//-----------------------------------------------
void SamplePlayer::ActionRolling()
{
	// ローリングアニメーションのセット
	// ローリングフラグが上がっていないとき
	if (!m_rolling_flag)
	{
		// ローリングアニメーションをセットする
		m_animation.Change_Animation(&m_model, rolling, false);
		// ローリングフラグをあげる
		m_rolling_flag = true;
	}

	// ローリング中の移動処理
	// 向いている方向に PLAYER_ROLLING_SPEED 分移動する
	m_transform.pos.z += PLAYER_ROLLING_SPEED * cosf(TO_RADIAN(m_transform.rot.y));
	m_transform.pos.x += PLAYER_ROLLING_SPEED * sinf(TO_RADIAN(m_transform.rot.y));

	// ローリングアニメーションが終わったら(終わりだとうまく入らなかったから終わる少し前にした)
	// またはダメージを食らったフラグが上がったいたら
	if (m_animation.m_contexts[0].play_time >= m_animation.m_contexts[0].animation_total_time - 10 || m_damage_flag )
	{
		// アニメーションのチェンジフラグを上げる
		m_animation.m_anim_change_flag = true;
		// ローリングフラグを下げる
		m_rolling_flag = false;
		// 一旦アクションモードをIDLEにしておく
		m_player_mode = IDLE;
	}
}

//-----------------------------------------------
// コンボの判断をする関数
//-----------------------------------------------
void SamplePlayer::ComboUpdate()
{
	// コンボ可能か判断用関数
	// 指定のキー操作がされた場合
	m_combo.Combo_Judgment_Key
	(
		&m_combo_flag,
		&m_mouse_flag,
		MOUSE_INPUT_LEFT,
		m_animation.m_contexts[0].play_time,
		m_animation.m_contexts[0].animation_total_time,
		&m_combo_count
	);
	m_combo.Combo_Judgment_Key
	(
		&m_combo_flag,
		&m_mouse_flag,
		MOUSE_INPUT_RIGHT,
		m_animation.m_contexts[0].play_time,
		m_animation.m_contexts[0].animation_total_time,
		&m_combo_count
	);
	// コンボフラグが上がっているとき
	if (m_combo_flag)
	{
		// 今のアニメーション番号から一つ次のアニメーション
		if (m_mouse_flag == MOUSE_INPUT_RIGHT)
		{
			m_next_anim = attack_kick_anim_1 + m_combo_count;
		}
		if (m_mouse_flag == MOUSE_INPUT_LEFT)
		{
			m_next_anim = attack_anim_1 + m_combo_count;
		}
		// コンボがアニメーションの最大と同じになったら
		if (m_combo_count >= COMBO_MAX)
		{
			// コンボをストップするようにする
			m_stop_combo_flag = false;
			// コンボフラグを下げる
			m_combo_flag = false;
			// コンボの回数をリセット
			m_combo_count = 0;
			// 当たり判定の設定がバックっているので一下げる
			m_next_anim--;
		}

		// コンボ用のアニメーションをつける
		m_animation.Action_Change_Animation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// 現在の攻撃アニメーションを保存
			m_now_attack_anim = m_next_anim;
			// 現在の攻撃番号を保存する
			m_now_attack = m_now_attack_anim - ATTACK_ANIM_STAR;
		}

	}

}

//-----------------------------------------------
// 攻撃を受けたかをチェックするための関数
//-----------------------------------------------
void SamplePlayer::CheckHitDamage()
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

//-----------------------------------------------
// 攻撃を受けた時の処理
//-----------------------------------------------
void SamplePlayer::HitDamageUpdate()
{
	// 攻撃を受けた時かつ攻撃を受けた時のアニメーションが付いてないとき(フラグが下がってるとき)
	if (m_damage_flag && m_damage_anim_flag == false)
	{
		// 攻撃を受けた時のアニメーションが付いたのでフラグを上げる
		m_damage_anim_flag = true;
		// ダメージアニメーションをつける
		if (m_animation.Change_Flag(m_damage_anim_flag))
		{
			// アニメーションを停止に変更する
			m_animation.Change_Animation(&m_model, hit_damage, true);

		}
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
