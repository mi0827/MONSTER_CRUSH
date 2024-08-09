#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "UIBar.h"

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
UIBra::UIBra()
{
}

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
UIBra::~UIBra()
{
}

//-----------------------------------------------
// 初期設定
//-----------------------------------------------
void UIBra::Set(Vector2 pos, Vector2 size, int* value, bool line)
{
	// 設定の保存
	m_pos1 = pos;                // 座標１
	m_size = size;                 // サイズ
	m_pos2 = pos + size;	      // 座標２
	m_value = *value;			  // 数値
	m_value_max = *value;   // 数値の最大数を保存
	m_line_judgment = line;	  // 外枠を作るかどうか

	// バックバーの設定
	m_back_pos1 = m_pos1;
	m_back_pos2 = m_pos2;

	// 外枠のラインを引く場合
	if (m_line_judgment) 
	{
		// 外枠のラインの設定
		m_line_pos1 = m_pos1;
		m_line_size = m_size;
		m_line_pos2 = m_pos2;
		SetColor(255, 255, 255, &m_line_color);
	}
	// 各カラーの設定
	SetColor(128, 128, 128, &m_color);
	SetColor(0, 0, 0, &m_back_color);
	SetColor(255, 255, 0, &m_character_color);

	// valueの値が１減るたびに減る量の設定
	aaa = m_size.x / m_value;


}

//-----------------------------------------------
// カラーのセット用関数
//-----------------------------------------------
void UIBra::SetColor(int red, int green, int blue, int* color)
{
	// 色の設定
	*color = GetColor(red, green, blue);
}

//-----------------------------------------------
// 名前の設定
//-----------------------------------------------
void UIBra::SetName(const char name[256])
{
	for (int i =0; i < 256; i++)
	{
		m_name[i] = name[i];
	}
	// 名前の設定をしたので名前のフラグを設定する
	m_name_judge = true;
}

//-----------------------------------------------
// 更新処理
//-----------------------------------------------
void UIBra::Update(int*value)
{
	// valueがどれだけ減っているのかを調べる
	int value_difference = m_value_max - m_value ;
	// 減っている分のバーを減らしたい
	new_aaa = aaa * value_difference;
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void UIBra::Draw()
{
	
	// バックバーの描画
	DrawBox(m_back_pos1.x, m_back_pos1.y, m_back_pos2.x, m_back_pos2.y, m_back_color, TRUE);
	
	DrawBox(m_pos1.x, m_pos1.y, m_pos2.x + new_aaa, m_pos2.y, m_color, TRUE);
	
	if (m_name_judge )
	{
		// 現在のフォントのサイズを保存してい置く
		int font_size = GetFontSize();
		// フォントのサイズを設定（バーのサイズに合わせる）
		SetFontSize(m_size.y);
		// 名前の描画
		DrawString(m_back_pos1.x, m_back_pos1.y, m_name, m_character_color);
		// フォントのサイズをもとのサイズに戻す
		SetFontSize(font_size);
	}
	
	// 外枠のラインを引く場合
	if (m_line_judgment)
	{
		DrawBox(m_line_pos1.x, m_line_pos1.y, m_line_pos2.x, m_line_pos2.y, m_line_color, FALSE);
	}

}
