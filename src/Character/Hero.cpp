#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"



#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"
#include "src/Hit/Hit.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/Character/CharacterBase.h"
#include "Hero.h"



//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
Hero::Hero()
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
Hero::~Hero()
{
	// 終了処理をする
	Exit();
}

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void Hero::Init()
{

	// ベースクラスで行っている初期化を呼ぶ
	CharacterBase::BaseInit(PLAYER_HIT_R, HP_MAX);



	// プレイヤーの初期座標
	SetCharacterPos({ 250,0,250 });
	// 向きの設定
	SetCharacterRot({ 0,180.0f,0 });
	// モデルのスケールの設定
	m_transform.scale.set(0.1f, 0.1f, 0.1f);

	// キャラクターモデル画像の読み込み
	m_model.LoadModel("Data/Model/Hero/Hero.mv1");
	// キャラクターのマテリアル調整
	// モデルのマテリアルを確保
	// マテリアルのアンビエントをあげると自分にかかっている影が明るくなる
	m_model.SetMaterialAmbient({ 1.0f,1.0f,1.0f,0.0f });

	// アニメーションの初期設定
	AnimLoadInit();
	// エフェクトの初期設定
	EffectLoadInit();
	// サウンドの初期化
	SELoadInit();
	// 攻撃関連のアニメーションについての情報を保存する
	SetAttackInfo(ATTACK_ANIM_STAR, attack_sword_anim_1, attack_sword_anim_4, COMBO_MAX);
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
void Hero::Update(Vector3* camera_rot)
{

	// 移動前の座標一旦保存しておく
	m_before_pos = m_transform.pos;
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
void Hero::LiveUpdate(Vector3* camera_rot)
{
	// モデルのY軸回転に制限をかけている
	m_model.RotationYLimits();
	// ローリングは無敵なのでローリングの間に攻撃を受けない
	// プレイヤーのモードがカウンターでないとき
	// プレイヤーが攻撃を受けたかのチェック
	if (!m_rolling_flag || m_player_mode != COUNTER)
	{
		CheckHitDamage();
		if (m_player_mode== HIT_DAMAGE)
		{
			// 再生中のSEを止める
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}
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
		if (m_player_mode == ROLLING)
		{
			// 再生中のSEを止める
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}
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
		else
		{
			// ランフラグが立っているときだけ
			// SEの再生
			SEUpdate(run_se_info);
		}
		
		// 最初の攻撃を判断する
		AttackFirst();
		// ランフラグが下がったときか攻撃に移行するタイミングで
		if (m_player_mode == ATTACK || m_run_flag == false)
		{
			// 再生中のSEを止める
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}

		break;
	case ROLLING: // ローリングアクションをしている時

		// ローリングの更新処理
		RollingActionUpdate(rolling, static_cast<int>(PLAYER_ROLLING_SPEED));
		

		break;
	case COUNTER:

		// カウンター攻撃をセット
		CounterAction(counter_anim);

		m_effect.StopEffect();
		// エフェクト再生可能状態にしておく
		m_effect.m_play_effect_flag = true;

		break;

	case ATTACK: // 攻撃を繰り出している時

		// コンボフラグが立っていなくて
		// 攻撃アニメーションの再生が終わっていたら
		if (m_combo_flag == false && m_animation.m_contexts[0].is_playing == false)
		{
			// 待機モードにしておく
			m_player_mode = IDLE;
			m_combo.ChangeComboMode(m_combo.COMBO_FINISH);

		}

		// カウンター攻撃が行われている場合
		// こうすることによってカウンター攻撃中に回避ができなくなる
		if (m_counter_flag && m_animation.m_contexts[0].is_playing == false)
		{
			// カウンターフラグを下す
			m_counter_flag = false;
			// 待機モードにしておく
			m_player_mode = IDLE;
		}

		// 攻撃用の関数
		AttackUpdate();


		// 攻撃のエフェクトがキック以外の時
		// 攻撃時のエフェクトは攻撃とタイミングを合わせる
		if (m_animation.m_contexts[0].play_time >= m_effect_info[m_now_attack].effect_start_anim_frame &&
			m_effect.m_play_effect_flag == true)
		{
			m_effect.StopEffect();
			if (m_now_attack == attack_kick_1 || m_now_attack == attack_kick_2)
			{
				// キック攻撃の時のエフェクト
				EffectUpdate(attack_kick_effect, m_now_attack);
			}
			else
			{
				// 剣で攻撃するときのエフェクト
				EffectUpdate(attack_sword_effect, m_now_attack);
			}
		}
		// 攻撃にあったサウンドを再生
		if (m_animation.m_contexts[0].play_time >= m_se_info[m_now_attack].se_start_frame)
		{
			SEUpdate(m_now_attack);
		}
		

		if (m_animation.m_contexts[0].is_playing == false)
		{
			// 攻撃アニメーションが終わったから
			// 次のエフェクトが再生できるようにする
			m_effect.m_play_effect_flag = true;
			// 次のSEを再生できるようにする
			m_se.m_playing_flag = true;
			// 再生中のSEを終わらせる
			m_se.StopSound();
		}

		break;

	case HIT_DAMAGE: // ダメージを受けた時
		HitDamageUpdate(hit_damage);
		break;
	}
}

//-----------------------------------------------
// 死んだ（負けた）時の更新処理
//-----------------------------------------------
void Hero::DieUpdate()
{
	// 死んだアニメーションが追わないように
	// 一定のところまで進んだら
	if (m_animation.m_contexts[0].play_time >= 140)
	{
		// 少し前に戻す
		m_animation.m_contexts[0].play_time = 140;
	}
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Hero::Draw()
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
	// m_body.Draw();
	/*m_right_hand.Draw();
	m_left_hand.Draw();
	m_right_feet.Draw();
	m_left_feet.Draw();*/
	//m_sword.Draw();
	// モデルの描画 (描画を後にしないと当たり判定がちかちかする)
	m_model.DrawModel(&m_transform);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void Hero::Exit()
{
	// 再生中のSEを止める
	m_se.StopSound();
}

//-----------------------------------------------
// あたり判定
//-----------------------------------------------
void Hero::CDUpdate()
{
	// キャラ本体の当たり判定のカプセル（後で消す）
	//m_body.CreateNodoCapsule(&m_model, 6, 65, 3.5f);
	m_body.CreateCapsule(m_transform.pos, { 0.0f, 20.0f, 0.0f },3.5f);
	// 右手のあたり判定
	m_right_hand.CreateNodoCapsule(&m_model, 32, 46, 2.5f);
	// 左手の当たり判定
	m_left_hand.CreateNodoCapsule(&m_model, 8, 22, 2.5f);
	// 右足の当たり判定
	m_right_feet.CreateNodoCapsule(&m_model, 60, 64, 2.5f);
	// 左足の当たり判定
	m_left_feet.CreateNodoCapsule(&m_model, 55, 59, 2.5f);

	// 剣の当たり判定を調べるために作ったもの
	// カプセルの座標１
	Vector3 top_pos;
	top_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 11,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 11);

	// カプセルの座標２
	Vector3 under_pos;
	under_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 8,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 8);
	// 剣用の当たり判定の作成
	m_sword.CreateCapsuleCoordinatePos(top_pos, under_pos, 10);

	// 攻撃時の当たり当た判定の保存とダメージの設定
	SetHitDamage(m_sword, m_attack_damage[attack_sword_1], (attack_sword_1));
	SetHitDamage(m_sword, m_attack_damage[attack_sword_2], (attack_sword_2));
	SetHitDamage(m_sword, m_attack_damage[attack_sword_3], (attack_sword_3));
	SetHitDamage(m_sword, m_attack_damage[attack_sword_4], (attack_sword_4));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_1], (attack_kick_1));
	SetHitDamage(m_left_feet, m_attack_damage[attack_kick_2], (attack_kick_2));
	SetHitDamage(m_sword, m_attack_damage[attack_counter], (attack_counter));

}

//-----------------------------------------------
// ステータスバーの設定用関数
//-----------------------------------------------
void Hero::StatusBarInit()
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
// ステータス描画用
//-----------------------------------------------
void Hero::StatusBarDraw()
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
void Hero::SetHitTimeInit()
{
	for (int i = 0; i < attack_max; i++)
	{
		SetHitTiming(attack_frame[i].start_frame, attack_frame[i].end_frame, attack_frame[i].can_hit_stop, i);
	}

}


//-----------------------------------------------
// アニメーションの初期処理
//-----------------------------------------------
void Hero::AnimLoadInit()
{
	// アニメーションの初期設定
	m_animation.InitAnimation(anim_max, idle);
	// アニメーションの読み込み
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Idle.mv1", idle, 0, 1.0f); //!< アイドル
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Run.mv1", run, 0, 1.0f);   //!< 走り
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Rolling.mv1", rolling, 0, 1.5f);   //!< ローリング
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Hit_damage.mv1", hit_damage, 0, 1.0f);   //!< 攻撃が当たったときのダメージ
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Die.mv1", die, 0, 1.0f);   //!< 死んだとき
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword1.mv1", attack_sword_anim_1, 0, 2.5f); //!< 剣攻撃１ 
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword2.mv1", attack_sword_anim_2, 0, 2.5f); //!< 剣攻撃２
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword3.mv1", attack_sword_anim_3, 0, 2.5f); //!< 剣攻撃３
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword4.mv1", attack_sword_anim_4, 0, 2.3f); //!< 剣攻撃４
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/Kick1.mv1", attack_kick_anim_1, 0, 2.3f); //<! キック攻撃１
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/Kick2.mv1", attack_kick_anim_2, 0, 2.3f); //<! キック攻撃２
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/counter3.mv1", counter_anim, 0, 3.5f);     //<! カウンター攻撃
	// 最初はデフォルトアニメーションをつけておく
	m_animation.InitAttachAnimation(&m_model, idle, true);
}

//-----------------------------------------------
// エフェクトの初期処理
//-----------------------------------------------
void Hero::EffectLoadInit()
{
	// エフェクト初期化
	// エフェクトの最大数を設定
	m_effect.NewArraySecure(effect_max);
	// エフェクトの読み込み
	m_effect.LoadEffect("Data/Model/Hero/Effect/Simple_Ribbon_Sword.efkefc", attack_sword_effect, 1.0f); // 剣攻撃時のエフェクト
	m_effect.LoadEffect("Data/Model/Hero/Effect/kick.efkefc", attack_kick_effect, 1.0f); // キック攻撃時のエフェクト
	m_effect.LoadEffect("Data/Model/Hero/Effect/HitDamage.efkefc", damage_effect, 1.0f); // ダメージを受けた時のエフェクト
	m_effect.LoadEffect("Data/Model/Hero/Effect/rolling.efkefc", rolling_effect, 1.0f); // ローリング時のエフェクト
}

//-----------------------------------------------
// エフェクトの更新処理
//-----------------------------------------------
void Hero::EffectUpdate(int effect_num, int effect_info_num)
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
	m_effect.SetEffectRotPos(m_transform.pos, m_effect_info[effect_info_num].pos, m_transform.rot);
	// エフェクトのサイズを合わせる
	m_effect.SetEffectSize(m_effect_info[effect_info_num].size);
	// エフェクトの向きを合わせる
	// プレイヤーの向きにも合わせる
	m_effect.SetEffectRot(m_effect_info[effect_info_num].rot.x, m_effect_info[effect_info_num].rot.y + m_transform.rot.y, m_effect_info[effect_info_num].rot.z);

}

//-----------------------------------------------
// SEの初期処理
//-----------------------------------------------
void Hero::SELoadInit()
{
	// SEの初期化
	// SEの最大数を設定
	m_se.NewArraySecureSound(se_max);
	// SEの読み込み
	m_se.LoadSound("Data/Model/Hero/SE/SwordAttack_1.mp3", sword_attack_se_1); // 剣攻撃１
	m_se.LoadSound("Data/Model/Hero/SE/SwordAttack_3.mp3", sword_attack_se_2); // 剣攻撃２
	m_se.LoadSound("Data/Model/Hero/SE/KickAttack_1.mp3", kick_attack_se_1); // キック攻撃１
	m_se.LoadSound("Data/Model/Hero/SE/KickAttack_2.mp3", kick_attack_se_2); // キック攻撃２
	m_se.LoadSound("Data/Model/Hero/SE/Run_1.mp3", run_se); // 足音
	m_se.LoadSound("Data/Model/Hero/SE/Rolling.mp3", rolling_se); // ローリング
	m_se.LoadSound("Data/Model/Hero/SE/Damage.mp3", damage_se); // 攻撃を受けた時のサウンド

}

//-----------------------------------------------
// SEの更新処理
//-----------------------------------------------
void Hero::SEUpdate(int se_info_num)
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

//-----------------------------------------------
// フラグ管理用関数
// この関数がないと色々なバグが出る
//-----------------------------------------------
void Hero::PlayerMode(int mode)
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
		m_attack_flag = false;
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

//-----------------------------------------------
// 攻撃を受けた時の更新処理
//-----------------------------------------------
void Hero::ComeAttackUpdate()
{
	// エフェクト再生可能状態にする
	m_effect.m_play_effect_flag = true;
	// ダメージを受けた時のエフェクト
	EffectUpdate(damage_effect, damage_effect_info);

	//再生中のSEを終わらせる
	//m_se.StopSound();
	// SE再生可能にしておく
	//m_se.m_playing_flag = true;
	// 攻撃を受けた時用のSEの再生
	SEUpdate(damage_se_info);

}


