#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"


#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"
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
	SetCharacterPos({ 170,0,170 });
	// 向きの設定
	SetCharacterRot({ 0,180.0f,0 });
	// モデルのスケールの設定
	m_transform.scale.set(0.1f, 0.1, 0.1);


	// モデル画像の読み込み
	m_model.LoadModel("Data/Model/Player/Player.mv1");
	// アニメーションの初期設定
	AnimLoadInit();

	// 攻撃関連のアニメーションについての情報を保存する
	SetAttackInfo(ATTACK_ANIM_STAR, attack_anim_1, attack_kick_anim_1, COMBO_MAX);
	// 攻撃アニメーションの数分の当たり判定の入れ物を確保する
	NEWSetAttackHitDamage(ATTACK_ACTION);

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
	m_hp.Update(m_hp_value);



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
			if (m_animation.ChangeFlag(true))
			{
				// アニメーションを停止に変更する
				m_animation.ChangeAnimation(&m_model, die, true);
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
	m_animation.PlayAnimation(&m_model, m_combo_flag);

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
	// ローリングは無敵なのでローリングの間に攻撃を受けない
	// プレイヤーのモードがカウンターでないとき
	// プレイヤーが攻撃を受けたかのチェック
	if (!m_rolling_flag || m_player_mode != COUNTER)
	{
		CheckHitDamage();
	}


	// 待機状態または走りの時だけ
	if (m_idle_flag == true || m_run_flag == true)
	{
		// ローリング処理中以外 かつ 敵からのダメージを食らったフラグが立ってなかったら
		if (m_rolling_flag == false && m_damage_flag == false && m_counter_flag == false)
		{
			// 移動処理
			MoveAction(run, PLAYER_MOVE_SPEED, camera_rot);
		}
	}

	// ダメージを食らったフラグが立っていなかったら
	if (m_damage_flag == false && !m_counter_flag)
	{
		// ローリングの切り替え
		RollingActionStart();
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
			if (m_animation.ChangeFlag(m_idle_flag))
			{
				// アニメーションを停止に変更する
				m_animation.ChangeAnimation(&m_model, idle, true);
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
		RollingActionUpdate(rolling, PLAYER_ROLLING_SPEED);

		break;
	case COUNTER:
		CounterAction(counter_anim);

		break;

	case ATTACK: // 攻撃を繰り出している時

		// コンボフラグが立っていなくて
		// 攻撃アニメーションの再生が終わっていたら
		if (m_combo_flag == false && m_animation.m_contexts[0].is_playing == false)
		{
			// 待機モードにしておく
			m_player_mode = IDLE;
		}
		// カウンター攻撃が行われている場合
		// こうすることによってカウンター攻撃ちゅに回避ができなくなる
		if (m_counter_flag && m_animation.m_contexts[0].is_playing == false)
		{
			// カウンターフラグを下す
			m_counter_flag = false;
			// 待機モードにしておく
			m_player_mode = IDLE;
		}

		// 攻撃用の関数
		AttackUpdate();
		break;

	case HIT_DAMAGE: // ダメージを受けた時
		HitDamageUpdate(hit_damage);
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
	if (m_attack_flag)
	{

		// 攻撃の当たり判定行っていいときだけ
		if (AttackHitGoodTiming(m_now_attack))
		{

			// 当たり判定を描画
			if (m_now_attack == attack_counter)
			{
				m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
			}
			m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
			
		}
	}
	/*m_body.Draw();
	m_right_hand.Draw();
	m_left_hand.Draw();
	m_right_feet.Draw();
	m_left_feet.Draw();*/

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

	// 剣の当たり判定を調べるために作ったもの
	{
		// カプセルの座標１
		Vector3 top_pos;
		top_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 10,
			m_transform.pos.y + 5,
			m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 10);

		// カプセルの座標２
		Vector3 under_pos;
		under_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 5,
			m_transform.pos.y + 5,
			m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 5);


	}
	// 攻撃時の当たり当た判定の保存とダメージの設定
	SetHitDamage(m_left_hand, m_attack_damage[attack_punch_1], (attack_punch_1));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_2], (attack_punch_2));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_3], (attack_punch_3));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_1], (attack_kick_1));
	SetHitDamage(m_left_feet, m_attack_damage[attack_kick_2], (attack_kick_2));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_3], (attack_kick_3));
	SetHitDamage(m_right_feet, m_attack_damage[attack_counter], (attack_counter));
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

	m_hp.Update(m_hp_value);
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
	for (int i = 0; i < attack_max; i++)
	{
		SetHitTiming(attack_frame[i].start_frame, attack_frame[i].end_frame, attack_frame[i].can_hit_stop, i);
	}


}


//-----------------------------------------------
// アニメーションの初期処理
//-----------------------------------------------
void SamplePlayer::AnimLoadInit()
{
	// アニメーションの初期設定
	m_animation.InitAnimation(anim_max, idle);
	// アニメーションの読み込み
	m_animation.LoadAnimation("Data/Model/Player/Animation/Player_Idle.mv1", idle, 1, 1.0f); //!< アイドル
	m_animation.LoadAnimation("Data/Model/Player/Animation/Player_Run.mv1", run, 1, 1.0f);   //!< 走り
	m_animation.LoadAnimation("Data/Model/Player/Animation/rolling.mv1", rolling, 1, 2.0f);     //!< ローリング
	m_animation.LoadAnimation("Data/Model/player/Animation/hit_damage.mv1", hit_damage, 1, 1.0f); //!< ダメージを食らった時
	m_animation.LoadAnimation("Data/Model/Player/Animation/die.mv1", die, 1, 1.0f);            //!< 死んだとき
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Punch.mv1", attack_anim_1, 1, 1.0f);  //!< 攻撃１
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Punch2.mv1", attack_anim_2, 1, 2.0f); //!< 攻撃２
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Punch3.mv1", attack_anim_3, 1, 2.0f); //!< 攻撃３
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick2.mv1", attack_kick_anim_1, 1, 1.5f); //<! キック攻撃
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick3.mv1", attack_kick_anim_2, 1, 1.5f); //<! キック攻撃
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick4.mv1", attack_kick_anim_3, 1, 1.5f); //<! キック攻撃
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick4.mv1", counter_anim, 1, 1.5f);       //<! カウンター攻撃
	// 最初はデフォルトアニメーションをつけておく
	m_animation.InitAttachAnimation(&m_model, idle, true);
}

//-----------------------------------------------
// エフェクトの初期処理
//-----------------------------------------------
void SamplePlayer::EffectLoadInit()
{
	
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
		m_rolling_flag = false;
		m_counter_flag = false;
		break;
	case RUN:
		m_idle_flag = false;
		m_run_flag = true;
		m_attack_flag = false;
		m_rolling_flag = false;
		m_counter_flag = false;
		break;
	case ATTACK:

		m_idle_flag = false;
		m_run_flag = false;
		m_rolling_flag = false;
		m_attack_flag = true;
		m_counter_flag = false;
		break;
	case ROLLING:
		m_idle_flag = false;
		break;
	case HIT_DAMAGE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_rolling_flag = false;
		m_damage_flag = true;

		break;
	case DIE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_damage_flag = false;
		m_rolling_flag = false;
		break;
	}
}

