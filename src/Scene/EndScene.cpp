#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"
#include "Base/Scene_Base.h"
#include "EndScene.h"

BGM end_bgm;

const int End_Time_MAX = 3;  // エンド描画時間(今だけ3秒)
//------------------------------------------
// 初期処理
//------------------------------------------
void EndScene::Init()
{
	m_background_image = LoadGraph("Data/background/End.jpg"); // 背景画像の読み込み
	m_image_pos.set(0.0f, 0.0f); // 描画座標の設定

	// カウント類は最初はマックススタート
	m_count_flame = FLAME_MAX;
	m_count_time  = End_Time_MAX;

	m_scene_change_judge = false; // 最初はシーンの切り替えをしてはいけない


	// フォントデータの読み込み
	//GTA_font_data = LoadFontDataToHandle("Data/Font/Gta/GTA.dft", 1.0f);
	m_GTA_font_path = "Data/Font/Gta/pricedown bl.ttf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(m_GTA_font_path, FR_PRIVATE, NULL) > 0) 
	{

	}
	else 
	{
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}
	ChangeFont("Pricedown Bl", DX_CHARSET_DEFAULT);

	// BGMの設定
	BGM_Init();

}

//------------------------------------------
// 更新処理
//------------------------------------------
void EndScene::Update(int bgm_volume, int se_volume)
{
	// BGMの音量の調整
	end_bgm.BGM_ChangeVolume(bgm_volume, BGM_MAX);

	m_count_flame--; // フレームのカウントを減らす
	if (m_count_flame <= 0) { // フレームが設定された値以上になったら
		m_count_flame = FLAME_MAX; // フレームカウントをリセット
		m_count_time--;    // タイムカウントを進める
	}
	//Flame_Time_Update(&count_flame, &count_time);
	if (m_count_time <= 0) // タイトル画面で10秒経ったら
	{
		m_scene_change_judge = true; // シーンの切り替えを許可する
		// BGMの再生を止める
		end_bgm.Stop_BGM(BGM_1);
	}
}

//------------------------------------------
// 描画処理
//------------------------------------------
void EndScene::Draw()
{
	// 文字列の描画と描画幅の取得で2回使うのでここで定義しときます
	int original_font_size = GetFontSize();
	// 背景画像の描画

	DrawExtendGraph((int)m_image_pos.x, (int)m_image_pos.y, SCREEN_W, SCREEN_H, (int)m_background_image, TRUE);
	SetFontSize(100); // フォントサイズの変更
	DrawFormatString(16, 16, GetColor(0, 0, 0), "RESTART:%2d", m_count_time);
	//SetFontSize(40); // フォントサイズの変更
	//DrawFormatString(16, 16, GetColor(255, 255, 255), "RESTART:%2d", count_time);


	SetFontSize(160); // フォントサイズの変更
	// 文字列の描画と描画幅の取得で2回使うのでここで定義しときます
	const char* name = "Thank you for playing";
	// 描画幅の取得
	int w = (int)GetDrawStringWidth(name, -1);
	// 文字列の高さ取得
	int h = (int)GetFontSize();
	// 描画
	DrawStringF(SCREEN_W / 2 - (float)(w / 2 - 3), SCREEN_H / 2 - 3, name, GetColor(255, 255, 255), TRUE);
	DrawStringF(SCREEN_W / 2 - (float)(w / 2), SCREEN_H / 2, name, GetColor(0, 0, 0), TRUE);

	SetFontSize(80); // フォントサイズの変更
	ChangeFont("ＭＳ 明朝", DX_CHARSET_DEFAULT); // フォントを明朝体にする
	DrawStringF(16, 800, "  SE：魔王魂", GetColor(0, 0, 0));
	DrawStringF(16, 900, " BGM：創作堂さくら紅葉", GetColor(0, 0, 0));
	DrawStringF(16, 1000, "BGM：MusMus", GetColor(0, 0, 0));

	SetFontSize(original_font_size); // フォントサイズを戻す
}

//------------------------------------------
// 終了処理
//------------------------------------------
void EndScene::Exit()
{
	DeleteGraph(m_background_image); // 画像データの解放
	// ********** フォントのアンロード **********
	if (RemoveFontResourceEx(m_GTA_font_path, FR_PRIVATE, NULL))
	{

	}
	else 
	{
		MessageBox(NULL, "remove failure", "", MB_OK);
	}
	ChangeFont("ＭＳ 明朝", DX_CHARSET_DEFAULT);
}

//------------------------------------------
// BGMの初期処理
//------------------------------------------
void EndScene::BGM_Init()
{
	// BGM用の配列の用意
	end_bgm.BGM_ContainerNew(BGM_MAX);
	// BGMの読み込み
	end_bgm.Load_BGM("Data/BGM/End/end.mp3", BGM_1);

	// 流したいBGMを再生する
	end_bgm.Play_BGM(DX_PLAYTYPE_BACK, true, BGM_1);
}
