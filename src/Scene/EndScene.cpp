#include "src/WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Animation/Animation.h"
#include "src/Effect/Effect.h"

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

#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"
#include "src/Camera.h"
#include "src/System/Text.h"
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
	// ベースクラスで初期化しておきたいものの初期化
	BaseInit();


	// フィールドの初期化
	m_field_2.Init();


	// カメラの初期設定
	camera.PlayFieldInit();

	PlayerInit(hero);
	// プレイヤーの初期設定 
	m_player->Init();

	// シャドーマップの設定
	ShadowMapInit();

	// 現在のシーンの設定(エンドシーン)
	m_now_scene = End;


	// エンドシーンで使う用のテキストの読み込み
	m_text.LoadText("Data/Text/End.txt", text_max);



	// SEの初期化
	m_se.NewArraySecureSound(se_max);
	m_se.LoadSound("Data/SE/menu_selection.mp3", se_1); // 各メニューを選択しているとき



}


//------------------------------------------
// 更新処理
//------------------------------------------
void EndScene::Update()
{
	// マウスの表示状態の設定
	SetMouseDispFlag(TRUE);
	// マウスの固定を解除する
	ChangeMousePosMove(TRUE);
	// プレイヤーの少し上にカメラを配置するための変数
	Vector3 target_pos;

	// マウスの座標を取得
	Vector2 mouse_pos = { float(GetMouseX()),float(GetMouseY()) };

	// どのメニューと当たっているかを調べる
	Vector2 pos2;

	switch (m_turn)
	{
	case Main:
		// 経過フレームを進める
		m_frame_count++;
		// 指定のフレームに達したら
		if (m_frame_count >= END_SCENE_FRAME_MAX)
		{
			// フレームリセット
			m_frame_count = 0;
			// タイトルに移動するようにする
			m_change_scene = Title;
			m_turn = FadeOut;
			m_start_flag = false;
		}


		// カメラの向きを取得する
		m_camera_rot = camera.GetCameraRot();
		// プレイヤーの更新処理
		m_player->Update(&m_camera_rot);
		// カメラの更新処理
		target_pos = { m_player->m_transform.pos.x,m_player->m_transform.pos.y + 20,m_player->m_transform.pos.z };
		camera.MoveCamera(&target_pos, 10, true);


		// ゲームパッドのでの選択
		if (IsPadOn(PAD_ID::PAD_D_LEFT))
		{
			m_select_num = text1;
			// あったているフラグを立てる
			m_hit_select_flag = true;
		}
		else if (IsPadOn(PAD_ID::PAD_D_RIGHT))
		{
			m_select_num = text2;
			// あったているフラグを立てる
			m_hit_select_flag = true;
		}

		// どこに戻るかを選択できるようにしている
		// ゲームパッドの接続がなければ
		if (GetJoypadNum() <= 0)
		{
			for (int i = 0; i < text_max; i++)
			{
				pos2 = { m_text_draw_pos[i].x + m_text.END_BACK_SIZE,  m_text_draw_pos[i].y + GetFontSize() };
				// 当たっていたもの選択状態にする
				if (CheckPointBoxHit(mouse_pos, m_text_draw_pos[i], pos2))
				{
					// 当たり判定のあったほうを選択状態する
					m_select_num = i;
					// あったているフラグを立てる
					m_hit_select_flag = true;
					break;
				}
				else
				{
					m_hit_select_flag = false;
				}
			}
		}
		


		// マウスが選択マスにあったているとき
		if (m_hit_select_flag)
		{
			if (CheckMouseInput(MOUSE_INPUT_LEFT) || IsPadOn(PAD_ID::PAD_X))
			{
				// SEの再生
				SoundPlay(se_1);
				// 選択完了フラグを上げる
				m_start_flag = true;
			}
		}
		// 選択したら
		if (m_start_flag)
		{
			switch (m_select_num)
			{
			case text1: // タイトルに戻る
				m_change_scene = Title;
				m_turn = FadeOut;
				m_start_flag = false;
				break;
			case text2: // クエスト選択画面にい戻る
				m_change_scene = QuestArea;
				m_turn = FadeOut;
				m_start_flag = false;
				break;
			}
		}
		break;
	case FadeOut:

		// フェードアウトの処理
		FadeOutSceneChange(m_change_scene);
		break;
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
		m_field_2.Draw(camera.m_pos,m_player->m_transform.pos);
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

	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(1, m_shadowMap_handle_1);
	{

		// プレイヤーの描画処理
		m_player->Draw();

	}
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// プレイヤーの描画処理
		m_player->Draw();
		// シャドウマップへキャラクターモデルの描画
		m_field_2.Draw(camera.m_pos, m_player->m_transform.pos);

	}
	UseShadowMapSet();


	// フォントサイズの設定
	SetFontSize(60);

	// 文字列の高さの取得
	float h = GetFontSize();
	// 描画座標
	Vector2 box_pos;
	// どちらを選択しているかの外枠
	box_pos.set(m_text_draw_pos[m_select_num].x - 30, m_text_draw_pos[m_select_num].y - 30);
	if (m_hit_select_flag)
	{
		DrawBox(box_pos.x, box_pos.y, box_pos.x + m_text.END_BACK_SIZE + 70, box_pos.y + h + 70, GetColor(255, 255, 0), TRUE);
	}

	// テキストの描画
	for (int i = 0; i < text_max; i++)
	{
		m_text.TextDraw(i, m_text_draw_pos[i], m_text.END_BACK_SIZE);
	}

	// フェードの描画
	FadeDraw();

}

//------------------------------------------
// 終了処理
//------------------------------------------
void EndScene::Exit()
{

	//　シャドーマップの削除
	ExitShadowMap();

	m_player->Exit();
	// プレイヤークラスはポインタなので最後に開放する
	//delete m_player;

	// マウスの表示状態の設定
	SetMouseDispFlag(FALSE);
	// マウスの固定する
	ChangeMousePosMove(FALSE);
}

//------------------------------------------
// フィールドとキャラクターの当たり判定
//------------------------------------------
void EndScene::HitField()
{
	// フィールドの地面モデルとキャラクターの当たり判定
	HitGroundCharacter(&m_player->m_transform.pos, &m_field_2.m_field_model);

	// 木のオブジェクトとプレイヤーの当たり判定
	//for (int i = 0; i < m_field_2.TREE_MAX; i++)
	//{
	//	// モンスターとプレイヤーの移動の当たり判定
	//	if (CheckCapsuleHit(m_field_2.m_hit_tree[i], m_player->m_body))
	//	{
	//		m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &m_field_2.m_hit_tree[i]);
	//	}
	//}

	//// フェンスとキャラクターの当たり判定
	//for (int i = 0; i < m_field_2.FENCE_MAX; i++)
	//{
	//	if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
	//		m_field_2.m_hit_fence[i].m_box.hit_pos, m_field_2.m_hit_fence[i].m_box.half_size))
	//	{
	//		m_player->MoveHitUpdate(&m_field_2.m_hit_fence[i]);
	//	}
	//}


	//// 石とキャラクターの当たり判定
	//for (int i = 0; i < m_field_2.STONE_MAX; i++)
	//{
	//	if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
	//		m_field_2.m_hit_stone[i].m_box.hit_pos, m_field_2.m_hit_stone[i].m_box.half_size))
	//	{
	//		m_player->MoveHitUpdate(&m_field_2.m_hit_stone[i]);
	//	}
	//}
}

//------------------------------------------
// オプションメニューでもらってきた値を反映する関数
//------------------------------------------
void EndScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// カメラの感度設定
	camera.SetCameraSensi(mouse);
	// キャラクターのサウンドの調整
	m_player->m_se.SetSoundVolume(se);
}

//----------------------------------------------
// サウンドの再生
//----------------------------------------------
void EndScene::SoundPlay(int se_num)
{
	// メニュー画面ではボタン連打があるため
	// 再生中にからならないために再生中のものを消してい空付きのを鳴らす
	// SEが再生中でなければ
	if (m_se.PlayingSound())
	{
		// SE再生可能にしておく
		m_se.m_playing_flag = true;
		// 現在再生中のSEを止める
		m_se.StopSound();
	}
	// SEの再生
	m_se.PlaySound_(se_num, DX_PLAYTYPE_BACK, true);
	m_se.m_playing_flag = false;
}

