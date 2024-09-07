#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Field/HitField.h"
#include "src/Field/Field.h"


// フィールドのオブジェクトを置く座標 
constexpr int MAP_W = 10;  // 縦
constexpr int MAP_H = 10;  // 横

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
	// フィールドモデルの読み込み
	m_field_model.LoadModel("Data/Field/ground/Stage00.mv1");
	// 地面モデルの当たり判定の設定
	SetHitGround(&m_field_model);
	// フィールド設定
	m_field_transform.pos.set(0, -10, 0);
	m_field_transform.scale.set(1, 1, 1);

	// フィールドのオブジェクトの初期化
	ObjectInit();
}

void Field::ObjectInit()
{
	// 木の初期処理
	TreeInit();
	// フェンスの初期処理
	FenceInit();
	// 石の初期処理
	StoneInit();
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
void Field::Draw()
{

	m_field_model.DrawModel(&m_field_transform);

	for (int i = 0; i < MODEL_MAX; i++)
	{
		m_field_object[i].model.DrawModel(&m_field_object[i].transform);
		m_field_object[i].box_hit.Draw(255, 255);
	}

}

//---------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------
void Field::Exit()
{

}

//---------------------------------------------------------------------------
//	木のオブジェクトの初期設定
//---------------------------------------------------------------------------
void Field::TreeInit()
{
	// モデルデータの読み込み
	m_field_object[tree1].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree2].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree3].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");

	// 座標設定
	m_field_object[tree1].transform.pos.set(0.0f, 30.0f, 0.0f);
	m_field_object[tree2].transform.pos.set(0.0f, 30.0f, 240.0f);
	m_field_object[tree3].transform.pos.set(240.0f, 30.0f, 0.0f);


	// 向きの設定
	m_field_object[tree1].transform.rot.set(0.0f, 45.0f, 0.0f);
	m_field_object[tree2].transform.rot.set(0.0f, 45.0f, 0.0f);
	m_field_object[tree3].transform.rot.set(0.0f, 45.0f, 0.0f);

	// サイズの設定
	m_field_object[tree1].transform.scale.set(0.5f, 0.5f, 0.5f);
	m_field_object[tree2].transform.scale.set(0.5f, 0.5f, 0.5f);
	m_field_object[tree3].transform.scale.set(0.5f, 0.5f, 0.5f);

	// 当たり判定の設定
	m_field_object[tree1].box_hit.CreateBox(m_field_object[tree1].transform.pos, { 50.0f,20.0f,50.f });
	m_field_object[tree2].box_hit.CreateBox(m_field_object[tree2].transform.pos, { 50.0f,20.0f,50.f });
	m_field_object[tree3].box_hit.CreateBox(m_field_object[tree3].transform.pos, { 50.0f,20.0f,50.f });
}

//---------------------------------------------------------------------------
//	フェンスのオブジェクトの初期設定
//---------------------------------------------------------------------------
void Field::FenceInit()
{
	// モデルデータの読み込み
	m_field_object[fence1 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence2 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence3 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence4 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence5 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence6 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence7 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence8 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence9 ].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence10].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence11].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence12].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence13].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence14].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence15].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence16].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence17].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence18].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence19].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence20].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	
	// 座標設定
	m_field_object[fence1  ].transform.pos.set(0.0f, 30.0f, 240.0f);
	m_field_object[fence2  ].transform.pos.set(60.0f, 30.0f, 240.0f);
	m_field_object[fence3  ].transform.pos.set(120.0f, 30.0f, 240.0f);
	m_field_object[fence4  ].transform.pos.set(180.0f, 30.0f, 240.0f);
	m_field_object[fence5  ].transform.pos.set(240.0f, 30.0f, 240.0f);

	m_field_object[fence6  ].transform.pos.set(0.0f, 30.0f, 0.0f);
	m_field_object[fence7  ].transform.pos.set(60.0f, 30.0f, 0.0f);
	m_field_object[fence8  ].transform.pos.set(120.0f, 30.0f,0.0f);
	m_field_object[fence9  ].transform.pos.set(180.0f, 30.0f, 0.0f);
	m_field_object[fence10].transform.pos.set(240.0f, 30.0f, 0.0f);

	m_field_object[fence11].transform.pos.set(0.0f, 30.0f, 20.0f);
	m_field_object[fence12].transform.pos.set(0.0f, 30.0f, 60.0f);
	m_field_object[fence13].transform.pos.set(0.0f, 30.0f, 120.0f);
	m_field_object[fence14].transform.pos.set(0.0f, 30.0f, 180.0f);
	m_field_object[fence15].transform.pos.set(0.0f, 30.0f, 240.0f);

	m_field_object[fence16].transform.pos.set(240.0f, 30.0f, 0.0f);
	m_field_object[fence17].transform.pos.set(240.0f, 30.0f, 60.0f);
	m_field_object[fence18].transform.pos.set(240.0f, 30.0f, 120.0f);
	m_field_object[fence19].transform.pos.set(240.0f, 30.0f, 180.0f);
	m_field_object[fence20].transform.pos.set(240.0f, 30.0f, 240.0f);

	// 向きの設定
	m_field_object[fence1 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence2 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence3 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence4 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence5 ].transform.rot.set(0.0f, 0.0f, 0.0f);

	m_field_object[fence6 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence7 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence8 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence9 ].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence10].transform.rot.set(0.0f, 0.0f, 0.0f);

	m_field_object[fence11].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence12].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence13].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence14].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence15].transform.rot.set(0.0f, 90.0f, 0.0f);

	m_field_object[fence16].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence17].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence18].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence19].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence20].transform.rot.set(0.0f, 90.0f, 0.0f);
	
	// サイズの設定
	m_field_object[fence1].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence2].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence3].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence4].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence5].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence6].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence7].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence8].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence9].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence10].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence11].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence12].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence13].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence14].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence15].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence16].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence17].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence18].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence19].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence20].transform.scale.set(0.1f, 0.1f, 0.1f);

	// 当たり判定の設定
	m_field_object[fence1].box_hit.CreateBox(m_field_object[fence1].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence2].box_hit.CreateBox(m_field_object[fence2].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence3].box_hit.CreateBox(m_field_object[fence3].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence4].box_hit.CreateBox(m_field_object[fence4].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence5].box_hit.CreateBox(m_field_object[fence5].transform.pos, { 60.0f,20.0f,5.0f });

	m_field_object[fence6].box_hit.CreateBox(m_field_object[fence6].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence7].box_hit.CreateBox(m_field_object[fence7].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence8].box_hit.CreateBox(m_field_object[fence8].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence9].box_hit.CreateBox(m_field_object[fence9].transform.pos, { 60.0f,20.0f,5.0f });
	m_field_object[fence10].box_hit.CreateBox(m_field_object[fence10].transform.pos, { 60.0f,20.0f,5.0f });

	m_field_object[fence11].box_hit.CreateBox(m_field_object[fence11].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence12].box_hit.CreateBox(m_field_object[fence12].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence13].box_hit.CreateBox(m_field_object[fence13].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence14].box_hit.CreateBox(m_field_object[fence14].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence15].box_hit.CreateBox(m_field_object[fence15].transform.pos, { 5.0f,20.0f,60.0f });

	m_field_object[fence16].box_hit.CreateBox(m_field_object[fence16].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence17].box_hit.CreateBox(m_field_object[fence17].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence18].box_hit.CreateBox(m_field_object[fence18].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence19].box_hit.CreateBox(m_field_object[fence19].transform.pos, { 5.0f,20.0f,60.0f });
	m_field_object[fence20].box_hit.CreateBox(m_field_object[fence20].transform.pos, { 5.0f,20.0f,60.0f });

}

//---------------------------------------------------------------------------
//	石のオブジェクトの初期設定
//---------------------------------------------------------------------------
void Field::StoneInit()
{
	// モデルデータの読み込み
	m_field_object[stone1].model.LoadModel("Data/Field/object/stone/Stage_Obj002.mv1");
	// 座標設定
	m_field_object[stone1].transform.pos.set(-50.0f, 30.0f, 0.0f);

	// 向きの設定
	m_field_object[stone1].transform.rot.set(0.0f, 0.0f, 0.0f);

	// サイズの設定
	m_field_object[stone1].transform.scale.set(0.05f, 0.05f, 0.05f);

	// 当たり判定の設定
	m_field_object[stone1].box_hit.CreateBox(m_field_object[stone1].transform.pos, { 50.0f,20.0f,110.f });
}
