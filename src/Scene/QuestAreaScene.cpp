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
#include "src/Hit/HitStop.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"
#include "src/Character/Receptionist.h"
#include "src/Character/Bot.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"
#include "src/Field/Field.h"

#include "src/System/DamageAction.h"
#include "src/Action/Attack.h"

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

	delete[] m_landmark_text;
}

//------------------------------------------
// 初期処理
//------------------------------------------
void QuestAreaScene::Init()
{
	m_select_num = 0;

	// ベースクラスで初期化しておきたいものの初期化
	BaseInit();

	// フィールドの初期化
	m_field_1.Init();

	// カメラの初期設定
	camera.PlayFieldInit();

	PlayerInit(hero);
	// プレイヤーの初期設定 
	m_player->Init();
	// プレイヤーの座標の設定
	m_player->SetCharacterPos({ 0.0f, 0.0f,250.0f });
	// プレイヤーの向きの設定
	m_player->SetCharacterRot({ 0.0f,-180.0f,0.0f });

	// カメラの向きの設定
	camera.SetCameraRot(m_player->m_transform.rot);

	// 受付嬢の初期設定
	receptionist.Init();
	// 受付嬢と話せるエリアの設定
	InitArea();

	// ボットの初期化
	m_bot.Init();

	// シャドーマップの設定
	ShadowMapInit();
	// 現在のシーンの設定(クエスト受注シーン)
	m_now_scene = QuestArea;

	// テキストの読み込み
	m_quest_text.LoadText("Data/Text/Quest.txt", quest_max);
	m_quest_area_text.LoadText("Data/Text/QuestAreaStory.txt", palyer_story_max);
	m_reception_text.LoadText("Data/Text/QuestReception.txt", recept_teception_max);
}


//------------------------------------------
// 更新処理
//------------------------------------------
void QuestAreaScene::Update()
{
	switch (m_turn)
	{
	case Main:
		// カメラの更新処理
		camera.MouseCamera(&m_player->m_transform.pos);
		// メインで行う処理をこのシーンの状態に合わせて処理を分け実行する関数
		QuestAreaUpdate();


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
	SetShadowMapArea(m_shadowMap_handle_1, m_player->m_transform.pos);
	SetShadowMapArea(m_shadowMap_handle_2, receptionist.m_transform.pos);
	//-------------------------------------------------------------
		// シャドウマップの作成（ここで各オブジェクトのシャドーマップの設定）
		//-------------------------------------------------------------
		// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(m_shadowMap_handle_1);
	{
		// プレイヤーの描画処理
		m_player->Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle_2);
	{
		receptionist.Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{
		// フィールドの描画
		m_field_1.Draw(camera.m_pos, m_player->m_transform.pos);
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
		m_player->Draw();
		// 受付嬢の描画
		receptionist.Draw();
		// ボット
		m_bot.Draw();
	}
	SetUseShadowMap(2, m_shadowMap_handle_2);
	{
		receptionist.Draw();
	}
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// プレイヤーの描画処理
		m_player->Draw();

		// 受付嬢の描画
		receptionist.Draw();
		// シャドウマップへキャラクターモデルの描画
		m_field_1.Draw(camera.m_pos, m_player->m_transform.pos);
	}
	UseShadowMapSet();


	// 会話内容に応じて処理を変更
	switch (scene_mode_num)
	{
	case normal:// 会話していない状態
		ModeNormalDraw();
		break;

	case talk_start:
		break;
	case receptionist_talk: // 会話中
		TalkDraw();
		break;
	case accepting_quest: // クエストを受けている状態
		AcceptingQuestDraw();
		break;
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

	// プレイヤーの終了処理
	m_player->Exit();
}

//------------------------------------------
// フィールドとキャラクターの当たり判定
//------------------------------------------
void QuestAreaScene::HitField()
{

	// フィールドの地面モデルとプレイヤーの当たり判定
	HitGroundCharacter(&m_player->m_transform.pos, &m_field_1.m_field_model);
	// 受付嬢
	HitGroundCharacter(&receptionist.m_transform.pos, &m_field_1.m_field_model);


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
void QuestAreaScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// カメラの感度設定
	camera.SetCameraSensi(mouse);
	// キャラクターのサウンドの調整
	m_player->m_se.SetSoundVolume(se);

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

//------------------------------------------
// 話せるようになるエリアの設定
//------------------------------------------
void QuestAreaScene::LandMarkUpdate()
{
	// カメラの向きを取得する
	m_camera_rot = camera.GetCameraRot();
	

	// 文字列の描画のための設定
	for (int i = 0; i < landmark_text_max; i++)
	{
		// 座標変換
		VECTOR pos = DrawStringWorld(receptionist.m_transform.pos, m_landmark_text[i].shift_pos);
		// 描画座用に変換
		m_landmark_text[i].draw_pos.VSet(pos);

		// 変換したスクリーン座標のZの値が0.0 ~ 1.0 なら描画していい
		if (i == f_text || i == x_text)
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
					m_landmark_text[i].draw_flag = true;
				}
				else // 画面がい
				{
					// 描画しない
					m_landmark_text[i].draw_flag = false;
				}
			}
			else // 範囲がいでも描画できない
			{
				m_landmark_text[i].draw_flag = false;
			}
		}
		else // 指定のテキスト以外は
		{
			// カメラの画角内稼働かによって描画しない
			if (pos.z > 0.0f && pos.z < 1.0f)
			{
				// 描画する
				m_landmark_text[i].draw_flag = true;
			}
			else
			{
				// 描画しない
				m_landmark_text[i].draw_flag = false;
			}
		}
	}
}

//------------------------------------------
// このシーンの状態に合わせて適切な処理をさせる関数
//------------------------------------------
void QuestAreaScene::QuestAreaUpdate()
{
	// ゲームが進んでいる間フレームをカウントし続ける
	// ストーリーを進める際のスペースキーの文字をちかちかさせるためのもの
	if (m_blend_flag)
	{
		m_frame_count++;
		if (m_frame_count >= 10)
		{
			m_frame_count = 0;
			m_blend_flag = false;
		}
	}
	else
	{
		if (m_blend_change_flag)
		{
			m_text_blend_value += 10;
			if (m_text_blend_value >= TEXT_BLEND_MAX)
			{
				m_blend_flag = true;
				m_blend_change_flag = false;
			}
		}
		else
		{
			m_text_blend_value -= 10;
			if (m_text_blend_value <= TEXT_BLEND_MIN)
			{
				m_blend_flag = true;
				m_blend_change_flag = true;
			}
		}
	}




	switch (scene_mode_num)
	{
	case normal:// 会話していない状態
		ModeNormalUpdate();
		break;
	case talk_start:
		TalkStart();
		break;

	case receptionist_talk: // 会話中
		TalkUpdate();

		break;
	case accepting_quest: // クエストを受けている状態
		AcceptingQuestUpdate();
		break;
	}
}

//------------------------------------------
// 会話していない状態の更新処理
//------------------------------------------
void QuestAreaScene::ModeNormalUpdate()
{

	// カメラの向きを取得する
	m_camera_rot = camera.GetCameraRot();


	// ヒットストップのタイミング以外
	if (m_hit_stop.CheckHitStop() == false)
	{
		// プレイヤーの更新処理
		m_player->Update(&m_camera_rot);
	}
	// 受付嬢のの更新処理
	receptionist.Update();

	// 受付嬢とプレイヤーの移動の当たり判定
	if (CheckCapsuleHit(receptionist.m_hit_body, m_player->m_body))
	{
		m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &receptionist.m_hit_body);
	}

	// ボットの更新処理
	BotUpdate();

	// フィールドとキャラクターの当たり判定
	HitField();
	// 目印を写すかの処理
	LandMarkUpdate();

	// プレイヤーが受付嬢と話せる範囲に入ったかの確認
	if (CheckCapsuleHit(m_area, m_player->m_body))
	{
		// Xキーを押された時にシーンの変更をする（今だけの仮）
		if (PushHitKey(KEY_INPUT_F) || IsPadOn(PAD_ID::PAD_X))
		{
			// このシーンの状態を会話パートに移動する
			scene_mode_num = talk_start;
		}
		// 話せるエリアに入ったからフラグを上げる
		m_area_hit = true;
	}
	else
	{
		// 範囲外にいるときはフラグを下げる
		m_area_hit = false;
	}
}

//------------------------------------------
// 会話はじめの処理
//------------------------------------------
void QuestAreaScene::TalkStart()
{
	// クエスト出発確認でいいえを選択し再度話しかけられている場合	
	if (m_quest_acception_num == quest_confirmation)
	{
		// 先ほどの会話の続きからになるように受付嬢のテキストの行を設定する
		m_reception_text_line = 3;
	}

	// シーンの状態を会話中に移動
	scene_mode_num = receptionist_talk;

}

//------------------------------------------
// 会話パートの更新処理
//------------------------------------------
void QuestAreaScene::TalkUpdate()
{
	// マウスの右クリックかスペースキーで会話を進める
	if (PushMouseInput(MOUSE_INPUT_LEFT) || PushHitKey(KEY_INPUT_SPACE) || IsPadOn(PAD_ID::PAD_X))
	{
		// 描画するテキストを一つ進める
		m_reception_text_line++;
	}

	// 受付嬢の会話がどこまで進んでいるのかで内容を変更するためのもの
	switch (m_quest_acception_num)
	{
	case quest_before_accepting:

		// テキストが一行進んだら
		if (m_reception_text_line >= 1)
		{
			// 次の会話モードに移動させておく
			m_quest_acception_num = quest_confirmation;
			// シーンの状態をクエスト選択状態に移行する
			scene_mode_num = accepting_quest;
			// タイミングを
			m_reception_text_line = 3;
		}
		break;
	case quest_confirmation:
		// 選択しの変更
		if (PushHitKey(KEY_INPUT_W) || IsPadOn(PAD_ID::PAD_D_UP))
		{
			m_select_num--;
			if (m_select_num < 0)
			{
				m_select_num = 0;
			}

		}
		if (PushHitKey(KEY_INPUT_S) || IsPadOn(PAD_ID::PAD_D_DOWN))
		{
			m_select_num++;
			if (m_select_num > 1)
			{
				m_select_num = 1;
			}
		}

		if (m_reception_text_line >= 4)
		{
			// 次の会話モードに移動させておく
			m_quest_acception_num = quest_after_accepting;
			//m_reception_text_line = 3;
		}
		break;
	case quest_after_accepting:
		// はいが選ばれた時の処理
		if (m_select_num == 0)
		{
			// フェードアウト状態にする
			m_turn = FadeOut;
			// テキスト番号も初期化しておく
			m_reception_text_line = 0;
			break;
		}
		else // いいえが選ばれた時の処理を分ける
		{
			if (m_reception_text_line >= 6)
			{
				// マウスの右クリックかスペースキーで会話を進める
				if (PushMouseInput(MOUSE_INPUT_LEFT) || PushHitKey(KEY_INPUT_SPACE) || IsPadOn(PAD_ID::PAD_X))
				{
					// 会話シーンに戻す
					m_quest_acception_num = quest_confirmation;
					// 会話を一つ巻き戻しておく
					// プレイヤーとの会話をいったん終わる
					scene_mode_num = normal;
				}
			}
		}
		break;
	}
}

//------------------------------------------
// クエストを受けているときの更新処理
//------------------------------------------
void QuestAreaScene::AcceptingQuestUpdate()
{
	// 選択しの変更
	if (PushHitKey(KEY_INPUT_W) || IsPadOn(PAD_ID::PAD_D_UP))
	{
		m_select_num--;
		if (m_select_num < 0)
		{
			m_select_num = 0;
		}
	}
	if (PushHitKey(KEY_INPUT_S) || IsPadOn(PAD_ID::PAD_D_DOWN))
	{
		m_select_num++;
		if (m_select_num > 1)
		{
			m_select_num = 1;
		}
	}

	// クエスト選択画面の状態別の処理
	switch (m_quest_selection_num)
	{
	case quest_selection:
		m_quest_num = m_select_num;
		break;
	case reply_selection:
		m_reply_num = m_select_num;
		break;
	}

	// 次に進む
	if (PushMouseInput(MOUSE_INPUT_LEFT) || PushHitKey(KEY_INPUT_SPACE) || IsPadOn(PAD_ID::PAD_X))
	{
		// ここだけ行を二列ずつ進める
		m_quest_text_line += 3;

		// 返答状態
		if (m_quest_selection_num == reply_selection)
		{
			// 返答がいいえだった時
			if (m_reply_num == 1)
			{
				// 返答がいいえならクエスト選択に戻す
				m_quest_text_line = 0;
			}
			else
			{
				// はいだった時
				// このシーンの状態を会話パートの状態しておく
				scene_mode_num = receptionist_talk;
				// 受付嬢の会話の途中からやり直す
				m_quest_text_line = 2;

			}
		}
		if (scene_mode_num == accepting_quest)
		{
			if (m_quest_text_line <= 3)
			{
				// 選択が画面を次に進める
				m_quest_selection_num = quest_selection;
			}
			if (m_quest_text_line >= 3 && m_quest_text_line < 5)
			{
				// 選択が画面を返答画面進める
				m_quest_selection_num = reply_selection;
			}
		}
	}
}

//------------------------------------------
// 会話していない状態の描画処理
//------------------------------------------
void QuestAreaScene::ModeNormalDraw()
{
	// フォントサイズの設定
	SetFontSize(FONT_SIZE);

	// 文字列の高さの取得
	float h = GetFontSize();
	// 目印の描画
	for (int i = 0; i < landmark_text_max; i++)
	{
		if (m_landmark_text[i].draw_flag)
		{
			// 描画幅の取得
			float w = GetDrawStringWidth(m_landmark_text[i].text, -1);
			// 描画座標
			m_landmark_draw_pos = { m_landmark_text[i].draw_pos.x - w / 2,  m_landmark_text[i].draw_pos.y - h };
			// 文字列の描画
			if (i != excamation_pos)
			{
				if (GetJoypadNum() >= 1)
				{
					DrawString(m_landmark_draw_pos.x, m_landmark_draw_pos.y, m_landmark_text[1].text, GetColor(255, 128, 50));
				}
				else
				{
					DrawString(m_landmark_draw_pos.x, m_landmark_draw_pos.y, m_landmark_text[0].text, GetColor(255, 128, 50));
				}
			}
			else
			{
				DrawString(m_landmark_draw_pos.x, m_landmark_draw_pos.y, m_landmark_text[i].text, GetColor(255, 128, 50));
			}

		}
	}
	// プレイヤーのテキストの描画
	SetFontSize(TEXT_FONT_SIZE);
	h = GetFontSize();
	m_text_draw_pos.set((SCREEN_W / 2 - m_quest_area_text.TITLE_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_quest_area_text.CREVICE_SIZE)));
	m_quest_area_text.TextDraw(m_quest_area_text_line, { m_text_draw_pos.x, (m_text_draw_pos.y + h) }, m_quest_area_text.TITLE_BACK_SIZE);

	// 誰が話しているかの描画
	DrawString(m_text_draw_pos.x, m_text_draw_pos.y, "Player", GetColor(255, 128, 50));
}

//------------------------------------------
// 会話スタート時の描画処理
//------------------------------------------
void QuestAreaScene::TalkStartDraw()
{

}


//------------------------------------------
// 会話パートの描画処理
//------------------------------------------
void QuestAreaScene::TalkDraw()
{
	// 文字列の高さの取得
	float h = GetFontSize();

	h = GetFontSize();
	// 外枠の描画座標
	Vector2 box_pos;
	// クエスト出発確認中だけ描画する
	if (m_quest_acception_num == quest_confirmation)
	{
		// どちらを選択しているかの外枠
		if (m_select_num == 0)
		{
			box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30);
		}
		else
		{
			box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30 + (h * 2 + m_quest_text.CREVICE_SIZE));
		}

		DrawBox(box_pos.x, box_pos.y, box_pos.x + m_quest_text.QUEST_BACK_SIZE + 70, box_pos.y + h * 2 + 40, GetColor(255, 255, 0), TRUE);
		// YES
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2));
		m_quest_text.TextDraw(3, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// NO
		// Y座標はずれてほしい分を足す
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2 + (h * 2 + m_quest_text.CREVICE_SIZE)));
		m_quest_text.TextDraw(4, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);
	}

	// 会話内容の描画
	m_text_draw_pos.set((SCREEN_W / 2 - m_reception_text.QUEST_STORY_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_reception_text.CREVICE_SIZE)));
	m_reception_text.TextDraw(m_reception_text_line, { m_text_draw_pos.x, (m_text_draw_pos.y + h) }, m_reception_text.QUEST_STORY_BACK_SIZE);

	// ストーリーを進行キーの描画
	const char* text = "SPACE";
	// ゲームパッドの接続しているかによって描画するものを変更する
	if (GetJoypadNum() <= 0)
	{
		text = "SPACE";
	}
	else
	{
		text = " X ";
	}

	float x = GetDrawStringWidth(text, -1);
	// テキストバーの端っこからテキスト分引いた位置に設定
	int draw_pos_x = m_text_draw_pos.x + m_reception_text.QUEST_STORY_BACK_SIZE - x;
	// このキーを描画するのだけ透明度を変更し続ける
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_text_blend_value);

	DrawString(draw_pos_x, m_text_draw_pos.y + h, text, GetColor(255, 128, 50));
	// 暗さの変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TEXT_BLEND_MAX);

	// 誰が話しているかの描画
	DrawString(m_text_draw_pos.x, m_text_draw_pos.y, "謎の女", GetColor(255, 128, 50));
}

//------------------------------------------
// クエストを受けているときの描画処理
//------------------------------------------
void QuestAreaScene::AcceptingQuestDraw()
{
	// 文字列の高さの取得
	float h = GetFontSize();

	h = GetFontSize();

	Vector2 box_pos;
	if (m_select_num == 0)
	{
		box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30);
	}
	else
	{
		box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30 + (h * 2 + m_quest_text.CREVICE_SIZE));
	}
	// どちらを選んでいるかがわかる用のボックス
	DrawBox(box_pos.x, box_pos.y, box_pos.x + m_quest_text.QUEST_BACK_SIZE + 70, box_pos.y + h * 2 + 40, GetColor(255, 255, 0), TRUE);

	if (m_quest_selection_num == quest_selection)
	{
		m_text_draw_pos.set(m_quest_draw_pos[0].x, m_quest_draw_pos[0].y);
		// クエスト１
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2));
		m_quest_text.TextDraw(0, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// クエスト２ 
		// Y座標はずれてほしい分を足す
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2 + (h * 2 + m_quest_text.CREVICE_SIZE)));
		m_quest_text.TextDraw(1, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// 確認文言1
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_STORY_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_quest_text.CREVICE_SIZE) - 100));
		m_quest_text.TextDraw(2, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_STORY_BACK_SIZE);
	}


	if (m_quest_selection_num == reply_selection)
	{

		// YES
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2));
		m_quest_text.TextDraw(3, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// NO
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2 + (h * 2 + m_quest_text.CREVICE_SIZE)));
		m_quest_text.TextDraw(4, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// 確認文言2
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_STORY_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_quest_text.CREVICE_SIZE) - 100));
		m_quest_text.TextDraw(5, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_STORY_BACK_SIZE);
	}

	// ストーリーを進行キーの描画
	const char* text = "SPACE";
	// ゲームパッドの接続しているかによって描画するものを変更する
	if (GetJoypadNum() <= 0)
	{
		text = "SPACE";
	}
	else
	{
		text = " X ";
	}
	float x = GetDrawStringWidth(text, -1);
	// テキストバーの端っこからテキスト分引いた位置に設定
	int draw_pos_x = m_text_draw_pos.x + m_reception_text.QUEST_STORY_BACK_SIZE - x;
	// このキーを描画するのだけ透明度を変更し続ける
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_text_blend_value);
	DrawString(draw_pos_x, m_text_draw_pos.y, text, GetColor(255, 128, 50));
	// 暗さの変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TEXT_BLEND_MAX);

}

//------------------------------------------
// ボット関連の更新処理
//------------------------------------------
void QuestAreaScene::BotUpdate()
{
	// ボットとプレイヤーの移動の当たり判定
	if (CheckCapsuleHit(m_bot.m_body, m_player->m_body))
	{
		m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &m_bot.m_body);
	}

	// プレイヤーが攻撃してきた時
	if (m_player->m_attack_flag)
	{
		// playerの攻撃の時に取りたい当たり判定とモンスターの体との当たり判定をとる
		int num = m_player->m_now_attack;
		// 攻撃の当たり判定行っていいときだけ(攻撃アニメーションの指定のフレーム間だけ)
		if (m_player->AttackHitGoodTiming(num))
		{
			// モンスターのボディーとの当たり判定をとる
			if (HitAttack(m_bot.m_body, m_player->m_attack_hit_damage[num]->attack_hit) == true)
			{
				// 当たり判定があったら一回だけこの処理を通るようにする
				// ダメージ処理を行っていいフラグが上がっていたら
				if (m_player->m_can_hit_damage_flag)
				{
					// 一回だけ通ってほしいからフラグを下げる
					m_player->m_can_hit_damage_flag = false;

					// ボットが攻撃受けた時の処理
					m_bot.Update();

					// ヒットストップを行っていいいタイミングだけ行う
					if (m_player->m_attack_hit_damage[num]->can_hit_stop)
					{
						// ダメージが入ったタイミングでヒットストップのカウントをリセットする
						m_hit_stop.StopCountReset();
					}
				}
			}
		}
		else
		{
			// 当たり判定をとれないときにフラグを戻す
			m_player->m_can_hit_damage_flag = true;
		}
	}

}





