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
#include "TitleScene.h"



//------------------------------------------
// 初期処理
//------------------------------------------
void TitleScene::Init()
{
	// ベースクラスで初期化しておきたいものの初期化
	BaseInit();

	// フィールドの初期化
	// m_field_1.Init();

	// カメラの初期設定
	camera.PlayFieldInit();

	m_background_image = LoadGraph("Data/Title/Title.png");
	// 現在のシーンの設定(タイトルシーン)
	m_now_scene = Title;

	// SEの初期化
	m_sound.NewArraySecureSound(SE_MAX);
	m_sound.LoadSound("Data/SE/Title/Title_start.mp3", decision);
}

//------------------------------------------
// 更新処理
//------------------------------------------
void TitleScene::Update()
{

	switch (m_turn)
	{
	case Main:

		// タイトルシーンの更新処理
		TitleUpdate();

		break;
	case FadeOut:
		// フェードアウトの処理
		FadeOutSceneChange(Story);
		break;
	}

}

//------------------------------------------
// 描画処理
//------------------------------------------
void TitleScene::Draw()
{


	//=============================================
	// 仮でタイトルを描画
	//=============================================
	// 背景の描画
	DrawExtendGraphF(0, 0, SCREEN_W, SCREEN_H, m_background_image, TRUE);

	// フォントサイズの設定
	SetFontSize(150);
	// タイトルの描画
	const char* name = "モンスタークラッシュ";
	// 描画幅の取得
	float w = GetDrawStringWidth(name, -1);
	// 文字列の高さの取得
	float h = GetFontSize();
	// 描画座標
	Vector2 draw_pos = { SCREEN_W / 2 - w / 2, SCREEN_H / 2 - h };
	DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));

	// どのボタンを押せば始まるかを描画
	SetFontSize(100);
	if (GetJoypadNum() <= 0)
	{
		name = "SPACE";
	}
	else
	{
		name = " A ";
	}

	w = GetDrawStringWidth(name, -1);
	h = GetFontSize();
	draw_pos = { SCREEN_W / 2 - w / 2, SCREEN_H / 2 + h * 2 };
	// このキーを描画するのだけ透明度を変更し続ける
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_text_blend_value);
	DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));
	// 暗さの変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TEXT_BLEND_MAX);

	// フェードの描画処理
	FadeDraw();

}

//------------------------------------------
// 終了処理
//------------------------------------------
void TitleScene::Exit()
{
	// 再生中のSEを止める
	m_sound.StopSound();

	//　シャドーマップの削除
	ExitShadowMap();

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

		// Xキーを押された時にシーンの変更をする（今だけの仮）
		if (PushHitKey(KEY_INPUT_SPACE)||IsPadOn(PAD_ID::PAD_A))
		{
			// フェードアウト状態する
			m_turn = FadeOut;
			// SEの再生
			m_sound.PlaySound_(decision, DX_PLAYTYPE_BACK, true);
		}


		// ゲームが進んでいる間フレームをカウントし続ける
		// ストーリーを進める際のスペースキーの文字をちかちかさせるためのもの
		if (m_count_time % 1 == 0)
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
		break;

	case MOVIE:

		// プレイ動画の再生
		m_movie = PlayMovie("Data/Movie/03.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);
		//GetMovieStateToGraph(m_movie);
		// 指定の動画の再生状態を取得
		if (GetMovieStateToGraph(m_movie))
		{
			// 画面の状態をタイトル状態にする
			m_title_mode_num = TITLE;

		}
		break;
	}
}


