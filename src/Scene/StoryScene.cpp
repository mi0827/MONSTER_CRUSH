#include "src/WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"
#include "src/Field/Field.h"

#include "src/Camera.h"
#include "src/System/Text.h"
#include "Base/Scene_Base.h"
#include "StoryScene.h"



//------------------------------------------
// 初期処理
//------------------------------------------
void StoryScene::Init()
{
	// ベースクラスで初期化しておきたいものの初期化
	BaseInit();
	

	// フィールドの初期化
	m_field_1.Init();

	// カメラの初期設定
	camera.PlayFieldInit();

	PlayerInit(hero);
	// プレイヤーの初期設定 
	m_player->Init();
	m_player->SetCharacterPos({ 0,0,750 });
	m_player->SetCharacterRot({ 0.0f,180.0f,0.0f });

	// カメラの向きの設定
	camera.SetCameraRot(m_player->m_transform.rot);

	// シャドーマップの設定
	ShadowMapInit();

	// AreaBOXの設定
	SetAeraBoxSet();

	// 現在のシーンの設定(タイトルシーン)
	m_now_scene = Story;

	// タイトルで使うテキストデータの読み込み
	m_text.LoadText("Data/Text/TitleStory.txt", STORY_NUM_MAX);
}

//------------------------------------------
// 更新処理
//------------------------------------------
void StoryScene::Update()
{

	switch (m_turn)
	{
	case Main:
		// カメラの向きを取得する
		m_camera_rot = camera.GetCameraRot();

		// プレイヤーの更新処理
		m_player->Update(&m_camera_rot);

		// フィールドとの当たり判定
		HitField();

		// カメラの更新処理
		camera.MouseCamera(&m_player->m_transform.pos);

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
			if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
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
void StoryScene::Draw()
{


	// プレイヤーのシャドーマップのエリアのセット
	SetShadowMapArea(m_shadowMap_handle_1, m_player->m_transform.pos);

	//-------------------------------------------------------------
	// シャドウマップの作成（ここで各オブジェクトのシャドーマップの設定）
	//-------------------------------------------------------------
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMap_handle_1);
	{
		// プレイヤーの描画処理
		m_player->Draw();

	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{
		// シャドウマップへキャラクターモデルの描画
		m_field_1.Draw(camera.m_pos, camera.m_length, m_player->m_transform.pos);
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

	// 空の描画
	SkyDraw();

	SetUseShadowMap(1, m_shadowMap_handle_1);
	{

		// プレイヤーの描画処理
		m_player->Draw();

	}
	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// プレイヤーの描画処理
		m_player->Draw();
		// シャドウマップへキャラクターモデルの描画
		m_field_1.Draw(camera.m_pos, camera.m_length, m_player->m_transform.pos);

	}


	UseShadowMapSet();


	// フォントサイズの設定
	SetFontSize(80);
	// 文字列の高さの取得
	float h = GetFontSize();

	Vector2 draw_pos = { (SCREEN_W / 2 - m_text.TITLE_BACK_HALF_SIZE),(SCREEN_H - h - m_text.CREVICE_SIZE) };
	// テキストファイルからのストーリーの描画
	m_text.TextDraw(m_text_num, { draw_pos.x, draw_pos.y }, m_text.TITLE_BACK_SIZE);

	// フェードの描画処理
	FadeDraw();
}

//------------------------------------------
// 終了処理
//------------------------------------------
void StoryScene::Exit()
{
	
	//　シャドーマップの削除
	ExitShadowMap();

	m_player->Exit();
}

//------------------------------------------
// フィールドとの当たり判定
//------------------------------------------
void StoryScene::HitField()
{

	// フィールドの地面モデルとキャラクターの当たり判定
	HitGroundCharacter(&m_player->m_transform.pos, &m_field_1.m_field_model);

	// 木のオブジェクトとプレイヤーの当たり判定
	// なぜか２本だけ当たり判定がどうしない
	for (int i = 0; i < m_field_1.TREE_MAX; i++)
	{
		// モンスターとプレイヤーの移動の当たり判定
		if (CheckCapsuleHit(m_field_1.m_hit_tree[i], m_player->m_body))
		{
			m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &m_field_1.m_hit_tree[i]);
		}
	}

	// プレイヤーとフィールドを囲ってる四角との当たり判定
	for (int i = 0; i < m_field_1.AROUND_MAX; i++)
	{
		if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
			m_field_1.m_hit_around[i].m_box.hit_pos, m_field_1.m_hit_around[i].m_box.half_size))
		{
			m_player->MoveHitUpdate(&m_field_1.m_hit_around[i]);
		}
	}
}

//------------------------------------------
// オプションメニューでもらってきた値を反映する関数
//------------------------------------------
void StoryScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// カメラの感度設定
	camera.SetCameraSensi(mouse);

	// キャラクターのサウンドの調整
	m_player->m_se.SetSoundVolume(se);

	
}

//------------------------------------------
// エリアBOXの初期設定
//------------------------------------------
void StoryScene::SetAeraBoxSet()
{
	// サイズの設定
	m_box_size.set(300.0f, 80.0f, 100.0f);

	// BOXの作成
	for (int i = 0; i < Area_Max; i++)
	{
		m_area_box[i].CreateBox(m_box_pos[i], m_box_size);
	}
}
