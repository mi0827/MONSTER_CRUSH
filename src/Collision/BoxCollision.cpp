#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "BoxCollision.h"

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
BoxCollision::BoxCollision()
{
}

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
BoxCollision::~BoxCollision()
{
}

//-----------------------------------------------
// 描画処理
//-----------------------------------------------
void BoxCollision::Draw(int color, int line_color)
{
	/*SetUseLighting(FALSE);*/
	// 色の保存
	m_box.color = color;
	m_box.line_color = line_color;

	// 立方体の描画
	DrawCube3D(m_box.pos1.VGet(), m_box.pos2.VGet(), m_box.color, m_box.color, TRUE);
	
	//DrawCube3D(m_box.pos1.VGet(), m_box.pos2.VGet(), m_box.color, m_box.line_color, FALSE);
	/*SetUseLighting(TRUE);*/
}

//-----------------------------------------------
// ボックス作成処理
//-----------------------------------------------
void BoxCollision::CreateBox(Vector3 pos, Vector3 size )
{
	// 座標の保存
	m_box.pos1.set(pos);
	// 座標2も設定
	m_box.pos2.set(pos.x + size.x, pos.y + size.y, pos.z + size.z);
	// サイズの保存
	m_box.size.set(size);
	// 当たり判定用の半分のサイズの設定
	m_box.half_size.set(size.x / 2.0f, size.y, size.z / 2.0f);
	// 当たり判定用ボックス座標の設定
	m_box.hit_pos.set(pos.x + m_box.half_size.x, pos.y + m_box.half_size.y, pos.z + m_box.half_size.z);
	
}


void BoxCollision::SetPos(Vector3 pos)
{
	// 座標のセット
	m_box.pos1.set(pos);
	// 座標2も設定
	m_box.pos2.set(pos.x + m_box.size.x, pos.y + m_box.size.y, pos.z + m_box.size.z);
	// 当たり判定用ボックス座標の設定
	m_box.hit_pos.set(pos.x + m_box.half_size.x, pos.y + m_box.half_size.y, pos.z + m_box.half_size.z);
}

void BoxCollision::SetSize(Vector3 size)
{
	// サイズの変更
	m_box.size.set(size);
	// 当たり判定用の半分のサイズの設定
	m_box.half_size.set(size.x / 2.0f, size.y, size.z / 2.0f);
}



