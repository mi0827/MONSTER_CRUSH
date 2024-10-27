#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include <fstream>
#include <string>
#include "Text.h"
using namespace std;

// このソースがないと警告文が出てエラーになる
#pragma warning(disable : 4996)

Text::Text()
{
}

Text::~Text()
{
	//delete[] m_text;
}

//---------------------------------------------------------------------------
// テキストファイルの読み込み処理
//---------------------------------------------------------------------------
void Text::LoadText(const char file_path[256], const int line_max)
{

	////m_text = new char* [line_max];
	//for (int i = 0; i < line_max; i++)
	//{
	//	m_text[i] = new char[256];
	//}


	// テキストファイルの読み込み
	// ファイルのデータを入れるもの
	ifstream file(file_path);
	if (file.is_open() == true)
	{
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



	//FILE** fp = 0;
	//fopen_s(fp, file_path, "r");
	//if (fp)
	//{
	//	// 行数分繰り返す
	//	for (int i = 0; i < line_max; i++)
	//	{
	//		fgets(m_text[i], 256, *fp);
	//	}


	//	// ファイルを閉じる
	//	fclose(*fp);
	//} 

}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void Text::TextDraw(int line_num, Vector2 draw_pos)
{
	// フォントサイズをとって文字列の背景バーの大きさを決める
	int font_size = GetFontSize();
	Vector2 box_size;
	box_size.set(draw_pos.x + BACK_SIZE, draw_pos.y + font_size);
	// 文字の後ろのところを半透明にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	// 文字の後ろを描画
	DrawBox(draw_pos.x, draw_pos.y, box_size.x, box_size.y, GetColor(255, 255, 255), TRUE);
	// 透明度をもとにもどす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);


	// テキストデータをいったん保存する
	const char* text = 0;
	text = lines[line_num].data();
	// 描画処理
	DrawString(draw_pos.x, draw_pos.y, text, GetColor(0, 0, 0));



}
