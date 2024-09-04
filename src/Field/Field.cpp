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
	Object_Init();
}

void Field::Object_Init()
{
	//---------------------------------
	// 木のモデル
	//--------------------------------
	// モデルデータの読み込み
	m_field_object[tree1].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	// 座標設定
	m_field_object[tree1].transform.pos.set(50.0f, 50.0f, 0.0f);

	// 向きの設定
	m_field_object[tree1].transform.rot.set(0.0f, 0.0f, 0.0f);

	// サイズの設定
	m_field_object[tree1].transform.scale.set(0.5f, 0.5f, 0.5f);

	// 当たり判定の設定
	m_field_object[tree1].box_hit.CreateBox(m_field_object[tree1].transform.pos, { 50.0f,20.0f,50.f });

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

	/*for (int i = 0; i < MODEL_MAX; i++)
	{
		m_field_object[i].model.DrawModel(&m_field_object[i].transform);
	}*/
	m_field_object[tree1].model.DrawModel(&m_field_object[tree1].transform);
	m_field_object[tree1].box_hit.Draw(255,255);
}

//---------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------
void Field::Exit()
{

}
