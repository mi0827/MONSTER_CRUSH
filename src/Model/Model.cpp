#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"
#include "Model.h"

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
Model::Model()
{
	m_model = 0;
}

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
Model::~Model()
{
	// モデルデータの削除
	MV1DeleteModel(m_model);
}

//-----------------------------------------------
// モデル読み込み関数
//-----------------------------------------------
void Model::LoadModel(const char data[256])
{
	// モデルの読み込み
	m_model = MV1LoadModel(data);

}

//-----------------------------------------------
// 描画用関数
//-----------------------------------------------
void Model::DrawModel(Transform *transform)
{
	// モデルのサイズの調整
	m_transform = *transform;
	//m_transform.scale.set(0.1f, 0.1f, 0.1f);
	//m_transform.scale.set(1.0f, 1.0f, 1.0f);

	MV1SetPosition(m_model, VGet(m_transform.pos.x, m_transform.pos.y, m_transform.pos.z)); // 描画するプレイヤーモデルの座標の設定
	MV1SetRotationXYZ(m_model, VGet(TO_RADIAN(m_transform.rot.x), TO_RADIAN(m_transform.rot.y + 180), TO_RADIAN(m_transform.rot.z))); // モデルの回転
	MV1SetScale(m_model, m_transform.scale.VGet()); // モデルの大きさ(10分の１のサイズ)
	MV1DrawModel(m_model); // モデルの描画

}

//-----------------------------------------------
// モデルの座標をもらってくる用関数
//-----------------------------------------------
Vector3 Model::GetModelPos()
{
	return m_transform.pos;
}

//-----------------------------------------------
// モデルの座標をセットする用関数
//-----------------------------------------------
void Model::SetModelPos(Vector3 pos)
{
	m_transform.pos.set(pos);
}

//-----------------------------------------------
// モデルの向きをもらう用関数
//-----------------------------------------------
Vector3 Model::GetModelRot(Vector3 rot)
{
	return m_transform.rot;
}

//-----------------------------------------------
// モデルに向きをセットするため用関数
//-----------------------------------------------
void Model::SetModelRot(Vector3 rot)
{
	m_transform.rot.set(rot);
}

//-----------------------------------------------
// モデルのスケールを変更するための関数
//-----------------------------------------------
void Model::SetModelScale(Vector3 scale)
{
	m_transform.scale.set(scale);
}

// ---------------------------------------------- -
//-----------------------------------------------
Vector3 Model::GetNodePos(int node_index)
{
	Vector3 pos;
	// 指定したノードの座標を取ってくる
	pos = MV1GetFramePosition(m_model, node_index);

	return pos;
}