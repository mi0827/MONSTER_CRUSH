#pragma once

class Text
{
public:

	//! @brief コンストラクタ
	Text();
	//! @brief デストラクタ
	~Text();

	void Init();

	//! @brief テキストファイルから文字列の読み込み処理
	//! @param ファイルのパス
	//! @param テキストファイルの中身の行数
	void LoadText(const char file_path[256], const int line_max);

	//void Update();

	//! @brief テキストの描画
	//! @param 描画したいテキストの行番号
	//! @param 描画したい座標
	void DrawText(int line_num, Vector2 draw_pos);

	void Exit();

public:
	// テキストファイルから文字列を読み取ったものを保存するもの
	char m_text[][256];

};

