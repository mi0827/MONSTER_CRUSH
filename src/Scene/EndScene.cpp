#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"


#include "src/Model/Model.h"
#include "src/Animation/Animation.h"
#include "src/Effect/Effect.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"

#include "src/System/UIBar.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/SamplePlayer.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
//#include "src/Field/TitleField.h"
#include "src/Field/Field.h"


#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"

#include "src/Camera.h"

#include "Base/Scene_Base.h"
#include "EndScene.h"


//------------------------------------------
// コンストラクタ
//------------------------------------------
EndScene::EndScene()
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
EndScene::~EndScene()
{
}

//------------------------------------------
// 初期処理
//------------------------------------------
void EndScene::Init()
{
	// フィールドの初期化
	field.Init();


	// カメラの初期設定
	camera.PlayField_Init();

	// プレイヤーの設定
	player = new SamplePlayer;

	// プリえやーの初期設定 
	player->Init();

	// シャドーマップの設定
	ShadowMapInit();

	// 現在のシーンの設定(エンドシーン)
	m_now_scene = End;
}


//------------------------------------------
// 更新処理
//------------------------------------------
void EndScene::Update()
{

	// プレイヤーの更新処理
	player->Update(&camera.m_rot);
	

	// カメラの更新処理
	camera.Update(&player->m_transform.pos);


	// エンターを押された時にシーンの変更をする（今だけの仮）
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		// 次に行ってほしいシーンの設定をする
		SetNextScene(Title);
		// 次のシーンに移動するためのフラグを立てる
		m_scene_change_judge = true;
	}
	else
	{
		m_scene_change_judge = false;
	}

	// エンターを押された時にシーンの変更をする（今だけの仮）
	if (PushHitKey(KEY_INPUT_RETURN)&& CheckHitKey(KEY_INPUT_LSHIFT))
	{
		// 次に行ってほしいシーンの設定をする
		SetNextScene(QuestArea);
		// 次のシーンに移動するためのフラグを立てる
		m_scene_change_judge = true;
	}
	else
	{
		m_scene_change_judge = false;
	}


	// フィールドとキャラクターのあたい判定
	HitField();

}

//------------------------------------------
// 描画処理
//------------------------------------------
void EndScene::Draw()
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

	}
	UseShadowMapSet();

	
	// フォントのデフォルトサイズの保存
	int default_font_size = GetFontSize();
	// フォントサイズの設定
	SetFontSize(80);
	const char* name = "END :: RETURN";
	// 描画幅の取得
	float w = GetDrawStringWidth(name, -1);
	// 文字列の高さの取得
	float h = GetFontSize();
	// 描画座標
	Vector2 draw_pos = { SCREEN_W / 2 - w / 2, 0 };
	DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));


	// フォントのサイズをデフォルトサイズに戻す
	SetFontSize(default_font_size);
	
	

}

//------------------------------------------
// 終了処理
//------------------------------------------
void EndScene::Exit()
{
	//　シャドーマップの削除
	ExitShadowMap();
	// プレイヤークラスはポインタなので最後に開放する
	delete player;
}

//------------------------------------------
// フィールドとキャラクターの当たり判定
//------------------------------------------
void EndScene::HitField()
{
	// フィールドの地面モデルとキャラクターの当たり判定
	HitGroundCharacter(&player->m_transform.pos, &field.m_field_model);

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

