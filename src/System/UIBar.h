#pragma once
//! @fule UIBar.h
//! @brief バー状のUIのクラス

class UIBra
{
public:
	//! コンストラクタ
	UIBra();
	//! デストラクタ
	~UIBra();

	//! @brief バーの初期化
	//! @param バーの座標
	//! @param バーのサイズ
	//! @param 設定したい数値(最大値)
	//! @param 枠を作成するかどうか
	void Set(Vector2 pos, Vector2 size, int* value, bool line);

	//! @brief バーの色の設定
	//! @param 赤の値
	//! @param 緑の値
	//! @param 青の値
	//! @param UIBraクラスの各カラー変数のどれかを入れる(どの色を変えか変数を入れる)
	void SetColor(int red,int green, int blue, int* color);
	
	//! @brief 名前の設定関数
	//! @param 名前
	void SetName(const char name[256]);

	//! @brief 更新処理
	void Update(int* value);

	//! @brief 描画用関数
	void Draw();

public:
	//! 設定したい数値を保存する変数
	int m_value = 0;
	//! 本体かカラー
	int m_color;
	//! バックカラー
	int m_back_color;
	//! 外枠のカラー
	int m_line_color;
	//! 文字のカラー
	int m_character_color;

private:
	//! バーの座標
	Vector2 m_pos1{0,0};
	Vector2 m_pos2{ 0,0 };
	//! バーの大きさ
	Vector2 m_size{0,0};

	//! バックバーの座標
	Vector2 m_back_pos1{ 0,0 };
	Vector2 m_back_pos2{ 0,0 };

	//! バーラインのラインも引く時用の座標
	Vector2 m_line_pos1{0,0};
	Vector2 m_line_pos2{ 0,0 };
	//! バーのラインも引く時用の大きさ
	Vector2 m_line_size{0,0};
	//! 外枠のラインを引くかひかないかを判断する変数
	bool m_line_judgment = false;

	//! valueの最大値を保存しておく変数
	int m_value_max;

	//! valueの値が１減るたびにバーの減る量
	 float value_decrease;

	// 今どれだけ増減してどれほぼ残っているか
	float new_value;

	//! 名前を保存するための変数
	 char m_name[256] ;
	 // 名前を描画する可の変数(最初は名前なし状態からする)
	 bool m_name_judge = false;
};

