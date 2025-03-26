#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/Model/Model.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"


#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h"
#include "src/Field/Field.h"


// 1が壁０が何もない
// フィールドのオブジェクトを置く座標用の二次元配列
//	初期化処理
Field::Field()
{

}

Field::~Field()
{

}

//---------------------------------------------------------------------------
//初期処理
//---------------------------------------------------------------------------
void Field::Init()
{
	// モデルのパスのデータの読み込み
	m_model_data_path.LoadText("Data/Field/FieldText/ModelText.txt");
	// モデルの座標のデータの読み込み
	m_object_pos_info.LoadText("Data/Field/FieldText/ObjectPosData.txt");
	// モデルの向きのデータの読み込み
	m_object_rot_info.LoadText("Data/Field/FieldText/ObjectRotData.txt");
	// モデルのサイズのデータの読み込み
	m_object_scale_info.LoadText("Data/Field/FieldText/ObjectScaleData.txt");

	// フィールドモデルの読み込み
	//m_field_model.LoadModel("Data/Field/ground/Stage00.mv1");
	m_field_model.LoadModel(m_model_data_path.lines[0].c_str());
	// 地面モデルの当たり判定の設定
	SetHitGround(&m_field_model);
	// フィールド設定
	m_field_transform.pos.set(0, -10, 0);
	m_field_transform.scale.set(0.5f, 0.5f, 0.5f);

	// フィールドのオブジェクトの初期化
	ObjectInit();
}

void Field::ObjectInit()
{
	int model_num = 0;
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (i <= tree24)
		{
			model_num = terr;
		}
		else
		{
			model_num = stone;
		}

		// モデルの読み込み処理
		m_field_object[i].model.LoadModel(m_model_data_path.lines[model_num].c_str());
		// モデルの座標設定
		m_object_pos_info.ConversionNumbers(i, &m_field_object[i].transform.pos.x, &m_field_object[i].transform.pos.y, &m_field_object[i].transform.pos.z);
		// モデルの向きの設定
		m_object_rot_info.ConversionNumbers(i, &m_field_object[i].transform.rot.x, &m_field_object[i].transform.rot.y, &m_field_object[i].transform.rot.z);
		// モデルのサイズの設定
		m_object_scale_info.ConversionNumbers(i, &m_field_object[i].transform.scale.x, &m_field_object[i].transform.scale.y, &m_field_object[i].transform.scale.z);
	}

	// ボックスの座標を保存するためのもの
	Vector3 pos = { 100.0f,0.0f,300.0f };
	// ボックスのサイズ
	Vector3 size = { 30.0f,60.0f,500.0f };
	m_hit_wall[0].CreateBox(pos, size);

	pos.set(500.0f, 0.0f, 300.0f);
	size.set(30.0f, 60.0f, 500.0f);
	m_hit_wall[1].CreateBox(pos, size);

	pos.set(300.0f, 0.0f, 100.0f);
	size.set(500.0f, 60.0f, 30.0f);
	m_hit_wall[2].CreateBox(pos, size);

	pos.set(300.0f, 0.0f, 500.0f);
	size.set(500.0f, 60.0f, 30.0f);
	m_hit_wall[3].CreateBox(pos, size);


}

//---------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------
void Field::Update()
{

}

//---------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------
void Field::Draw(Vector3 camera_pos, float camera_length,Vector3 player_pos)
{
	// 半透明を先に描画する
	// 描画ブレンドモードをアルファブレンド（５０％）にする

	m_field_model.DrawModel(&m_field_transform);

	// 半透明にしない分を描画
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (ObjectDrawSituation(camera_pos, camera_length, player_pos, m_field_object[i].transform.pos))
		{
			m_field_object[i].model.DrawModel(&m_field_object[i].transform);
		}
	}
	// 半透明にする分の描画
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (i >= stone1)
		{
			if (ObjectDrawSituation(camera_pos, camera_length, player_pos, m_field_object[i].transform.pos) == false)
			{
				// 本来ここで
				MV1SetOpacityRate(m_field_object[i].model.m_model, 0.3f);
				// モデルの描画
				m_field_object[i].model.DrawModel(&m_field_object[i].transform);
				// 描画ブレンドモードをノーブレンドにする
				MV1SetOpacityRate(m_field_object[i].model.m_model, 1.0f);
			}
		}
	}	
}

//---------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------
void Field::Exit()
{

}






