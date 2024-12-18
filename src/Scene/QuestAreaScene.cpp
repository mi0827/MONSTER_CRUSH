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
#include "src/Character/Receptionist.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"


#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"

#include "src/Camera.h"

#include "src/System/Text.h"

#include "Base/Scene_Base.h"
#include "QuestAreaScene.h"


//------------------------------------------
// コンストラクタ
//------------------------------------------
QuestAreaScene::QuestAreaScene()
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
QuestAreaScene::~QuestAreaScene()
{

	delete[] m_text;
}

//------------------------------------------
// 初期処理
//------------------------------------------
void QuestAreaScene::Init()
{
	// ベースクラスで初期化しておきたいものの初期化
	BaseInit();

	// フィールドの初期化
	field.Init();

	// カメラの初期設定
	camera.PlayFieldInit();

	// プレイヤーの設定
	player = new Hero;

	// プレイヤーの初期設定 
	player->Init();
	// プレイヤーの座標の設定
	player->SetCharacterPos({ 0.0f, 0.0f,250.0f });
	// プレイヤーの向きの設定
	player->SetCharacterRot({ 0.0f,-180.0f,0.0f });

	// カメラの向きの設定
	camera.SetCameraRot(player->m_transform.rot);

	// 受付嬢の初期設定
	receptionist.Init();
	// 受付嬢と話せるエリアの設定
	InitArea();
	// シャドーマップの設定
	ShadowMapInit();
	// 現在のシーンの設定(クエスト受注シーン)
	m_now_scene = QuestArea;

	// テキストの読み込み
	m_quest_text.LoadText("Data/Text/Quest.txt", quest_max);
	m_quest_area_text.LoadText("Data/Text/QuestAreaStory.txt", story_max);

}


//------------------------------------------
// 更新処理
//------------------------------------------
void QuestAreaScene::Update()
{
	switch (m_turn)
	{
		/*case FadeIn:
			FadeInUpdate();
			break;*/
	case Main:
		// カメラの向きを取得する
		m_camera_rot = camera.GetCameraRot();

		// プレイヤーの更新処理
		player->Update(&m_camera_rot);

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
				//// 次に行ってほしいシーンに移動(バトルシーン)
				//SetNextScene(Battle);
				//m_scene_change_judge = true;
				// フェード嘔吐のターンに変更
				m_turn = FadeOut;
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
		camera.MouseCamera(&player->m_transform.pos);

		// 文字列の描画のための設定
		for (int i = 0; i < text_max; i++)
		{
			// 座標変換
			VECTOR pos = DrawStringWrold(receptionist.m_transform.pos, m_text[i].shift_pos);
			// 描画座用に変換
			m_text[i].draw_pos.VSet(pos);

			// 変換したスクリーン座標のZの値が0.0 ~ 1.0 なら描画していい
			if (i == f_text)
			{
				// 描画したい文字列がF : 話すの場合
				// 話していいエリアに入っていないと描画できないようにする
				if (m_area_hit)
				{
					// カメラの画角内稼働かによって描画しない
					// 画角内
					if (pos.z > 0.0f && pos.z < 1.0f)
					{
						// 描画する
						m_text[i].draw_flag = true;
					}
					else // 画面がい
					{
						// 描画しない
						m_text[i].draw_flag = false;
					}
				}
				else // 範囲がいでも描画できない
				{
					m_text[i].draw_flag = false;
				}

			}
			else // 指定のテキスト以外は
			{
				// カメラの画角内稼働かによって描画しない
				if (pos.z > 0.0f && pos.z < 1.0f)
				{
					// 描画する
					m_text[i].draw_flag = true;
				}
				else
				{
					// 描画しない
					m_text[i].draw_flag = false;
				}
			}
		}
		{

		}
		break;
	case FadeOut:
		// フェードアウトの処理
		FadeOutSceneChange(Battle);
		break;
	}








}

//------------------------------------------
// 描画処理
//------------------------------------------
void QuestAreaScene::Draw()
{
	// シャドーマップのエリアのセット
	SetShadowMapArea(m_shadowMap_handle_1, player->m_transform.pos);
	SetShadowMapArea(m_shadowMap_handle_2, receptionist.m_transform.pos);
	//-------------------------------------------------------------
		// シャドウマップの作成（ここで各オブジェクトのシャドーマップの設定）
		//-------------------------------------------------------------
		// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMap_handle_1);
	{
		// プレイヤーの描画処理
		player->Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle_2);
	{
		receptionist.Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{
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
	SetUseShadowMap(1, m_shadowMap_handle_1);
	{
		player->Draw();
		// 受付嬢の描画
		receptionist.Draw();
	}
	SetUseShadowMap(2, m_shadowMap_handle_2);
	{
		receptionist.Draw();
	}
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// プレイヤーの描画処理
		player->Draw();

		// 受付嬢の描画
		receptionist.Draw();
		// シャドウマップへキャラクターモデルの描画
		field.Draw();

	}
	UseShadowMapSet();


	// エリアを描画（仮後で消す）
	// m_area.Draw();


	

	//int a = CreateFontToHandle("MS:明朝",40,10,10);
		// フォントサイズの設定
	SetFontSize(FONT_SIZE);

	// 文字列の高さの取得
	float h = GetFontSize();


	for (int i = 0; i < text_max; i++)
	{
		
		if (m_text[i].draw_flag)
		{
			// 描画幅の取得
			float w = GetDrawStringWidth(m_text[i].text, -1);

			// 描画座標
			Vector2 draw_pos = { m_text[i].draw_pos.x - w / 2,  m_text[i].draw_pos.y - h };
			// 文字列の描画
			DrawString(draw_pos.x, draw_pos.y, m_text[i].text, GetColor(255, 128, 50));

		}

	}


	// テキストを描画
	SetFontSize(40);
	h = GetFontSize();
	Vector2 text_draw_pos;
	text_draw_pos.set((SCREEN_W / 2 - m_quest_area_text.TITLE_BACK_HALF_SIZE), (SCREEN_H - h * story_max - m_quest_area_text.CREVICE_SIZE));
	for (int i = 0; i < story_max; i++)
	{
		m_quest_area_text.TextDraw(i, { text_draw_pos.x, (text_draw_pos.y + h * i) }, m_quest_area_text.TITLE_BACK_SIZE);
	} 

	

	// フェードの描画処理
	FadeDraw();
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
// オプションメニューでもらってきた値を反映する関数
//------------------------------------------
void QuestAreaScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// カメラの感度設定
	camera.SetCameraSensi(mouse);
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
