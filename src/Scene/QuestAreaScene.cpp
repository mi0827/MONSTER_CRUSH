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
#include "src/Character/Receptionist.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"


#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"

#include "src/Camera.h"

#include "Base/Scene_Base.h"
#include "QuestAreaScene.h"



//------------------------------------------
// 初期処理
//------------------------------------------
void QuestAreaScene::Init()
{
	// フィールドの初期化
	field.Init();


	// カメラの初期設定
	camera.PlayField_Init();

	// プレイヤーの設定
	player = new SamplePlayer;

	// プレイヤーの初期設定 
	player->Init();

	// 受付嬢の初期設定
	receptionist.Init();
	// 受付嬢と話せるエリアの設定
	InitArea();
	// シャドーマップの設定
	ShadowMapInit();
}


//------------------------------------------
// 更新処理
//------------------------------------------
void QuestAreaScene::Update()
{

	// プレイヤーの更新処理
	player->Update(&camera.m_rot);
	// 受付嬢のの更新処理
	receptionist.Update();

	// 受付嬢とプレイヤーの移動の当たり判定
	if (CheckCapsuleHit(receptionist.m_hit_body, player->m_body))
	{
		player->m_move.Move_Hit_Capsule(&player->m_transform.pos, player->m_body.m_capsule.radius, &receptionist.m_hit_body);
	}
	// フィールドとキャラクターの当たり判定
	HitField();


	// プレイヤーが受付嬢と話せる範囲に入ったかの確認
	if (CheckCapsuleHit(m_area, player->m_body))
	{
		// Xキーを押された時にシーンの変更をする（今だけの仮）
		if (PushHitKey(KEY_INPUT_F))
		{
			m_scene_change_judge = true;
		}
		// 話せるエリアに入ったからフラグを上げる
		m_area_hit = true;
	}
	else
	{
		// 範囲外にいるときはフラグを下げる
		m_area_hit = false;
	}

	// カメラの更新処理
	camera.Update(&player->m_transform.pos);


	

}

//------------------------------------------
// 描画処理
//------------------------------------------
void QuestAreaScene::Draw()
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
		// 受付嬢の描画
		receptionist.Draw();

		// フィールドの描画
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

		// 受付嬢の描画
		receptionist.Draw();
		// シャドウマップへキャラクターモデルの描画
		field.Draw();

	}
	UseShadowMapSet();


	// エリアを描画（仮後で消す）
	m_area.Draw();


	// 仮のフォントの描画
	// 後で変更予定
	// フォントのデフォルトサイズの保存
	int default_font_size = GetFontSize();
	// フォントサイズの設定
	SetFontSize(80);
	const char* name = "F : 話す";
	// 描画幅の取得
	float w = GetDrawStringWidth(name, -1);
	// 文字列の高さの取得
	float h = GetFontSize();
	// 描画座標
	Vector2 draw_pos = { SCREEN_W / 2 - w / 2 + 200, SCREEN_H / 2 - h };
	if (m_area_hit)
	{
		DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));
	}

	// フォントのサイズをデフォルトサイズに戻す
	SetFontSize(default_font_size);



}

//------------------------------------------
// 終了処理
//------------------------------------------
void QuestAreaScene::Exit()
{
	//　シャドーマップの削除
	ExitShadowMap();

}

//------------------------------------------
// フィールドとキャラクターの当たり判定
//------------------------------------------
void QuestAreaScene::HitField()
{

	// フィールドの地面モデルとプレイヤーの当たり判定
	HitGroundCharacter(&player->m_transform.pos, &field.m_field_model);
	// 受付嬢
	HitGroundCharacter(&receptionist.m_transform.pos, &field.m_field_model);


	// 木のオブジェクトとプレイヤーの当たり判定
	// なぜか２本だけ当たり判定がどうしない
	for (int i = 0; i < field.TREE_MAX; i++)
	{
		// モンスターとプレイヤーの移動の当たり判定
		if (CheckCapsuleHit(field.m_hit_tree[i], player->m_body))
		{
			player->m_move.Move_Hit_Capsule(&player->m_transform.pos, player->m_body.m_capsule.radius, &field.m_hit_tree[i]);
		}
	}

	// プレイヤーとフィールドを囲ってる四角との当たり判定
	for (int i = 0; i < field.AROUND_MAX; i++)
	{
		if (CheckBoxHit3D(player->m_transform.pos, player->m_move_hit_size,
			field.m_hit_around[i].m_box.hit_pos, field.m_hit_around[i].m_box.half_size))
		{
			player->MoveHitUpdate(&field.m_hit_around[i]);
		}
	}

}

//------------------------------------------
// 話せるようになるエリアの設定
//------------------------------------------
void QuestAreaScene::InitArea()
{
	// カプセル座標は受付嬢と同じ座標にする
	Vector3 pos = receptionist.m_transform.pos;
	Vector3 pos2 = { 0.0f, 60.0f, 0.0f };
	// 半径の設定
	float r = AREA_FIELD_R;
	// カプセルの作成
	m_area.CreateCapsule(pos, pos2, r);
}
