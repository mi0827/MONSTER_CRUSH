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
void CapsuleCollision::CreateNodoCapsule(Model* model, int nodo_index)
{
	// モデルの指定の位置の座標を取ってくる
	m_capsule.pos1 = model->GetNodePos(nodo_index);
	// サイズは適当な初期値を入れておく
	m_capsule.pos2 = m_capsule.pos1 + Vector3(1.0f, 1.0f, 1.0f);
	// 半径も適当な値を入れておく
	m_capsule.radius = 1.0f;
}

//-----------------------------------------------
// カプセルをつける
//-----------------------------------------------
void CapsuleCollision::CreateCapsule(Vector3 pos)
{
	// 座標の保存
	m_capsule.pos1 = pos;
	// サイズは適当な初期値を入れておく
	m_capsule.pos2 = m_capsule.pos1 + Vector3(1.0f, 1.0f, 1.0f);
	// 半径も適当な値を入れておく
	m_capsule.radius = 1.0f;
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
