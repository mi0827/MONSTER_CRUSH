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
	void TextDraw(int line_num, Vector2 draw_pos, int back_size);

	//void Exit();

public:
	// 文字列を保存する配列の最大数
	static constexpr int CHAE_MAX = 256;

	// 文字列を描画する際画面の上下の場合に開けてほしい隙間の大きさ
	static constexpr int CREVICE_SIZE = 50;

	// 文字列の背景の大きさ
	static constexpr int TITLE_BACK_SIZE = 1000; //タイトルストーリー (クエストエリアでも使っている)
	static constexpr int OPTION_BACK_SIZE = 500; // オプションメニュー
	static constexpr int END_BACK_SIZE = 650;    // エンドシーン
	// 半分のサイズ
	static constexpr int  TITLE_BACK_HALF_SIZE = TITLE_BACK_SIZE / 2; //タイトルバージョン (クエストエリアでも使っている)
	static constexpr int OPTION_BACK_HALF_SIZE = OPTION_BACK_SIZE / 2; // オプションメニュー
	static constexpr int  END_BACK_HALF_SIZE = END_BACK_SIZE / 2;       // エンドシーン
	// テキストファイルから文字列を読み取ったものを保存するもの
	// 文字列を入れる
	std::vector<std::string> lines;
};

