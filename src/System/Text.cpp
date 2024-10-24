#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "Text.h"


//---------------------------------------------------------------------------
// テキストファイルの読み込み処理
//---------------------------------------------------------------------------
void Text::LoadText(const char file_path[256], const int line_max)
{

	////m_text[1][1] = new char[line_max][256];
	//// テキストファイルの読み込み
	//FILE* fp = fopen(file_path, "r");
	//if (fp)
	//{
	//	// 行数分繰り返す
	//	for (int i = 0; i < line_max; i++)
	//	{
	//		fgets(m_text[i], 256, fp);
	//	}

	//	// ファイルを閉じる
	//	fclose(fp);

	//}

}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void Text::DrawText(int line_num, Vector2 draw_pos)
{
	// 描画処理
	DrawString(draw_pos.x, draw_pos.y, m_text[line_num], GetColor(255, 255, 255));
}
