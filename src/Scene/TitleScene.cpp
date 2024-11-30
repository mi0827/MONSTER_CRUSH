#include "src/WinMain.h"
#include "src/System/Random.h"
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
#include "src/Character/Hero.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"


#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"

#include "src/Camera.h"

#include "src/System/Text.h"

#include "Base/Scene_Base.h"
#include "TitleScene.h"



//------------------------------------------
// 初期処理
//------------------------------------------
void TitleScene::Init()
{
	// ベースクラスで初期化しておきたいものの初期化
	BaseInit();
	// フィールドの初期化
	field.Init();


	// カメラの初期設定
	camera.PlayFieldInit();

	// プレイヤーの設定
	player = new Hero;

	// プリえやーの初期設定 
	player->Init();
	player->SetCharacterPos({ 0,0,750 });
	player->SetCharacterRot({ 0.0f,180.0f,0.0f });

	// カメラの向きの設定
	camera.SetCameraRot(player->m_transform.rot);

	// シャドーマップの設定
	ShadowMapInit();

	// AreaBOXの設定
	SetAeraBoxSet();

	// 現在のシーンの設定(タイトルシーン)
	m_now_scene = Title;



	// タイトルで使うテキストデータの読み込み
	m_text.LoadText("Data/Text/TitleStory.txt", STORY_NUM_MAX);
}

//------------------------------------------
// 更新処理
//------------------------------------------
void TitleScene::Update()
{

	switch (m_turn)
	{
	case Main:
		// カメラの向きを取得する
		m_camera_rot = camera.GetCameraRot();

		// プレイヤーの更新処理
		player->Update(&m_camera_rot);

		// フィールドとの当たり判定
		HitField();

		// カメラの更新処理
		camera.MouseCamera(&player->m_transform.pos);

		// Xキーを押された時にシーンの変更をする（今だけの仮）
		if (PushHitKey(KEY_INPUT_RETURN))
		{
			// 次に行ってほしいシーンに移動
			SetNextScene(QuestArea);
			// シーン変更フラグを立てる
			m_scene_change_judge = true;
		}


		// この当たり判定に入ったら
		for (int i = 0; i < Area_Max; i++)
		{
			if (CheckBoxHit3D(player->m_transform.pos, player->m_move_hit_size,
				m_area_box[i].m_box.hit_pos, m_area_box[i].m_box.half_size))
			{
				// 当たり判定がヒットしたエリアよりさかのぼってもストーリーをさかのぼらせないための処理
				// 新しくヒットエリアが前回ヒットしたエリア以上だったら
				if (i >= hit_area)
				{
					// ヒットしたエリアを保存
					hit_area = i;
					// 当たり判定
					if (i == next_scene)
					{
						// フェード嘔吐のターンに変更
						m_turn = FadeOut;
					}
					else
					{
						// 当たり判定があったエリアに応じて表示するテキストを変更する
						m_text_num = i;
					}
				}
			}
		}

		break;
	case FadeOut:
		// フェードアウトの処理
		FadeOutSceneChange(QuestArea);
		break;
	}

}

//------------------------------------------
// 描画処理
//------------------------------------------
void TitleScene::Draw()
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

	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{
		// シャドウマップへキャラクターモデルの描画
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


	
	//=============================================
	// 仮でタイトルを描画
	//=============================================

	// フォントサイズの設定
	SetFontSize(80);
	const char* name = "モンスタークラッシュ" /*:: RENTER*/;
	// 描画幅の取得
	float w = GetDrawStringWidth(name, -1);
	// 文字列の高さの取得
	float h = GetFontSize();
	// 描画座標
	Vector2 draw_pos = { SCREEN_W / 2 - w / 2, 0 };
	DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));


	/*name = "Push : W";
	w = GetDrawStringWidth(name, -1);*/
	//h = GetFontSize();
	draw_pos = { (SCREEN_W / 2 - m_text.TITLE_BACK_HALF_SIZE),(SCREEN_H - h - m_text.CREVICE_SIZE) };
	// テキストファイルからのストーリーの描画
	m_text.TextDraw(m_text_num, { draw_pos.x, draw_pos.y }, m_text.TITLE_BACK_SIZE);


	DrawFormatString(16, 300, GetColor(255, 255, 255), "now_anim : %d", player->m_now_attack_anim);
	DrawFormatString(16, 400, GetColor(255, 255, 255), "combo_count : %d", player->m_combo_count);
	DrawFormatString(16, 500, GetColor(255, 255, 255), "now_attack : %d", player->m_now_attack);
	DrawFormatString(16, 600, GetColor(255, 255, 255), "aaa: %d", player->aaa);
	DrawFormatString(16, 700, GetColor(255, 255, 255), "bbb: %d", player->bbb);

	// フェードの描画処理
	FadeDraw();
}

//------------------------------------------
// 終了処理
//------------------------------------------
void TitleScene::Exit()
{

	//　シャドーマップの削除
	ExitShadowMap();
}

//------------------------------------------
// フィールドとの当たり判定
//------------------------------------------
void TitleScene::HitField()
{

	// フィールドの地面モデルとキャラクターの当たり判定
	HitGroundCharacter(&player->m_transform.pos, &field.m_field_model);

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
// オプションメニューでもらってきた値を反映する関数
//------------------------------------------
void TitleScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// カメラの感度設定
	camera.SetCameraSensi(mouse);

}

//------------------------------------------
// エリアBOXの初期設定
//------------------------------------------
void TitleScene::SetAeraBoxSet()
{
	// サイズの設定
	m_box_size.set(300.0f, 80.0f, 100.0f);

	// BOXの作成
	for (int i = 0; i < Area_Max; i++)
	{
		m_area_box[i].CreateBox(m_box_pos[i], m_box_size);
	}

	//// 座標の設定
	//m_box_pos[next_scene].set(0.0f, 0.0f, 150);	
}
