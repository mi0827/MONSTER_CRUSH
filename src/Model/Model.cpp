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
	
	GetMaterilaInfo();
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

//-----------------------------------------------
// モデルの指定のノードを取得する
//-----------------------------------------------
Vector3 Model::GetNodePos(int node_index)
{
	Vector3 pos;
	// 指定したノードの座標を取ってくる
	pos = MV1GetFramePosition(m_model, node_index);

	return pos;
}


//-----------------------------------------------
// マテリアルの各情報を取得する
//-----------------------------------------------
void Model::GetMaterilaInfo()
{
	m_material_max = MV1GetMaterialNum(m_model);

	// マテリアルの数文配列を増やす
	m_material.resize(m_material_max);

	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Diffuse = MV1GetMaterialDifColor(m_model, i);
		m_material[i].Ambient = MV1GetMaterialAmbColor(m_model, i);
		m_material[i].Emissive = MV1GetMaterialEmiColor(m_model, i);
		m_material[i].Specular = MV1GetMaterialSpcColor(m_model, i);
		m_material[i].Power = MV1GetMaterialSpcPower(m_model, i);
	}

}

//-----------------------------------------------
// マテリアルのDiffuseの設定用関数
//-----------------------------------------------
void Model::SetMaterialDiffuse(COLOR_F color)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Diffuse = { color.r,color.g,color.b, color.a };
		// 受け取ってきたカラーを設定する
		MV1SetMaterialDifColor(m_model, i, m_material[i].Diffuse);
	}
	
}

//-----------------------------------------------
// マテリアルのAmbientの設定用関数
//-----------------------------------------------
void Model::SetMaterialAmbient(COLOR_F color)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Ambient = { color.r,color.g,color.b, color.a };
		// 受け取ってきたカラーを設定する
		MV1SetMaterialAmbColor(m_model, i, m_material[i].Ambient);
	}
}

//-----------------------------------------------
// マテリアルのSpecularの設定用関数
//-----------------------------------------------
void Model::SetMaterialSpecular(COLOR_F color)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Specular = { color.r,color.g,color.b, color.a };
		// 受け取ってきたカラーを設定する
		MV1SetMaterialSpcColor(m_model, i, m_material[i].Specular);
	}
}

//-----------------------------------------------
// マテリアルのEmissiveの設定用関数
//-----------------------------------------------
void Model::SetMaterialEmissive(COLOR_F color)
{
	for(int i = 0; i < m_material_max; i++)
	{
		m_material[i].Emissive = { color.r,color.g,color.b, color.a };
		// 受け取ってきたカラーを設定する
		MV1SetMaterialEmiColor(m_model, i, m_material[i].Emissive);
		m_material[i].Emissive = MV1GetMaterialEmiColor(m_model, i);
	}
}

//-----------------------------------------------
// マテリアルのスペキュラハイライトの鮮明度の設定用関数
//-----------------------------------------------
void Model::SetMaterialPower(float power)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Power = power;
		// 受け取ってきたスペキュラーのを設定する
		MV1SetMaterialSpcPower(m_model, i, m_material[i].Power);
	}
}
