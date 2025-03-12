#pragma once
// !@file InfoText.h
//! @テキストファイルから情報の読み取り、書き込み用のクラス
class InfoText
{
public:

	//! @brief コンストラクタ
	InfoText();
	//! @brief デストラクタ
	~InfoText();

	//void Init();

	//! @brief テキストファイルから文字列の読み込み処理
	//! @param ファイルのパス
	//! @param テキストファイルの中身の行数
	void LoadText(const char file_path[256]);

	//! @brief 読み込んだテキストの文字を数字に変換するようの関数
	//! @param 読み込んだテキストの何行目を変換したいか
	//! @param 変換した数字を保存するための変数１
	//! @param 変換した数字を保存するための変数２(なければ設定されない)
	//! @param 変換した数字を保存するための変数３(なければ設定されない)
	void ConversionNumbers(int text_num , float* variable_1, float* variable_2 = nullptr, float* variable_3 = nullptr);
	//! @brief テキストに書き込むように変数
	void UpText();


public:
	// 文字列を保存する配列の最大数
	static constexpr int CHAE_MAX = 256;

	// 何行読み取ったのかを保存するためのもの
	int m_LINES_MAX = 0;
	// テキストファイルから文字列を読み取ったものを保存するもの
	// 文字列を入れる
	std::vector<std::string> lines;



};