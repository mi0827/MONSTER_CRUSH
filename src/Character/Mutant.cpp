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
#include "src/Character/MonsterBase.h"

#include "src/System/UIBar.h"
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
	m_transform.pos.set(50.0f, 0.0f, 50.0f);
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
	Anima_Load_Init();

	// 攻撃アニメーションの数分のあたり判定んお入れ物を確保する
	NEW_Set_Attack_Hit_Damage(ATTACK_ACTION_MAX);

	// ステータスバーの設定
	Status_Bar_Init();

	// 当たり判定をとってほしいタイミングのせってい 
	SetHitTimeInit();
	// モンスターのステータスの初期設定
	BaseInit(HP_MAX, JUMP_UP_SPEED, JUMP_DOWN_SPEED);
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Mutant::Update(Transform* target_pos, float target_r)
{
	//clsDx();
	// HPの値が減ったかどうか
	m_hp.Update(&m_hp_value);
	

	
	switch (m_life_and_death)
	{

	case alive: // 生きいるとき
		// 移動先のターゲットの設定
		BaseSetTarget(target_pos, target_r);
		LiveUpdate(target_pos,target_r);
		// モンスターのHPがより多いい時
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
			if (m_animation.Change_Flag(m_idle_flag))
			{
				// アニメーションを死んだのに変更する
				m_animation.Change_Animation(&m_model, die, true);
			}
		}

		break;

	case die: // 死んだとき
		DieUpdate();
		break;

	default:
		break;
	}
	

	// アニメーションの再生
	m_animation.Play_Animation(&m_model, m_combo_flag);
	// あたり判定の更新処理
	CDUpdate();


	// プレイヤーではこれがないとバグるが
	// モンスターではこれがあるとバグる
	// モンスターのほうがフラグ管理に失敗した
	//MonsterMode(m_monster_mode);
}

//-----------------------------------------------
// 生きているときの更新処理
//-----------------------------------------------
void Mutant::LiveUpdate(Transform* target_pos, float target_r)
{
	// 待機状態または走りの時だけｗ
	// 移動処理
	if (m_idle_flag == true || m_run_flag == true /*&& m_monster_mode == IDLE*/)
	{
		// モンスターの回転してよいようにする
		move.Set_Can_Rotate(true);
		// 移動処理
		Move_Update();
	}

	switch (m_monster_mode)
	{
	case IDLE: // 停止状態 
		if (m_idle_flag)
		{
			//Player_Mode(IDLE);
			// アニメーション変更が可能な時に
			if (m_animation.Change_Flag(m_idle_flag))
			{
				// アニメーションを停止に変更する
				m_animation.Change_Animation(&m_model, idle, true);
			}

			// 移動が止まっていたら
			if (!move.m_hit)
			{
				// 最初の攻撃を行う
				// 攻撃フラグを上げる
				m_attack_flag = true;
				Attack_First();
			}
		}
		break;
	case RUN:
		// 待機フラグを毎回リセット
		m_idle_flag = false;
		// run_flagfフラグがさっがたら
		if (m_run_flag == false)
		{
			// 待機フラグを上げる
			m_idle_flag = true;
			// アニメーション変更が行えるようにする
			m_animation.m_anim_change_flag = true;
			m_monster_mode = IDLE;
		}

		// 走っている間に一定以上の距離が空いたら
		// ジャンプ攻撃をする
		Attack_Jump();

		break;
	case ATTACK:

		// 歩いてほしくないのでフラグを
		m_idle_flag = false;
		m_run_flag = false;


		// ジャンプ攻撃時の処理
		if (m_now_attack_anim == jump)
		{
			Jump_Update();
		}

		// ローリングアクション時の処理
		if (m_now_attack_anim == rolling)
		{
			Action_Rolling(ROLLING_SPEED);
		}
		// 攻撃中(アニメーション中)は回転してほしくない
		move.Set_Can_Rotate(false);
		// 歩いていい範囲かをプレイヤーの向きとあっていいるかを調べる
		move.m_hit = move.Target_Hit();

		// アニメーションの再生が終わったとき
		if (m_animation.m_contexts[0].play_time >= m_animation.m_contexts[0].animation_total_time)
		{
			// 移動していい状態だったら
			if (move.m_hit)
			{
				// 移動フラグを立てる
				m_run_flag = true;
				//m_monster_mode = RUN;
			}
		}
		// 攻撃用の関数
		Attack_Update();

		break;
	}
}


//-----------------------------------------------
// 死んだときの更新処理
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
	//attack_hit_damage[m_now_attack].attack_hit.Draw();
	// カプセルの描画(当たり判定)
	m_body.Draw();
	m_left_hand.Draw();
	m_right_hand.Draw();
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
void Mutant::Status_Bar_Init()
{
	// HPの
	m_hp_value = HP_MAX;
	// HPの設定
	m_hp.Set({ 25,25 }, { SCREEN_W - 50, 25 }, &m_hp_value, true);
	m_hp.SetColor(255, 100, 50, &m_hp.m_color);
	m_hp.SetColor(128, 128, 128, &m_hp.m_back_color);
	m_hp.SetColor(0, 0, 0, &m_hp.m_line_color);
	m_hp.SetColor(255, 255, 255, &m_hp.m_character_color);
	m_hp.SetName("HP");

}

//-----------------------------------------------
// ステータスバー描画用関数
//-----------------------------------------------
void Mutant::Status_Bar_Draw()
{
	
	//===================
	// UIの描画
	//===================
	m_hp.Draw();
}


//-----------------------------------------------
// 当たり判定を行って欲しいタイミングを保存する関数
// 全部の攻撃当たり判定に設定する(後でやる)
//-----------------------------------------------
void Mutant::SetHitTimeInit()
{
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
	}
}

//-----------------------------------------------
// アニメーションの初期処理
//-----------------------------------------------
void Mutant::Anima_Load_Init()
{
	// アニメーションの初期設定
	m_animation.Init_Animation(anim_max, idle);
	// アニメーションの読み込み
	m_animation.Load_Animation("Data/Model/Mutant/Animation/idle.mv1", idle, 0, 1.0f); //!< アイドル
	m_animation.Load_Animation("Data/Model/Mutant/Animation/Run.mv1", run, 0, 1.0f); //!< ラン
	m_animation.Load_Animation("Data/Model/Mutant/Animation/die.mv1", die, 0, 1.0f); //!< 死亡

	// もっとモンスターっぽい攻撃を探してこい
	m_animation.Load_Animation("Data/Model/Mutant/Animation/Attack/Punch1.mv1", attack_1, 0, 1.0f); //!< 攻撃１
	m_animation.Load_Animation("Data/Model/Mutant/Animation/Attack/Punch2.mv1", attack_2, 0, 1.0f); //!< 攻撃２
	m_animation.Load_Animation("Data/Model/Mutant/Animation/Attack/Punch3.mv1", attack_3, 0, 1.0f); //!< 攻撃３
	m_animation.Load_Animation("Data/Model/Mutant/Animation/Attack/Punch4.mv1", attack_4, 0, 1.0f); //!< 攻撃４
	m_animation.Load_Animation("Data/Model/Mutant/Animation/Attack/Rolling.mv1", rolling, 0, 1.0f); //!< ローリング
	m_animation.Load_Animation("Data/Model/Mutant/Animation/Attack/jump.mv1", jump, 0, 1.0f); //!< ジャンプ
	// 最初はデフォルトアニメーションをつけておく
	m_animation.Init_Attach_Animation(&m_model, idle, true);
}

//-----------------------------------------------
// プレイヤーの移動用関数
//-----------------------------------------------
void Mutant::Move_Update()
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
	if (m_animation.Change_Flag(m_run_flag)) {
		// 走りアニメーションに変更
		m_animation.Change_Animation(&m_model, run, true);
		// アニメーションが変わったから
		// プレイヤーモードの切り替えをする
		m_monster_mode = RUN;
	}
}

//-----------------------------------------------
// 最初の攻撃を判断する
//-----------------------------------------------
void Mutant::Attack_First()
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
	int attack = GetRand(attack_rolling) + ATTACK_ANIM_START;
	m_animation.Change_Animation(&m_model, attack, false);
	// 攻撃アニメーション番号の保存
	m_now_attack_anim = attack_1;

	// 現在の攻撃番号を保存する
	m_now_attack = m_now_attack_anim - ATTACK_ANIM_START;

	m_stop_combo_flag = true;
}

//-----------------------------------------------
// 攻撃に関する更新処理
//-----------------------------------------------
void Mutant::Attack_Update()
{
	// コンボをしていいフラグがったている時だけ
	if (m_stop_combo_flag)
	{
		// コンボ関数を呼ぶ
		Combo_Update();
	}
}

//-----------------------------------------------
// ジャンプ攻撃に関する処理
//-----------------------------------------------
void Mutant::Attack_Jump()
{
	// ターゲットとの距離
	float distance = move.Get_Target_Distance();
	// ターゲットとの距離が一定以上になったら
	if (TARGET_DISTANCE <= distance)
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

		m_animation.Change_Animation(&m_model, jump, false);
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = jump;
		// 現在の攻撃番号を保存する
		m_now_attack = m_now_attack_anim - ATTACK_ANIM_START;

		m_stop_combo_flag = true;
		// ジャンプ処理は
		jump_num = STANDBY;
	}
	else
	{
		// ジャンプフラグを下げる
		m_jump_flag = false;
	}
}

//-----------------------------------------------
// ジャンプ攻撃中の処理
//-----------------------------------------------
void Mutant::Jump_Update()
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
		m_down_speed = JUMP_DOWN_SPEED;

		// フラグが立っている時かつ地面につくアニメーションの時
		if (m_jump_flag&& m_animation.m_contexts[0].play_time >= 140.0f)
		{
			// 移動先の座標の設定ターゲットの座標からモンスターのbodyの半径分
			m_transform.pos.x = move.m_target_info.m_target->pos.x - m_body.m_capsule.radius;
			m_transform.pos.z = move.m_target_info.m_target->pos.z - m_body.m_capsule.radius;
			// ジャンプフラグを下げる
			m_jump_flag = false; // 落ちる処理へ
		}
	}
	//switch (jump_num)
	//{
	//case STANDBY: // 待機
	//	// 指定のアニメーションフレームになったら指定の処理のところに行くようにすうる

		//	
		//	break;
		//case GOUP:        // 上がる
		//	m_transform.pos.y += 5;
		//	// 一定の高さまで上がったら
		//	if (m_transform.pos.y >= JUMP_HEIGHT)
		//	{
		//		// 上がるのを止めて移動の処理へ
		//		m_transform.pos.y = JUMP_HEIGHT;
		//		jump_num = MOVE;
		//	}
		//	break;

		//case MOVE:        // 座標移動
		//	// 移動先の座標の設定ターゲットの座標からモンスターのbodyの半径より少し小さいくらいずらしたとこ
		//	m_transform.pos.x = move.m_target_info.m_target->pos.x - m_body.m_capsule.radius + 5;
		//	m_transform.pos.z = move.m_target_info.m_target->pos.z - m_body.m_capsule.radius + 5;

		//	jump_num = STANDBY; // 落ちるタイミングを合わせるためにいったん待機へ
		//	break;

		//case DROPDOWN:   // 落ちる
		//	m_transform.pos.y -= 5;
		//	// 図面についたら落ちるのをやめる
		//	if (m_transform.pos.y <= 0)
		//	{
		//		m_transform.pos.y = 0;
		//	}
		//	break;
		//}

}

//-----------------------------------------------
// コンボの判断をする関数
//-----------------------------------------------
void Mutant::Combo_Update()
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
	m_combo.Combo_Judgment_Condition
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
		m_next_anim = Set_Rand_Attack();

		// コンボ用のアニメーションをつける
		m_animation.Action_Change_Animation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// 現在の攻撃アニメーションを保存
			m_now_attack_anim = m_next_anim;
			// 現在の攻撃番号を保存する
			m_now_attack = m_now_attack_anim - ATTACK_ANIM_START;
		}
	}
}


//-----------------------------------------------
// 行いたいアニメーションをランダムで選ぶための関数
//-----------------------------------------------
int Mutant::Set_Rand_Attack()
{
	// 次に行ってほしいアニメーションを入れる変数
	int next_anim = 0;

	// アニメーションが決まる名で無限ループ
	while (true)
	{
		// 次のアニメーションをランダムで入れる
		// 攻撃アニメーションスタートから攻撃アニメーションの最大までで
		next_anim = GetRand(ATTACK_ANIM_MAX) + ATTACK_ANIM_START;
		// 次に行いたいアニメーションと今のアニメーションがかぶったら
		if (next_anim == m_now_attack_anim || next_anim == 0)
		{
			// またランダムで入れなおす
			next_anim = GetRand(ATTACK_ANIM_MAX) + ATTACK_ANIM_START;
		}
		break;
	}

	// 次に行ってほしい攻撃アニメーションを返す
	return next_anim;
}
