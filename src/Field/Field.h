#pragma once



//	Field(フィールド)クラス
class Field
{

private:
	//---------------
	// 定数
	//---------------
	const float MODEL_SIZE = 25.0f; // モデルサイズ

public:
	// モデルの保存番号
	enum
	{
		// 木のモデル番号
		tree1,
		tree2,
		tree3,
		tree4,
		tree5,
		tree6,
		tree7,
		tree8,


		// フェンスのモデル番号
		fence1,
		fence2,
		fence3,
		fence4,
		fence5,
		fence6,
		fence7,
		fence8,
		fence9,
		fence10,
		fence11,
		fence12,
		fence13,
		fence14,
		fence15,
		fence16,
		fence17,
		fence18,
		fence19,
		fence20,

		// 石のモデル番号
		stone1,


		MODEL_MAX
	};

	//! フェンスの数
	static constexpr int FENCE_MAX = fence20 - fence1 + 1;

	//! 木の数
	static constexpr int TREE_MAX = tree8 - tree1 + 1;

	//! 石の数
	static constexpr int STONE_MAX = stone1 - stone1 + 1;

	//---------------
	// 関数の定義
	//---------------
	// コンストラクタ
	Field();
	// デストラクタ
	~Field();





	//! @brief 初期処理
	void Init();


	//! @brief 更新処理
	void Update();
	//! @brief 描画処理
	void Draw();
	//! @brief 終了処理
	void Exit();

	//! @brinf オブジェクトの座標、サイズの初期設定
	void ObjectInit();
	//! @brinf 木のオブジェクトの初期設定関数
	void TreeInit();
	//! @brinf フェンスのオブジェクトの初期設定
	void FenceInit();
	//! @brinf 石のオブジェクトの初期設定
	void StoneInit();

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

	//! フェンス用のあたり判定
	BoxCollision m_hit_fence[FENCE_MAX];

	//! 木の当たり判定
	CapsuleCollision m_hit_tree[TREE_MAX];

	//! 石の当たり判定
	CapsuleCollision m_hit_stone[STONE_MAX];

	//フィールドモデルの入れ物
	Model m_field_model;
	Transform m_field_transform;
};
