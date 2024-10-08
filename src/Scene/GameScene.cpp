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
#include "src/Hit/HitStop.h"

#include "src/System/UIBar.h"

#include "src/System/Move.h"
#include "src/System/TargetMove.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/MonsterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"
#include "src/Character/Monster.h"
#include "src/Character/Mutant.h"

#include "src/Field/FieldBase.h"
#include "src/Field/Field.h"
#include "src/Field/HitField.h" 

#include "src/System/DamageAction.h"
#include "src/Action/Attack.h"

#include "src/Camera.h"



#include "src/Scene/Base/Scene_Base.h"
#include "src/Scene/GameScene.h"


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
	/*if (m_monster_num == MONSTER)
	{
		monster = new Monster;
	}*/

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
	// 現在のシーンの設定(バトルシーン)
	m_now_scene = Battle;

	// カメラの設定
	camera.SetCamera(camera.CAMERA_HEIGHT_MONSTER, camera.CAMERA_LENGTH);
}

//---------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------
void GameScene::Update()
{

	// フィールドとの当たり判定
	// 一旦当たり判定を切っておく
	HitField();

	// ゲームシーンの中でどの場面かによって処理を変える
	switch (m_what_scene)
	{
	case entry:
		EntryUpdate();
		break;
	case battle: // バトルシーン
		GameUpdate();
		break;
	case power_up: // モンスターのパワーアップ演出
		break;
	case result: // バトルシーンの後
		EndUpdate();
		break;

	}


	// Xキーを押された時にシーンの変更をする（今だけの仮）
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		// 次に行ってほしいシーンの設定をする
		SetNextScene(End);

		m_scene_change_judge = true;
	}

}

//---------------------------------------------------------------------------
// モンスターの登場演出
//---------------------------------------------------------------------------
void GameScene::EntryUpdate()
{
	// カメラの更新処理
	camera.MoveCamera(&monster->m_transform.pos, CAMERA_DIRECTIN_FLET, CAMERA_ROT_SPEED);
	monster->EntryUpdate();

	// フレームのカウントを増やす
	m_count_flame++;
	// フレームが指定の値まで増えたら
	if (m_count_flame >= ONE_SECOND_FLAME)
	{
		// タイマーを進める
		m_count_time++;
		// カウントをリセット
		m_count_flame = 0;
	}
	// タイマーが一定時間たったら(５秒)
	if (m_count_time > CHANGE_TIME)
	{
		// バトルをスターとする
		m_what_scene = battle;
	}


}

//---------------------------------------------------------------------------
// バトルシーンでの処理
//---------------------------------------------------------------------------
void GameScene::GameUpdate()
{
	// ヒットストップが起こってほしいときいがい
	if (hit_stop.Hit_Stop() == false)
	{
		// キャラクターの更新処理
		CharacterUpdate();
	}

	// カメラの更新処理
	camera.TargetCamera(&player->m_transform, &monster->m_transform.pos);
	// プレイヤーのHPが０になったら
	if (player->m_hp_value <= 0)
	{
		// プレイヤーの死亡を知らせる
		m_who_died = player_die;
		// バトルが終わったころを知らせる
		m_what_scene = result;
	}
	// モンスターのHPがゼロになったら
	if (monster->m_hp_value <= 0)
	{
		// モンスターの死亡を知らせる
		m_who_died = monster_die;
		// バトルが終わったころを知らせる
		m_what_scene = result;
	}
}

//---------------------------------------------------------------------------
// バトルが終わったときの処理
//---------------------------------------------------------------------------
void GameScene::EndUpdate()
{
	// フレームのカウントを増やす
	m_count_flame++;
	// フレームが指定の値まで増えたら
	if (m_count_flame >= ONE_SECOND_FLAME)
	{
		// タイマーを進める
		m_count_time++;
		// カウントをリセット
		m_count_flame = 0;
	}
	// タイマーが一定時間たったら(５秒)
	if (m_count_time > CHANGE_TIME)
	{
		// 次に行ってほしいシーンの設定をする
		SetNextScene(End);
		// 次のシーンに移動するためのフラグを立てる
		m_scene_change_judge = true;
	}
	// どちらのモンスターが死んだかによって処理を変える
	switch (m_who_died)
	{

	case player_die: // プレイヤーが死んだとき
		// プレイヤーを中心に
		// 右回転
		camera.MoveCamera(&player->m_transform.pos, CAMERA_DIRECTIN_RIGHT, CAMERA_ROT_SPEED);
		break;

	case monster_die: // モンスターが死んだとき
	    // モンスターを中心に
		// 左回転
		camera.MoveCamera(&monster->m_transform.pos, CAMERA_DIRECTIN_FLET, CAMERA_ROT_SPEED);
		break;

	}

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
		field.Draw();
		// モンスターの描画
		monster->Draw();
	}
	UseShadowMapSet();

	// ステータスバーの描画
	StatusDraw();

	// リザルトの時だけ描画する
	if (m_what_scene == result)
	{
		// バトル後のメッセージの描画
		VDMessage();
	}
	

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
	player->StatusBarDraw();
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

	// フィールドの地面モデルとキャラクターの当たり判定
	HitGroundCharacter(&player->m_transform.pos, &field.m_field_model);

	// モンスターと地面の当たり判定（ジャンプ処理科できる）
	HitGroundCharacterJump(&monster->m_transform.pos, &monster->m_jump_mov,
		&monster->m_jump_flag, monster->m_up_speed, monster->m_down_speed,
		&field.m_field_model);

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
void GameScene::CharacterUpdate()
{
	// プレイヤーの更新処理
	player->Update(&camera.m_rot);

	// モンスターの更新処理
	monster->Update(&player->m_transform, player->m_hit_r);



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
		int num = player->m_now_attack;
		CharacterBase::Attack_Hit_Damage* ptr = player->m_attack_hit_damage[num];
		if (player_attack_hit.HitAttack(monster->m_body, ptr->attack_hit, player->m_animation) == true)
		{
			// 攻撃の当たり判定行っていいときだけ(攻撃アニメーションの指定のフレーム間だけ)
			if (player->AttackHitGoodTiming(player->m_now_attack))
			{
			
				// ダメージを入れるのは攻撃アニメーションの間に一回だけ
				Damage_Count(player->m_attack_hit_damage[player->m_now_attack]->attack_damage, 5, &monster->m_hp_value);
			}

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
			// プレイヤーの攻撃受けたフラグが下がっているとき
			if (player->m_damage_flag == false)
			{
				// プレイヤーの攻撃受けたフラグを上げる
				player->m_damage_flag = true;
			}
			// ダメージを入れるのは攻撃アニメーションの間に一回だけ
			// モンスターの当たり判定とダメージの設定はアニメーションがもっといいのが見つかったら
			Damage_Count(monster->m_attack_hit_damage[monster->m_now_attack]->attack_damage, 5, &player->m_hp_value);
		
			// ダメージが入ったタイミングでヒットストップのカウントをリセットする
			hit_stop.Stop_Count_Reset();
		}
	}
}


//---------------------------------------------------------------------------
// キャラクタ―の攻撃に関する更新処理
// Victory(勝利)
// Defeat(敗北)
//---------------------------------------------------------------------------
void GameScene::VDMessage()
{

	// フォントサイズの設定
	// フォントのデフォルトサイズの保存
	int default_font_size = GetFontSize();
	// フォントサイズの設定
	SetFontSize(80);
	// 描画幅の取得
	float w = GetDrawStringWidth(m_massage[m_who_died].message, -1);
	// 文字列の高さの取得
	float h = GetFontSize();
	// 描画座標
	m_massage_pos = { SCREEN_W / 2 - w / 2, SCREEN_H / 2 - h };
	DrawString(m_massage_pos.x, m_massage_pos.y, m_massage[m_who_died].message, GetColor(255, 128, 50));

	// フォントのサイズをデフォルトサイズに戻す
	SetFontSize(default_font_size);
}
