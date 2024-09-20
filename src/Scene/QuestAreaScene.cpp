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

	// フィールドの地面モデルとプレイヤーの当たり判定
	HitGroundCharacter(&player->m_transform.pos, &field.m_field_model);
	// 受付嬢
	HitGroundCharacter(&receptionist.m_transform.pos, &field.m_field_model);



	// カメラの更新処理
	camera.Update(&player->m_transform.pos);


	// Xキーを押された時にシーンの変更をする（今だけの仮）
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		m_scene_change_judge = true;
	}
	else
	{
		m_scene_change_judge = false;
	}

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

	

	//// 仮のフォントの描画
	//// 後で変更予定
	//// フォントのデフォルトサイズの保存
	//int default_font_size = GetFontSize();
	//// フォントサイズの設定
	//SetFontSize(80);
	//const char* name = "エストエリア :: RENTER";
	//// 描画幅の取得
	//float w = GetDrawStringWidth(name, -1);
	//// 文字列の高さの取得
	//float h = GetFontSize();
	//// 描画座標
	//Vector2 draw_pos = { SCREEN_W / 2 - w / 2, 0 };
	//DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));

	//// フォントのサイズをデフォルトサイズに戻す
	//SetFontSize(default_font_size);


	
}

//------------------------------------------
// 終了処理
//------------------------------------------
void QuestAreaScene::Exit()
{
	//　シャドーマップの削除
	ExitShadowMap();

}
