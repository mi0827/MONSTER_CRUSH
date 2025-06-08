#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"

#include "UI.h"

//---------------------------------------
// コンストラクタ(初期化)
//---------------------------------------
UI::UI()
{

}

//---------------------------------------
// デストラクタ(終了処理)
//---------------------------------------
UI::~UI()
{

}

//---------------------------------------
// UI描画座標とサイズの初期化
//---------------------------------------
void UI::SetUiPosSize(Vector2 pos, Vector2 size)
{
	m_pos = pos;
	m_size = size;
	m_pos2 = pos + size;
}


//---------------------------------------
// UI画像の読み込み
//---------------------------------------
void UI::LoadUiImage(const char file_path[256])
{
	m_ui_image = LoadGraph(file_path);
}

//---------------------------------------
// UIの表示非表示の変更
//---------------------------------------
void UI::ChangeDisplay(bool display_flag)
{
	m_display_flag = display_flag;
}

//---------------------------------------
// 描画処理
//---------------------------------------
void UI::Draw()
{
	// 描画していいときは描画させる	
	DrawExtendGraph(m_pos.x, m_pos.y, m_pos2.x, m_pos2.y, m_ui_image, true);
}


