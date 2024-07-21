#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"

#include "src/System/Spotlight.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

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
	m_transform.pos.set(50.0f, 0.0f, 50.0f);
	// モデルのスケールの設定
	m_transform.scale.set(0.2f, 0.2f, 0.2f);
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
	// アニメーションの初期設定
	Anima_Load_Init();

	// 足元にボックスの当たり判定の作成
	m_move_hit.CreateBox(m_transform.pos, M_MOVE_SIZE);
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Monster::Update(Transform* traget_pos, float target_r)
{
	clsDx();

	MonsterBase::BaseInit(traget_pos, target_r);

	// 待機状態または走りの時だけｗ
	// 移動処理
	if (m_idle_flag == true || m_run_flag == true)
	{
		Move_Update();
	}
	
	// 足元の当たり判定の更新処理
	// 当たり判定の座標をモンスターを中心にしたいので
	// M_MOVE_SIZEの半分分ずらす(モンスターの座標からハーフ分ずらす)
	m_move_hit.SetPos(m_transform.pos-M_MOVE_SIZE_HALF);

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



		break;
	case ATTACK:


		// コンボフラグが立っていなくて
		// 攻撃アニメーションの再生が終わっていたら
		// 待機モードにしておく
		if (m_combo_flag == false && m_animation.m_contexts[0].is_playering == false)
		{
			m_monster_mode = IDLE;
		}
		// 攻撃用の関数
		Attack_Update();
		break;
	}


	// アニメーションの再生
	m_animation.Play_Animation(&m_model, m_combo_flag);
	// あたり判定の更新処理
	CDUpdate();
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Monster::Draw()
{
	// カプセルの描画（仮）（後で消す）
	m_body.Draw();
	m_right_hand.Draw();
	
	m_move_hit.Draw(255, 100);
	// モデルの描画 (描画を後にしないと当たり判定がちかちかする)
	m_model.DrawModel(&m_transform);
}

//-----------------------------------------------
// 終了処理
//-----------------------------------------------
void Monster::Exit()
{
}

//-----------------------------------------------
// あたり判定
//-----------------------------------------------
void Monster::CDUpdate()
{
	// キャラ本体の当たり判定のカプセル（後で消す）
	m_body.CreateCapsule(m_transform.pos);
	m_body.SetSize({ 0.0f,15.0f, 0.0f }, 2.5);

	// 右手のあたり判定
	m_right_hand.CreateNodoCapsule(&m_model, 10);
	m_right_hand.NodoSetSize(&m_model, 9, 1.0f);

}

//-----------------------------------------------
// アニメーションの初期処理
//-----------------------------------------------
void Monster::Anima_Load_Init()
{
	// アニメーションの初期設定
	m_animation.Init_Animation(anim_max, idle);
	// アニメーションの読み込み
	m_animation.Load_Animation("Data/Model/Monster/Animation/idle.mv1", idle, 0, 1.0f); //!< アイドル
	m_animation.Load_Animation("Data/Model/Monster/Animation/Run.mv1", run, 0, 1.0f); //!< ラン

	// もっとモンスターっぽい攻撃を探してこい

	m_animation.Load_Animation("Data/Model/Monster/Animation/Punch_1.mv1", attack_1, 0, 1.0f); //!< 攻撃１
	m_animation.Load_Animation("Data/Model/Monster/Animation/rolling.mv1", rolling, 0, 1.0f); //!< ローリング
	m_animation.Load_Animation("Data/Model/Monster/Animation/jump.mv1", jump, 0, 1.0f); //!< ジャンプ
	// 最初はデフォルトアニメーションをつけておく
	m_animation.Init_Attach_Animation(&m_model, idle, true);
}

//-----------------------------------------------
// プレイヤーの移動用関数
//-----------------------------------------------
void Monster::Move_Update()
{
	// 毎回リセット
	m_run_flag = false;

	// 移動前の座標一旦保存しておく
	m_before_pos = m_transform.pos;

	//// ベースクラスの更新処理
	//// 移動の処理が中に入っている
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
void Monster::Attack_First()
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
	m_animation.Change_Animation(&m_model, attack_1, false);
	// 攻撃アニメーション番号の保存
	m_now_attack_anim = attack_1;


	m_stop_combo_flag = true;
}

//-----------------------------------------------
// 攻撃に関する更新処理
//-----------------------------------------------
void Monster::Attack_Update()
{
	// コンボをしていいフラグがったている時だけ
	if (m_stop_combo_flag)
	{
		// コンボ関数を呼ぶ
		Combo_Update();
	}
}

//-----------------------------------------------
// コンボの判断をする関数
//-----------------------------------------------
void Monster::Combo_Update()
{
	// コンボを行っていい状態なのはかを保存する変数
	bool combo_jug;
	// TargetMoveがターゲットと接しているそうでないかで変わる
	// 接していず移動可能状態になれば
	if (move.m_hit)
	{
		// コンボをできる状態でない
		combo_jug = false;
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
		m_next_anim = Set_Rand_Attack();

		// コンボ用のアニメーションをつける
		m_animation.Action_Change_Animation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// 現在の攻撃アニメーションを保存
			m_now_attack_anim = m_next_anim;
		}
	}
}


//-----------------------------------------------
// 行いたいアニメーションをランダムで選ぶための関数
//-----------------------------------------------
int Monster::Set_Rand_Attack()
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
		if (next_anim == m_now_attack_anim)
		{
			// またランダムで入れなおす
			next_anim = GetRand(ATTACK_ANIM_MAX) + ATTACK_ANIM_START;
		}
		break;
	}

	// 次に行ってほしい攻撃アニメーションを返す
	return next_anim;
}
