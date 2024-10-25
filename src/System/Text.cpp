#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "Text.h"
using namespace std;

// このソースがないと警告文が出てエラーになる
#pragma warning(disable : 4996)

Text::Text()
{
}

Text::~Text()
{
	delete[] m_text;
}

//---------------------------------------------------------------------------
// テキストファイルの読み込み処理
//---------------------------------------------------------------------------
void Text::LoadText(const char file_path[256], const int line_max)
{

	m_text = new char*[line_max];
	for (int i = 0; i < line_max; i++)
	{
		m_text[i] = new char[256];
	}

	// テキストファイルの読み込み
	FILE* fp = fopen(file_path, "r");
	if (fp)
	{
		// 行数分繰り返す
		for (int i = 0; i < line_max; i++)
		{
			fgets(m_text[i], 256, fp);
		}

		
		// ファイルを閉じる
		fclose(fp);
	}
	
}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void Text::TextDraw(int line_num, Vector2 draw_pos)
{
	// 描画処理
	DrawStringF(draw_pos.x, draw_pos.y, m_text[line_num], GetColor(255, 255, 255));
	
}
