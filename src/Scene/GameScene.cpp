#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"

#include "src/Action/Combo.h"
#include "src/System/Transform.h"
#include "src/System/InputPad.h"

#include "src/System/Spotlight.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"
#include "src/Effect/Effect.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"

#include "src/System/UIBar.h"

#include "src/System/Move.h"
#include "src/System/TargetMove.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/MonsterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"
#include "src/Character/Monster.h"
#include "src/Character/Mutant.h"

#include "src/Field/Field.h"

#include "src/System/DamageAction.h"
#include "src/Action/Attack.h"

#include "src/Camera.h"

#include "src/Scene/Base/Scene_Base.h"
#include "src/Scene/GameScene.h"

////! カメラクラスのオブジェクト
//Camera camera;
//
////! サンプルプレイヤークラスのオブジェクト
//SamplePlayer player;
////! ヒーロークラスのオブジェクト
////Hero hero;
////! モンスタークラスのオブジェクト
////Monster monster;
//Mutant mutant;
//
////! 攻撃アクションの当たり判定用のオブジェクト
//// このクラスをただの関数でもよかったけど中にbool 型の変数が一つあるのでクラスにした
//Attack player_attack_hit;
//Attack monster_attack_hit;
//
//// 仮のボックスの描画
//BoxCollision box1;
//
//// 仮の地面モデルの入れ物
//int ground = 0;
//
//// フィールドオブジェクト
//Field field;

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
GameScene::GameScene()
{
}

//---------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------
GameScene::~GameScene()
{
}

//---------------------------------------------------------------------------
// 初期処理
//---------------------------------------------------------------------------
void GameScene::GameSceneInit(int player_num, int monster_num)
{
}

//---------------------------------------------------------------------------
// 初期処理
//---------------------------------------------------------------------------
void GameScene::Init()
{
	// カメラの初期設定
	camera.PlayField_Init();

	// フィールドの初期化 
	field.Init();
}

//---------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------
void GameScene::Update(int bgm_volume, int se_volume)
{
	// キャラクターの更新処理
	Character_Update(se_volume);
	// カメラの更新処理
	camera.Update(&player->m_transform.pos);
}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void GameScene::Draw()
{
	
}

//---------------------------------------------------------------------------
// 終了処理
//---------------------------------------------------------------------------
void GameScene::Exit()
{
}

//---------------------------------------------------------------------------
// ライトの初期設定
//---------------------------------------------------------------------------
void GameScene::Light_Init()
{
}

//---------------------------------------------------------------------------
// キャラクターの初期処理
//---------------------------------------------------------------------------
void GameScene::Character_Init()
{
	// プレイヤーの初期処理
	player->Init();
}

//---------------------------------------------------------------------------
// キャラクタ―の更新処理
//---------------------------------------------------------------------------
void GameScene::Character_Update(int se_volume)
{
	// プレイヤーの更新処理
	player->Update(&camera.m_rot);

	// モンスターの更新処理
	monster->Update(&player->m_transform,player->m_hit_r);

	// フィールドのオブジェクトとプレイヤーの移動の際の壁擦り判定
	if (CheckBoxHit3D(player->m_transform.pos, player->m_move_hit_size,
		field.m_field_object[0].box_hit.m_box.hit_pos, field.m_field_object[0].box_hit.m_box.half_size))
	{
		player->MoveHitUpdate(&field.m_field_object->box_hit);
	}

	// モンスターとプレイヤーの移動の当たり判定
	if (CheckCapsuleHit(monster->m_body, player->m_body))
	{
		player->m_move.Move_Hit_Capsule(&player->m_transform.pos, player->m_body.m_capsule.radius, &monster->m_body);
	}
}

//---------------------------------------------------------------------------
// キャラクタ―の攻撃に関する更新処理
//---------------------------------------------------------------------------
void GameScene::AttackUpdate()
{
	// プレイヤーの攻撃
	if (player->m_attack_flag)
	{
		// 一つのアニメーションの間に一回だけ当たったらtrueを返すようにするようにしたいがうまくいかない
		// 詳しくは関数の中に書く

		// playerの攻撃の時に取りたい当たり判定とモンスターの体との当たり判定をとる
		// 当たり判定がうまく一定ないのはこの関数の中身のせい
		if (player_attack_hit.HitAttack(monster->m_body, player->m_attack_hit_damage[player->m_now_attack].attack_hit, player->m_animation) == true)
		{
			// ダメージを入れるのは攻撃アニメーションの間に一回だけ
			Damage_Count(player->m_attack_hit_damage[player->m_now_attack].attack_damage, 5, &monster->m_hp_value);
		}
	}
	//モンスターの攻撃
	if (monster->m_attack_flag)
	{
		// モンスターの攻撃時に使いたい当たり判定とplayerの体との当たり判定
		if (monster_attack_hit.HitAttack(player->m_body,  monster->m_attack_hit_damage[monster->m_now_attack].attack_hit,  monster->m_animation) == true)
		{
			// ダメージを入れるのは攻撃アニメーションの間に一回だけ
			// モンスターの当たり判定とダメージの設定はアニメーションがもっといいのが見つかったら
			Damage_Count(monster->m_attack_hit_damage[monster->m_now_attack].attack_damage, 5, &player->m_hp_value);
		}
	}
}
