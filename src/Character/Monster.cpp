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



//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
Monster::Monster()
{
	// 最初はアイドル状態にしておく
	m_idle_flag = true;
	// 最初はアイドル状態にしておく
	m_player_mode = IDLE;
	
	// 初期座標の設定
	m_info.m_transform.pos.set(50.0f, 0.0f, 50.0f);
	
}


//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
Monster::~Monster()
{
}

//-----------------------------------------------
// 初期化処理
//-----------------------------------------------
void Monster::Init(Vector3* traget_pos)
{
	MonsterBase::BaseInit();
	// モデル画像の読み込み
	m_model.LoadModel("Data/Model/Monster/Monster.mv1");
	// アニメーションの初期設定
	Anima_Load_Init();


}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void Monster::Update()
{
	clsDx();

	// 待機状態または走りの時だけｗ
	// 移動処理
	if (m_idle_flag == true || m_run_flag == true)
	{
		Move_Update();
	}
	
	switch (m_player_mode)
	{
	case IDLE:
		if (m_idle_flag)
		{
			//Player_Mode(IDLE);
			// アニメーション変更が可能な時に
			if (m_animation.Change_Flag(m_idle_flag))
			{
				// アニメーションを停止に変更する
				m_animation.Change_Animation(&m_model, idle, true);

			}
		}

		// 最初の攻撃を判断する
		Attack_First();
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
			m_player_mode = IDLE;
		}

		// 最初の攻撃を判断する
		Attack_First();

		break;
	case ATTACK:


		// コンボフラグが立っていなくて
		// 攻撃アニメーションの再生が終わっていたら
		// 待機モードにしておく
		if (m_combo_flag == false && m_animation.m_contexts[0].is_playering == false)
		{
			m_player_mode = IDLE;
		}
		// 攻撃用の関数
		Attack_Update();
		break;
	}


	// アニメーションの再生
	m_animation.Play_Animation(&m_model, m_combo_flag);
	// あたり判定の更新処理
	CDUpdate();

	// フラグ管理用関数
	Player_Mode(m_player_mode);
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void Monster::Draw()
{
	// カプセルの描画（仮）（後で消す）
	m_body.Draw();
	m_right_hand.Draw();

	// モデルの描画 (描画を後にしないと当たり判定がちかちかする)
	m_model.DrawModel(&m_info.m_transform);
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
	m_body.CreateCapsule( m_info.m_transform.pos);
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
	m_animation.Load_Animation("Data/Model/Monster/Animation/jump.mv1", idle, 0, 1.0f); //!< アイドル

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
	m_before_pos = m_info.m_transform.pos;

	//// ベースクラスの更新処理
	//// 移動の処理が中に入っている
	BaseUpdate();

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
// 終了処理
//----------------------------------------------- 
void Monster::Player_Mode(int mode)
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

	}
}

//-----------------------------------------------
// 最初の攻撃を判断する
//-----------------------------------------------
void Monster::Attack_First()
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
		m_animation.Change_Animation(&m_model, attack_1, false);
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = attack_1;

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
		m_animation.Change_Animation(&m_model, attack_kick_1, false);
		// 攻撃アニメーション番号の保存
		m_now_attack_anim = attack_1;
		// コンボの回数をリセット
		m_combo_count = 0;

		// コンボをしていいようにする
		m_stop_combo_flag = true;
	}
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
	// コンボ可能か判断用関数
	m_combo.Combo_Judgment
	(
		&m_combo_flag,
		&m_mouse_flag,
		MOUSE_INPUT_LEFT,
		m_animation.m_contexts[0].play_time,
		m_animation.m_contexts[0].animation_total_time,
		&m_combo_count
	);
	m_combo.Combo_Judgment
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
			m_next_anim = attack_kick_1 + m_combo_count;
		}
		if (m_mouse_flag == MOUSE_INPUT_LEFT)
		{
			m_next_anim = attack_1 + m_combo_count;
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
		}

		// コンボ用のアニメーションをつける
		m_animation.Action_Change_Animation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// 現在の攻撃アニメーションを保存
			m_now_attack_anim = m_next_anim;
		}
	}
}
