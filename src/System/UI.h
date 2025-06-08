#pragma once
//! @file UI.h
//! @brief UIクラス
class UI
{
public:
	//! コンストラクタ
	UI();
	//! デストラクタ
	~UI();

	//! @brief UI描画座標とサイズの初期化
	//! @param UIの座標
	//! @param UIのサイズ
	void SetUiPosSize(Vector2 pos, Vector2 size);


	//! @brief UI画像の読み込み処理
	//! @param 画像のファイルパス
	void LoadUiImage(const char file_path[256]);


	//! @brief UIの表示非表示の変更
	//! @param 表示するかどうか、true : 描画する、false : 描画しない
	void ChangeDisplay(bool display_flag);

	//! @brief 描画用関数
	void Draw();

private:
	//! UIの座標
	Vector2 m_pos{ 0,0 };
	Vector2 m_pos2{ 0,0 };
	//! バーの大きさ
	Vector2 m_size{ 0,0 };

	//! UI画像保存用の変数
	int m_ui_image = 0;
public:
	//! 表示していいかのフラグ
	bool m_display_flag = false;
};

