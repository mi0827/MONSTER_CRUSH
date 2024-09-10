#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"
#include "Base/Scene_Base.h"
#include "TitleScene.h"



//------------------------------------------
// 初期処理
//------------------------------------------
void TitleScene::Init()
{
	
}

//------------------------------------------
// 更新処理
//------------------------------------------
void TitleScene::Update()
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
void TitleScene::Draw()
{

	DrawString(100, 100, "タイトル", GetColor(255, 255, 255));

}

//------------------------------------------
// 終了処理
//------------------------------------------
void TitleScene::Exit()
{
	

}
