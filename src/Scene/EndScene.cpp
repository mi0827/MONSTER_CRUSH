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


}

//------------------------------------------
// 更新処理
//------------------------------------------
void EndScene::Update()
{
	
	// Xキーを押された時にシーンの変更をする（今だけの仮）
	if (PushHitKey(KEY_INPUT_X))
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
void EndScene::Draw()
{
	DrawString(100, 100, "終わり", GetColor(255, 255, 255));
}

//------------------------------------------
// 終了処理
//------------------------------------------
void EndScene::Exit()
{

}
//------------------------------------------
// BGMの初期処理
//------------------------------------------
void EndScene::BGM_Init()
{
	
}
