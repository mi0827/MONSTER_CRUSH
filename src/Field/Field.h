#pragma once



//	Field(フィールド)クラス
class Field 
{

	// モデルの保存番号
	enum
	{
		// 木のモデル番号
		tree1,
		/*tree2,
		tree3,*/
		// フェンスのモデル番号
		fence1,

		// 石のモデル番号
		stone1,


		MODEL_MAX
	};

	//---------------
	// 定数
	//---------------


private:

	const float MODEL_SIZE = 25.0f; // モデルサイズ

public:
	//---------------
	// 関数の定義
	//---------------
	// コンストラクタ
	Field();	//	初期化処理
	void Init();             //　初期処理
	int  Field_Init();       // フィールド上にオブジェクトの置く数を返す関数
	void Object_Init();      // オブジェクトの座標、サイズの初期設定
	void Update();		     //	更新処理
	void Draw();		     //	描画処理
	void Exit();		     //	終了処理

	//---------------
	// 変数の定義
	//---------------

	//--------------- 
	// 各クラスのオブジェクトの作成
	//---------------
	// フィールドのオブジェクト
	struct field
	{
		Model model;
		Transform transform;
		// 当たり判定用のボックス
		BoxCollision box_hit;
	};
	// 置かれるオブジェクトの数分用意する
	field m_field_object[MODEL_MAX];

	//フィールドモデルの入れ物
	Model m_field_model;
	Transform m_field_transform;
};
