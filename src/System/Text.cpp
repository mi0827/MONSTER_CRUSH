#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "Text.h"
#include <fstream>
#include <string>
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

	m_text = new char* [line_max];
	for (int i = 0; i < line_max; i++)
	{
		m_text[i] = new char[256];
	}


	// テキストファイルの読み込み
	// ファイルのデータを入れるもの
	ifstream file(file_path);
	if (file.is_open() __ true)
	{
		// 文字列を入れる
		vector<string> lines;
		while (true)
		{
			string line;
			getline(file, line);
			// データの一番最後に行ったら
			if (file.eof())
			{
				break;
			}
			// vectorの配列に追加
			lines.emplace_back(line);
		}
		// ファイルを閉じる
		file.close();
	}



	FILE** fp = 0;
	fopen_s(fp, file_path, "r");
	if (fp)
	{
		// 行数分繰り返す
		for (int i = 0; i < line_max; i++)
		{
			fgets(m_text[i], 256, *fp);
		}


		// ファイルを閉じる
		fclose(*fp);
	} 

}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void Text::TextDraw(int line_num, Vector2 draw_pos)
{
	// 描画処理
	DrawStringF(draw_pos.x, draw_pos.y, lines[line_num], GetColor(255, 255, 255));

}
