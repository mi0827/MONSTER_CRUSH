#pragma once

class Text
{
public:

	//! @brief コンストラクタ
	Text();
	//! @brief デストラクタ
	~Text();

	//void Init();

	//! @brief テキストファイルから文字列の読み込み処理
	//! @param ファイルのパス
	//! @param テキストファイルの中身の行数
	void LoadText(const char file_path[256], const int line_max);

	//void Update();

	//! @brief テキストの描画
	//! @param 描画したいテキストの行番号
	//! @param 描画したい座標
	void TextDraw(int line_num, Vector2 draw_pos);

	//void Exit();

public:
	// 文字列を保存する配列の最大数
	static constexpr int CHAE_MAX = 256;
	// 文字列の背景の大きさ
	static constexpr int BACK_SIZE = 1000;

	// テキストファイルから文字列を読み取ったものを保存するもの
	//char **m_text;
	// 文字列を入れる
	std::vector<std::string> lines;


};

