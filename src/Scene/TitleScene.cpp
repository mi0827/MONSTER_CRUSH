#include "src/WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
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
#include "TitleScene.h"



//------------------------------------------
// 初期処理
//------------------------------------------
void TitleScene::Init()
{
	// ベースクラスで初期化しておきたいものの初期化
	BaseInit();
	// フィールドの初期化
	m_field_1.Init();

	// カメラの初期設定
	camera.PlayFieldInit();

	//PlayerInit(hero);
	//// プレイヤーの初期設定 
	//m_player->Init();
	//m_player->SetCharacterPos({ 0,0,750 });
	//m_player->SetCharacterRot({ 0.0f,180.0f,0.0f });

	// カメラの向きの設定
	//camera.SetCameraRot(m_player->m_transform.rot);

	// シャドーマップの設定
	//ShadowMapInit();

	m_background_image = LoadGraph("Data/Title/Title.jpg");
	// 現在のシーンの設定(タイトルシーン)
	m_now_scene = Title;
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
		//m_camera_rot = camera.GetCameraRot();

		// プレイヤーの更新処理
		//m_player->Update(&m_camera_rot);

		// フィールドとの当たり判定
		//HitField();

		// カメラの更新処理
		//camera.MouseCamera(&m_player->m_transform.pos);

		// タイトルシーンの更新処理
		TitleUpdate();



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

	//// プレイヤーのシャドーマップのエリアのセット
	//SetShadowMapArea(m_shadowMap_handle_1, m_player->m_transform.pos);
	////-------------------------------------------------------------
 //   // シャドウマップの作成（ここで各オブジェクトのシャドーマップの設定）
 //   //-------------------------------------------------------------
 //   // シャドウマップへの描画の準備
	//ShadowMap_DrawSetup(m_shadowMap_handle_1);
	//{
	//	// プレイヤーの描画処理
	//	m_player->Draw();
	//}
	//ShadowMap_DrawSetup(m_shadowMap_handle);
	//{
	//	// シャドウマップへキャラクターモデルの描画
	//	m_field_1.Draw();
	//}
	//// シャドウマップへの描画を終了
	//ShadowMap_DrawEnd();
	////-------------------------------------------------------------
	//// 各モデルの描画
	////-------------------------------------------------------------
	//// バックバッファに描画する
	//SetDrawScreen(DX_SCREEN_BACK);
	//
	//// カメラの描画処理
	//camera.Draw();
	//
	//// 空の描画
	//SkyDraw();
	//
	//SetUseShadowMap(1, m_shadowMap_handle_1);
	//{
	//	// プレイヤーの描画処理
	//	m_player->Draw();
	//}
	//// 描画に使用するシャドウマップを設定
	//SetUseShadowMap(0, m_shadowMap_handle);
	//{
	//	// プレイヤーの描画処理
	//	m_player->Draw();
	//	// シャドウマップへキャラクターモデルの描画
	//	m_field_1.Draw();
	//}
	//
	//
	//UseShadowMapSet();



	//=============================================
	// 仮でタイトルを描画
	//=============================================

	DrawExtendGraphF(0, 0, SCREEN_W, SCREEN_H, m_background_image, TRUE);

	// フォントサイズの設定
	//SetFontSize(80);
	//const char* name = "モンスタークラッシュ" /*:: RENTER*/;
	//// 描画幅の取得
	//float w = GetDrawStringWidth(name, -1);
	//// 文字列の高さの取得
	//float h = GetFontSize();
	// 描画座標
	/*Vector2 draw_pos = { SCREEN_W / 2 - w / 2, 0 };
	DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));*/
	/*name = "Push : W";
	w = GetDrawStringWidth(name, -1);*/
	//h = GetFontSize();
	//draw_pos = { (SCREEN_W / 2 - m_text.TITLE_BACK_HALF_SIZE),(SCREEN_H - h - m_text.CREVICE_SIZE) };
	//// テキストファイルからのストーリーの描画
	//m_text.TextDraw(m_text_num, { draw_pos.x, draw_pos.y }, m_text.TITLE_BACK_SIZE);
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

	//m_player->Exit();
}

//------------------------------------------
// フィールドとの当たり判定
//------------------------------------------
void TitleScene::HitField()
{

}

//------------------------------------------
// オプションメニューでもらってきた値を反映する関数
//------------------------------------------
void TitleScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// カメラの感度設定
	camera.SetCameraSensi(mouse);

	// キャラクターのサウンドの調整
	//m_player->m_se.SetSoundVolume(se);

	// BGMのサウンドの調整
}

//------------------------------------------
// タイトルの更新処理
//------------------------------------------
void TitleScene::TitleUpdate()
{
	switch (m_title_mode_num)
	{
	case TITLE:

		// タイトル画面でのカウントを減らす
		m_count_flame++;
		// フレームが一定以下になったら
		if ((m_count_flame % FLAME_MAX == 0) && m_count_flame != 0)
		{
			// 時間を増やす
			m_count_time++;
		}
		// 時間が一定以上経過したら
		if (m_count_time >= TITLE_TIME_MAX)
		{
			// カウントをリセット
			m_count_time = 0;
			m_count_flame = 0;
			// 動画モードに移行する
			m_title_mode_num = MOVIE;
		}
		// BGMが終わったら
		//if (!title_bgm.Playing_BGM(TITLE_BGM))
		//{
		//	// BGMの再生
		//	title_bgm.Play_BGM(DX_PLAYTYPE_LOOP, true, TITLE_BGM);
		//}
		// Xキーを押された時にシーンの変更をする（今だけの仮）
		if (PushHitKey(KEY_INPUT_RETURN))
		{
			// 次に行ってほしいシーンに移動
			SetNextScene(Story);
			// シーン変更フラグを立てる
			m_scene_change_judge = true;
		}
		break;

	case MOVIE:

		// プレイ動画の再生
		m_movie = PlayMovie("Data/Movie/02.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);
		//GetMovieStateToGraph(m_movie);
		// 指定の動画の再生状態を取得
		if (GetMovieStateToGraph(m_movie))
		{
			// 画面の状態をタイトル状態にする
			m_title_mode_num = TITLE;
			// 再生中の動画を停止する
			PauseMovieToGraph(m_movie);
		}
	

	
		break;
	}
}

