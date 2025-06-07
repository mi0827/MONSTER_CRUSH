#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include <fstream>
#include <string>
#include "InfoText.h"
using namespace std;

InfoText::InfoText()
{
}
InfoText::~InfoText()
{
	// 最後に開放しておく
	lines.clear();
}

//---------------------------------------
// テキストファイルから文字列の読み込み処理
//---------------------------------------

void InfoText::LoadText(const char file_path[256])
{
	// テキストファイルの読み込み
	// ファイルのデータを入れるもの
	ifstream file(file_path);
	if (file.is_open() == true)
	{
		while (true)
		{
			string line;
			// 一行取得する
			getline(file, line);

			// 何行読み取ったのかを増やす
			m_LINES_MAX++;

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


//---------------------------------------
// 読み込んだテキストの文字を数字に変換するようの関数
//---------------------------------------
void InfoText::ConversionNumbers(int text_num, float* variable_1, float* variable_2, float* variable_3)
{
	// 引数に渡された変数によって処理が少し変わる 
	// 設定された引数が一つの時
	if (variable_2 == nullptr && variable_3 == nullptr)
	{
		sscanf_s(lines[text_num].c_str(), "%f", variable_1);
	}
	// 設定された引数が二つの時
	else if (variable_3 == nullptr)
	{
		sscanf_s(lines[text_num].c_str(), "%f %f", variable_1, variable_2);
	}
	// 設定された引数が三つの時
	else
	{
		sscanf_s(lines[text_num].c_str(), "%f %f %f", variable_1, variable_2, variable_3);
	}
}




