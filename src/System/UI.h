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


	//! @brief 更新処理
	//! 
	void ChangeDisplay();

	//! @brief 描画用関数
	void Draw();

	//! @brief バーと点との当たり判定を返す
	//! @param x座標
	//! @param y座標
	//! @return true : ヒットしている、false : ヒットしていない
	bool HitPointBra(float pos_x, float pos_y);

private:
	//! UIの座標
	Vector2 m_pos{ 0,0 };
	Vector2 m_pos2{ 0,0 };
	//! バーの大きさ
	Vector2 m_size{ 0,0 };

	//! UI画像保存用の変数
	int m_ui_image = 0;

	//! 表示していいかのフラグ
	bool m_display_flag = false;
};

