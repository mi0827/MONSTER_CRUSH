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
#include "src/Field/HitField.h" 

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
// キャラクターの設定
//---------------------------------------------------------------------------
void GameScene::GameSceneInit()
{
	// キャラクターの設定
	if (m_player_num == SAMPLEPLAYER)
	{
		player = new SamplePlayer;
	}
	if (m_monster_num == MUTANT)
	{
		monster = new Mutant;
	}
	if (m_monster_num == MONSTER)
	{
		monster = new Monster;
	}

}


//---------------------------------------------------------------------------
// 初期処理
//---------------------------------------------------------------------------
void GameScene::Init()
{

	GameSceneInit();

	// カメラの初期設定
	camera.PlayField_Init();

	// キャラクターの初期処理
	Character_Init();

	// フィールドの初期化 
	field.Init();

	// シャドーマップの設定
	ShadowMapInit();

}

//---------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------
void GameScene::Update(int bgm_volume, int se_volume)
{
	// キャラクターの更新処理
	CharacterUpdate(se_volume);
	
	// フィールドとの当たり判定
	HitField();

	// カメラの更新処理
	camera.Update(&player->m_transform.pos);
}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void GameScene::Draw()
{
	// プレイヤーのシャドーマップのエリアのセット
	SetPlayerShadowMapArea(player->m_transform.pos);

	//-------------------------------------------------------------
		// シャドウマップの作成（ここで各オブジェクトのシャドーマップの設定）
		//-------------------------------------------------------------
		// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_player_shadowMap_handle);
	{
		// プレイヤーの描画処理
		player->Draw();

		// ヒーローの描画処理
		//hero.Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{
		// シャドウマップへキャラクターモデルの描画
		//MV1SetPosition(ground, VGet(0.0f, 0.0f, 0.0f)); // 描画するプレイヤーモデルの座標の設定
		//MV1SetRotationXYZ(ground, VGet(TO_RADIAN(0.0f), TO_RADIAN(0.0f), TO_RADIAN(0.0f))); // モデルの回転
		//MV1SetScale(ground, VGet(10, 10, 10)); // モデルの大きさ(10分の１のサイズ)
		//MV1DrawModel(ground); // モデルの描画
		field.Draw();
		// モンスターの描画
		monster->Draw();
	}

	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	//-------------------------------------------------------------
	// 各モデルの描画
	//-------------------------------------------------------------

	// バックバッファに描画する
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラの描画処理
	camera.Draw();


	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(1, m_player_shadowMap_handle);
	{
		player->Draw();

		// ヒーローの描画処理
		//hero.Draw();
	}
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// シャドウマップへキャラクターモデルの描画

		//MV1SetPosition(ground, VGet(0.0f, 0.0f, 0.0f)); // 描画するプレイヤーモデルの座標の設定
		//MV1SetRotationXYZ(ground, VGet(TO_RADIAN(0.0f), TO_RADIAN(0.0f), TO_RADIAN(0.0f))); // モデルの回転
		//MV1SetScale(ground, VGet(10, 10, 10)); // モデルの大きさ(10分の１のサイズ)
		//MV1DrawModel(ground); // モデルの描画
		field.Draw();
		// モンスターの描画
		monster->Draw();
	}
	UseShadowMapSet();

	// ステータスバーの描画
	StatusDraw();
}

//---------------------------------------------------------------------------
// 終了処理
//---------------------------------------------------------------------------
void GameScene::Exit()
{
	//　シャドーマップの削除
	ExitShadowMap();
}

//---------------------------------------------------------------------------
// ステータスバーの描画
//---------------------------------------------------------------------------
void GameScene::StatusDraw()
{
	player->Status_Bar_Draw();
	monster->Status_Bar_Draw();

}

//---------------------------------------------------------------------------
// ライトの初期設定
//---------------------------------------------------------------------------
void GameScene::LightInit()
{
}

//---------------------------------------------------------------------------
// フィールドの当たり判定の
//---------------------------------------------------------------------------
void GameScene::HitField()
{
	// 木のオブジェクトとプレイヤーの当たり判定
	for (int i = 0; i < field.TREE_MAX; i++)
	{
		// モンスターとプレイヤーの移動の当たり判定
		if (CheckCapsuleHit(field.m_hit_tree[i], player->m_body))
		{
			player->m_move.Move_Hit_Capsule(&player->m_transform.pos, player->m_body.m_capsule.radius, &field.m_hit_tree[i]);
		}
	}
	// フェンスとキャラクターの当たり判定
	for (int i = 0; i < field.FENCE_MAX; i++)
	{
		if (CheckBoxHit3D(player->m_transform.pos, player->m_move_hit_size,
			field.m_hit_fence[i].m_box.hit_pos, field.m_hit_fence[i].m_box.half_size))
		{
			player->MoveHitUpdate(&field.m_hit_fence[i]);
		}
	}


	// 石とキャラクターの当たり判定
	for (int i = 0; i < field.STONE_MAX; i++)
	{
		if (CheckBoxHit3D(player->m_transform.pos, player->m_move_hit_size,
			field.m_hit_stone[i].m_box.hit_pos, field.m_hit_stone[i].m_box.half_size))
		{
			player->MoveHitUpdate(&field.m_hit_stone[i]);
		}
	}
}

//---------------------------------------------------------------------------
// キャラクターの初期処理
//---------------------------------------------------------------------------
void GameScene::Character_Init()
{
	// プレイヤーの初期処理
	player->Init();
	// モンスターの初期処理
	monster->Init();
}

//---------------------------------------------------------------------------
// キャラクタ―の更新処理
//---------------------------------------------------------------------------
void GameScene::CharacterUpdate(int se_volume)
{
	// プレイヤーの更新処理
	player->Update(&camera.m_rot);

	// モンスターの更新処理
	monster->Update(&player->m_transform, player->m_hit_r);

	// フィールドの地面モデルとキャラクターの当たり判定
	HitGroundCharacter(&player->m_transform.pos, &field.m_field_model);
	// 今の状態だとジャンプ攻撃ができない
	// 家のデスクトップ尾モデル自身に当たり判定を設定のソースにジャンプのヒントが書いてある
	HitGroundCharacterJump(&monster->m_transform.pos, &monster->m_jump_mov,
		&monster->m_jump_flag, monster->m_up_speed, monster->m_down_speed,
		&field.m_field_model);





	// モンスターとプレイヤーの移動の当たり判定
	if (CheckCapsuleHit(monster->m_body, player->m_body))
	{
		player->m_move.Move_Hit_Capsule(&player->m_transform.pos, player->m_body.m_capsule.radius, &monster->m_body);
	}

	// 攻撃関係のアップデート
	AttackUpdate();
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
		if (player_attack_hit.HitAttack(monster->m_body, player->m_attack_hit_damage[player->m_now_attack]->attack_hit, player->m_animation) == true)
		{
			// ダメージを入れるのは攻撃アニメーションの間に一回だけ
			Damage_Count(player->m_attack_hit_damage[player->m_now_attack]->attack_damage, 5, &monster->m_hp_value);
		}
	}
	//モンスターの攻撃
	if (monster->m_attack_flag)
	{
		// モンスターの攻撃時に使いたい当たり判定とplayerの体との当たり判定
		int num = monster->m_now_attack;
		MonsterBase::Attack_Hit_Damage* ptr = monster->m_attack_hit_damage[num];
		if (monster_attack_hit.HitAttack(player->m_body, ptr->attack_hit, monster->m_animation) == true)
		{
			// ダメージを入れるのは攻撃アニメーションの間に一回だけ
			// モンスターの当たり判定とダメージの設定はアニメーションがもっといいのが見つかったら
			Damage_Count(monster->m_attack_hit_damage[monster->m_now_attack]->attack_damage, 5, &player->m_hp_value);
		}
	}
}
