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
	// 最後に開放しておく
	lines.clear();
}

//---------------------------------------------------------------------------
// テキストファイルの読み込み処理
//---------------------------------------------------------------------------
void Text::LoadText(const char file_path[256], const int line_max)
{

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
}

//---------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------
void Text::TextDraw(int line_num, Vector2 draw_pos , int back_size)
{
	// 今テキストが一行づつ一気に出てくるから
	// 一文字一文字出てくるようにする

	// フォントサイズをとって文字列の背景バーの大きさを決める
	int font_size = GetFontSize();
	Vector2 box_size;
	box_size.set(draw_pos.x + back_size+ MARGIN* 2, draw_pos.y + font_size+ MARGIN*2);

	// 文字の後ろを描画ふち
	DrawBox((int)(draw_pos.x - MARGIN*2), (int)(draw_pos.y - MARGIN*2), (int)(box_size.x + MARGIN), (int)(box_size.y+ MARGIN), GetColor(m_r, m_b, m_g), TRUE);
	// 文字の後ろのところを半透明にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	// 文字の後ろを描画
	DrawBox((int)(draw_pos.x - MARGIN), (int)(draw_pos.y - MARGIN), (int)box_size.x, (int)box_size.y, GetColor(255, 255, 255), TRUE);
	// 透明度をもとにもどす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);


	// テキストデータをいったん保存する
	const char* text = 0;
	text = lines[line_num].data();
	// 描画処理
	DrawString((int)draw_pos.x, (int)(draw_pos.y + MARGIN), text, GetColor(0, 0, 0));
}


//---------------------------------------------------------------------------
// テキストの枠の色のをセットする関数
//---------------------------------------------------------------------------
void Text::SetFrameColor(int r, int g, int b)
{
	// 赤色
	m_r = r;
	// 緑
	m_b = g;
	// 青
	m_g = b;
}
