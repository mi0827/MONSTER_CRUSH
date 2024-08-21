#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"
#include "Base/Scene_Base.h"
#include "TitleScene.h"

const int Title_Time_MAX = 5;  // タイトル描画時間(今だけ3秒)
BGM title_bgm; // BGMクラスのオブジェクト
SE title_se; // SEクラスのオブジェクト

//------------------------------------------
// 初期処理
//------------------------------------------
void TiteleScene::Init()
{
	m_background_image = LoadGraph("Data/Background/Title.jpg"); // 画像の用見込み
	m_image_pos.set(0.0f, 0.0f); // 描画座標の設定

	// カウント類は最初はマックススタート
	m_count_flame = FLAME_MAX;
	m_count_time = Title_Time_MAX;

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

	// BGMの配列の確保
	title_bgm.BGM_ContainerNew(BGM_MAX);
	// BGMの読み込み
	title_bgm.Load_BGM("Data/BGM/Title/Title.mp3", TITLE_BGM);
	// BGMの再生
	title_bgm.Play_BGM(DX_PLAYTYPE_BACK, true, TITLE_BGM);
	// SE用の配列の用意
	title_se.SE_ContainerNew(SE_MAX);
	// SEの読み込み
	title_se.Load_SE("Data/SE/Title/Title_start.mp3", DECISION);

}

//------------------------------------------
// 更新処理
//------------------------------------------
void TiteleScene::Update(int bgm_volume, int se_volume)
{

	title_bgm.BGM_ChangeVolume(bgm_volume, BGM_MAX); // BGMのボリューム変更処理
	title_se.SE_ChangeVolume(se_volume, SE_MAX);     // SEのボリューム変更処理
	switch (m_title_scene)
	{
	case TITLE:

		// BGMが終わったら
		if (!title_bgm.Playing_BGM(TITLE_BGM))
		{
			// BGMの再生
			title_bgm.Play_BGM(DX_PLAYTYPE_LOOP, true, TITLE_BGM);
		}

		// ゲームパッドの情報を丸ごと取得
		if (GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A)
		{
			// Aボタンを押されたら次のシーンに移動
			m_start_flag = true;

		}
		else if (GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_A)
		{
			// Aボタンを押されたら次のシーンに移動
			m_start_flag = true;
		}
		else
		{
			// ボタンが押されていないときはフラグを下げる
			m_start_flag = false;
		}


		// スタートフラグがたっていたらSEの再生
		if (m_start_flag) {
			title_se.Play_SE(DECISION, DX_PLAYTYPE_BACK, TRUE);
		}
		// スタートフラグがたっていたら次のシーンに進む
		if (m_start_flag && title_se.Playing_SE(DECISION))
		{
			m_scene_change_judge = true; // シーンの切り替えを許可する
			title_bgm.Stop_BGM(TITLE_BGM); // BGMを止める
		}
		else {
			// フラグが下がっていたら
			m_count_flame--; // フレームのカウントを減らす
			if (m_count_flame <= 0)
			{ // フレームが設定された値以上になったら
				m_count_flame = FLAME_MAX; // フレームカウントをリセット
				m_count_time--;    // タイムカウントを進める
			}
			if (m_count_time <= 0) // タイトル画面で一定時間止まっていたら
			{
				m_title_scene = MOVIE; // 動画のシーンの切り替える
			}
		}

		break;

	case MOVIE:

		title_bgm.Stop_BGM(TITLE_BGM); // BGMを止める

		// プレイ動画の再生
		m_movie = PlayMovie("Data/Movie/02.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);
		GetMovieStateToGraph(m_movie);
		// ゲームパッドの情報を丸ごと取得
		if (GetMovieStateToGraph(m_movie))
		{
			// Aボタンを押されたらタイトル描画に戻る
			m_title_scene = TITLE;
			m_count_time = Title_Time_MAX;
			PauseMovieToGraph(m_movie);
		}
		// ゲームパッドの情報を丸ごと取得
		//if (GetJoypadInputState(DX_INPUT_PAD2) & PAD_INPUT_A) {
		//	// Aボタンを押されたらタイトル描画に戻る
		//	title_scene = TITLE;
		//}

		break;
	}
}

//------------------------------------------
// 描画処理
//------------------------------------------
void TiteleScene::Draw()
{

	// 背景画像の描画
	DrawExtendGraphF(m_image_pos.x, m_image_pos.y, SCREEN_W, SCREEN_H, m_background_image, TRUE);
	// ここでとってきた文字をセットしておく
	// 文字の最初の大きさをとっておく
	int original_font_size = GetFontSize();


	switch (m_title_scene)
	{
	    case TITLE:
	    {
		    SetFontSize(200); // フォントサイズの変更
		    const char* name = "GLADIATOR";
		    // 描画幅の取得
		    int w = GetDrawStringWidth(name, -1);
		    // 文字列の高さ取得
		    int h = GetFontSize();
		    // 描画
		    int taile_h = -50;
		    DrawString(SCREEN_W / 2 - w / 2 + 7, SCREEN_H / 2 + taile_h + 7, name, GetColor(128, 0, 0)); // 下
		    DrawString(SCREEN_W / 2 - w / 2, SCREEN_H / 2 + taile_h, name, GetColor(255, 0, 0));         // 上
	    }
	break;

	   case MOVIE:
	   {

	   }
	   break;
	}

	SetFontSize(100); // フォントサイズの変更
	DrawString(16 + 5, 16 + 5, "START: A ", GetColor(0, 128, 128)); // 下
	DrawString(16, 16, "START: A ", GetColor(0, 255, 255));         // 上
	SetFontSize(original_font_size); // フォントサイズを

}

//------------------------------------------
// 終了処理
//------------------------------------------
void TiteleScene::Exit()
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
