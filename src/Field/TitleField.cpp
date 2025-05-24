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
#include "src/Field/TitleField.h"

// 1が壁０が何もない
// フィールドのオブジェクトを置く座標用の二次元配列
//	初期化処理
TitleField::TitleField()
{

}

TitleField::~TitleField()
{
}

//---------------------------------------------------------------------------
//初期処理
//---------------------------------------------------------------------------
void TitleField::Init()
{
	// モデルのパスのデータの読み込み
	m_model_data_path.LoadText("Data/Field/TitleFieldText/ModelText.txt");
	// モデルの座標のデータの読み込み
	m_object_pos_info.LoadText("Data/Field/TitleFieldText/ObjectPosData.txt");
	// モデルの向きのデータの読み込み
	m_object_rot_info.LoadText("Data/Field/TitleFieldText/ObjectRotData.txt");
	// モデルのサイズのデータの読み込み
	m_object_scale_info.LoadText("Data/Field/TitleFieldText/ObjectScaleData.txt");


	// フィールドモデルの読み込み
	m_field_model.LoadModel(m_model_data_path.lines[0].c_str());
	// 地面モデルの当たり判定の設定
	SetHitGround(&m_field_model);
	// フィールド設定
	m_field_transform.pos.set(0, -10, 0);
	m_field_transform.scale.set(1, 1, 1);

	// フィールドのオブジェクトの初期化
	ObjectInit();
}

void TitleField::ObjectInit()
{

	int model_num = 0;
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (i <= tree27)
		{
			model_num = terr;
		}
		else
		{
			model_num = fence;
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

	// 木の初期処理
	TreeInit();
	// フェンスの初期処理
	FenceInit();
	// 石の初期処理
	StoneInit();

	// 周りの値判定の設定
	// 座標の設定
	m_hit_around_pos[0].set(0.0f, 0.0f, 870.0f);      // 後ろ
	m_hit_around_pos[1].set(130.0f, 0.0f, 450.0f);    // 左側
	m_hit_around_pos[2].set(-130.0f, 0.0f, 450.0f);  // 右側
	m_hit_around_pos[3].set(0.0f, 0.0f, 0.0f);            // まえ
	// サイズの設定
	m_hit_around_size[0].set(400.0f, 90.0f, 10.0f);     // 後ろ
	m_hit_around_size[1].set(20.0f, 90.0f, 900.0f);	 // 左側
	m_hit_around_size[2].set(20.0f, 90.0f, 900.0f);	 // 右側
	m_hit_around_size[3].set(400.0f, 90.0f, 10.0f);	 // まえ

	// 当たり判定の設定
	for (int i = 0; i < AROUND_MAX; i++)
	{
		m_hit_around[i].CreateBox(m_hit_around_pos[i], m_hit_around_size[i]);
	}


}

//---------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------
void TitleField::Update()
{

}

//---------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------
void TitleField::Draw(Vector3 camera_pos, float camera_length, Vector3 player_pos)
{
	// 地面も出るの描画
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
		{
			if (ObjectDrawSituation(camera_pos, 150.0f, player_pos, m_field_object[i].transform.pos) == false)
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
void TitleField::Exit()
{

}

//---------------------------------------------------------------------------
//	木のオブジェクトの初期設定
//---------------------------------------------------------------------------
void TitleField::TreeInit()
{

	//--------------------------------------------------------------------------
	// 当たり判定の設定
	//--------------------------------------------------------------------------
	m_hit_tree[tree1].CreateCapsule(m_field_object[tree1].transform.pos, { 0.0f,50.0f,0.0f }, 100);
	// 左側
	m_hit_tree[tree2].CreateCapsule(m_field_object[tree2].transform.pos, { 0.0f,30.0f,0.0f }, 30);
	m_hit_tree[tree3].CreateCapsule(m_field_object[tree3].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree4].CreateCapsule(m_field_object[tree4].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree5].CreateCapsule(m_field_object[tree5].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree6].CreateCapsule(m_field_object[tree6].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree7].CreateCapsule(m_field_object[tree7].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree8].CreateCapsule(m_field_object[tree8].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree9].CreateCapsule(m_field_object[tree9].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree10].CreateCapsule(m_field_object[tree10].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree11].CreateCapsule(m_field_object[tree11].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree12].CreateCapsule(m_field_object[tree12].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree13].CreateCapsule(m_field_object[tree13].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	// 右側
	m_hit_tree[tree14].CreateCapsule(m_field_object[tree14].transform.pos, { 0.0f,30.0f,0.0f }, 30);
	m_hit_tree[tree15].CreateCapsule(m_field_object[tree15].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree16].CreateCapsule(m_field_object[tree16].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree17].CreateCapsule(m_field_object[tree17].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree18].CreateCapsule(m_field_object[tree18].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree19].CreateCapsule(m_field_object[tree19].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree20].CreateCapsule(m_field_object[tree20].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree21].CreateCapsule(m_field_object[tree21].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree22].CreateCapsule(m_field_object[tree22].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree23].CreateCapsule(m_field_object[tree23].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree24].CreateCapsule(m_field_object[tree24].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree25].CreateCapsule(m_field_object[tree25].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree26].CreateCapsule(m_field_object[tree26].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree27].CreateCapsule(m_field_object[tree27].transform.pos, { 0.0f,30.0f,0.0f }, 10);


}

//---------------------------------------------------------------------------
//	フェンスのオブジェクトの初期設定
//---------------------------------------------------------------------------
void TitleField::FenceInit()
{
	
}

//---------------------------------------------------------------------------
//	石のオブジェクトの初期設定
//---------------------------------------------------------------------------
void TitleField::StoneInit()
{
	
}


