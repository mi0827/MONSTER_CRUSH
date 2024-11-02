#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "CapsuleCollision.h"

CapsuleCollision::CapsuleCollision()
{
}

CapsuleCollision::~CapsuleCollision()
{
}

//-----------------------------------------------
// カプセルの描画
//-----------------------------------------------
void CapsuleCollision::Draw()
{
	DrawCapsule3D(m_capsule.pos1.VGet(), m_capsule.pos2.VGet(), m_capsule.radius, 8, GetColor(255, 0, 0), GetColor(255, 255, 255), FALSE);
}

//-----------------------------------------------
// モデルの指定の位置にカプセルをつける
//-----------------------------------------------
void CapsuleCollision::CreateNodoCapsule(Model* model, int nodo_index, int node_index2, float radius)
{
	// モデルの指定の位置の座標を取ってくる
	m_capsule.pos1 = model->GetNodePos(nodo_index);
	// サイズは適当な初期値を入れておく
	m_capsule.pos2 = m_capsule.pos1 + Vector3(1.0f, 1.0f, 1.0f);
	// サイズを入れる
	NodoSetSize(model, node_index2, radius);
}

//-----------------------------------------------
// カプセルをつける
//-----------------------------------------------
void CapsuleCollision::CreateCapsule(Vector3 pos,Vector3 pos2, float radius)
{
	// 座標の保存
	m_capsule.pos1 = pos;
	// サイズの設定
	SetSize(pos2, radius);
}

void CapsuleCollision::CreateCapuselCoordinatePos(Vector3 pos, Vector3 pos2, float radius)
{
	// 座標の保存
	m_capsule.pos1 = pos;
	// 
	m_capsule.pos2 = pos2;
}


//-----------------------------------------------
// カプセルのもう一つの座標をノードからもらってくる
//-----------------------------------------------
void CapsuleCollision::NodoSetSize(Model* model, int nodo_index, float radius)
{
	// サイズの調整
	m_capsule.pos2 = model->GetNodePos(nodo_index);
	// カプセルの半径の設定
	m_capsule.radius = radius;
}

//-----------------------------------------------
// カプセルのサイズの調整
//-----------------------------------------------
void CapsuleCollision::SetSize(Vector3 size,  float radius)
{
	// サイズの調整
	m_capsule.pos2 = m_capsule.pos1 + size;
	// カプセルの半径の設定
	m_capsule.radius = radius;
}
