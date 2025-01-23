#pragma once
//! @file Text.h
//! @テキストファイルから読み取り描画するクラス
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
	//! @param 文字のところの背景のサイズ
	void TextDraw(int line_num, Vector2 draw_pos, int back_size);

	//! @brief テキストの枠の色のをセットする関数
	//! @param r 赤色( 0 ～ 255)
	//! @param g 緑色( 0 ～ 255)
	//! @param b 青色( 0 ～ 255) 
	void SetFrameColor( int r, int g,int b );
	//void Exit();

public:
	// 文字列を保存する配列の最大数
	static constexpr int CHAE_MAX = 256;

	// 文字列を描画する際画面の上下の場合に開けてほしい隙間の大きさ
	static constexpr int CREVICE_SIZE = 50;

	// 文字列の背景の大きさ
	static constexpr int TITLE_BACK_SIZE = 1000; //タイトルストーリー (クエストエリアでも使っている)
	static constexpr int QUEST_STORY_BACK_SIZE = 1400; // クエスト選択画面の会話が表示されているところ
	static constexpr int QUEST_BACK_SIZE = 500; // クエストが表示されている場所
	static constexpr int OPTION_BACK_SIZE = 350; // オプションメニュー
	static constexpr int END_BACK_SIZE = 650;    // エンドシーン
	// 半分のサイズ
	static constexpr int  TITLE_BACK_HALF_SIZE = TITLE_BACK_SIZE / 2; //タイトルバージョン (クエストエリアでも使っている)
	static constexpr int QUEST_STORY_BACK_HALF_SIZE = QUEST_STORY_BACK_SIZE / 2; // クエスト選択画面の会話が表示されているところ
	static constexpr int QUEST_BACK_SIZE_HALF_SIZE = QUEST_BACK_SIZE / 2; // クエストが表示されている場所
	static constexpr int  OPTION_BACK_HALF_SIZE = OPTION_BACK_SIZE / 2; // オプションメニュー
	static constexpr int  END_BACK_HALF_SIZE = END_BACK_SIZE / 2;       // エンドシーン
	// テキストファイルから文字列を読み取ったものを保存するもの
	// 文字列を入れる
	std::vector<std::string> lines;


private:
	// テキストと背景との余白の大きさ
	static constexpr int MARGIN = 10;
	
	// テキスト描画の枠の色
	int m_r = 0;
	int m_b = 0;
	int m_g = 0;
};

