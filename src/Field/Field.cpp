#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/Base.h"
#include "src/Object/Object.h"
#include "src/Field/Field.h"


// フィールドのオブジェクトを置く座標 
constexpr int MAP_W = 10;  // 縦
constexpr int MAP_H = 10;  // 横

// 1が壁０が何もない
// フィールドのオブジェクトを置く座標用の二次元配列
int MapData[MAP_H][MAP_W]
{
	{ 2,2,2,2,2,2,2,2,2,2},
	{ 2,0,0,0,0,0,0,0,0,2},
	{ 2,0,0,1,0,0,1,0,0,2},
	{ 2,0,0,0,0,0,0,0,0,2},
	{ 2,0,0,1,0,0,1,0,0,2},
	{ 2,0,0,0,0,0,0,0,0,2},
	{ 2,0,0,1,0,0,1,0,0,2},
	{ 2,0,0,0,0,0,0,0,0,2},
	{ 2,0,0,1,0,0,1,0,0,2},
	{ 2,2,2,2,2,2,2,2,2,2},
};
//	初期化処理
Field::Field()
{
	//// ここでフィールドモデルの読み込みをする
	//m_model = MV1LoadModel("Data/Model/Field/z.mv1");
	m_pos.set(0 + MODEL_SIZE* 15,0.0f, 0 + MODEL_SIZE * 15); // 描画座標の設定
	m_rot.clear(); // 回転量の 

	m_wall_obj_max = 0;
	m_model = 0;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void Field::Init()
{
	// ここでフィールドモデルの読み込みをする
	m_model = MV1LoadModel("Data/Model/Field/Ground_2.mv1");
	m_obj_max = Field_Init();
	//wall_obj_max = Wall_Field_Init();
	// オブジェクトの初期設定
	for (int i = 0; i < m_obj_max; ++i)
	{
		// ここでNEWする
		Object* object = new Object;
		// 追加登録(これでオブジェクトクラスの配列が増えていく)
		objects.push_back(object);
	}

	
	Field_Init();  // フィールド上にオブジェクトの置く数を返す
	Object_Init(); // オブジェクトの座標、サイズの初期設定
}

//---------------------------------------------------------------------------
// フィールド上にオブジェクトの置く数を返す
//---------------------------------------------------------------------------
int Field::Field_Init()
{
	// 何個のオブジェクトが必要か数える
	int object_count = 0;
	for (int h = 0; h < MAP_H; h++) 
	{
		for (int w = 0; w < MAP_W; w++) 
		{
			if (MapData[h][w] != 0) 
			{
				object_count++;
			}
		}
	}
	return object_count; // 何個のオブジェクトが必要か返す
}

//---------------------------------------------------------------------------
// オブジェクトの座標、サイズの初期設定
//---------------------------------------------------------------------------
void Field::Object_Init()
{
	// 何個めオブジェクトか数える
	int object_count = 0;
	for (int h = 0; h < MAP_H; h++) 
	{
		for (int w = 0; w < MAP_W; w++)
		{
			if (MapData[h][w] == 1)
			{   // フィールドの上の置物
				// 最初の０個目を触るために上に置く
				objects[object_count]->Field_Object_Init(m_field_size, h, w);
				object_count++; // 次の分のカウントを進めておく
			}
			if (MapData[h][w] == 2)
			{   // 壁
				// 最初の2個目を触るために上に置く
				objects[object_count]->Field_Wall_Object_Init(m_wall_size, h, w);
				object_count++; // 次の分のカウントを進めてお
			}
		}
	}

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
	// フィールドモデルの描画座標
	MV1SetPosition(m_model, VGet(m_pos.x, m_pos.y, m_pos.z));
	// フィールドの回転
	MV1SetRotationXYZ(m_model, VGet(TO_RADIAN(m_rot.x), TO_RADIAN(m_rot.y), TO_RADIAN(m_rot.z)));
	// モデルの大きさ
	MV1SetScale(m_model, VGet(MODEL_SIZE, MODEL_SIZE, MODEL_SIZE));
	// モデルの描画
	MV1DrawModel(m_model);

	for (int i = 0; i < m_obj_max; i++) 
	{
		objects[i]->Draw();
	}
}

//---------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------
void Field::Exit()
{
	// モデルデータの削除
	MV1DeleteModel(m_model);

	// オブジェクトの配列の解放
	objects.clear();
	m_wall_objects.clear();
}
