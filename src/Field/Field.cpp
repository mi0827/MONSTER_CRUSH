#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
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
	// フィールド設定
	m_field_transform.pos.set(0, 0, 0);
	m_field_transform.scale.set(0.1, 0.1, 0.1);
	//m_field_object[0].model.LoadModel();
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
}

//---------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------
void Field::Exit()
{

}
