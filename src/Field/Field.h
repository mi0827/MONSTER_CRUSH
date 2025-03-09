#pragma once


//! @file Field.h
//! @brief	Field(フィールド)クラス
//! @brief フィールドベースクラスを継承
class Field :public FieldBase
{
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
		tree9,
		tree10,
		tree11,
		tree12,
		tree13, // フィールドの周りを囲んでいる木の番号

		// 外の見た目の部分の木
		// ここから下の木には当たり判定はいらない
		tree14,
		tree15,
		tree16,
		tree17,
		tree18,
		tree19,
		tree20,
		tree21,
		tree22,
		tree23,
		tree24,
		tree25,
		tree26,
		tree27,



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



		// 石のモデル番号
		stone1,
		stone2,
		stone3,
		stone4,
		stone5,


		MODEL_MAX
	};

	//! フェンスの数(当たり判定がいる数)
	static constexpr int FENCE_MAX = fence9 - fence1 + 1;
	//! フェンスの配列番号が始まる番号
	static constexpr int FENCE_INDEX_STRAT = fence1;
	//! 木の数(当たり判定がいる数)
	static constexpr int TREE_MAX = tree13 - tree1 + 1;
	//! 木の配列番号が始まる番号
	static constexpr int TREE_INDEX_STRAT = tree1;
	//! 石の数(当たり判定がいる数)
	static constexpr int STONE_MAX = stone5 - stone1 + 1;
	//! 石の配列番号が始まる番号
	static constexpr int STONE_INDEX_STRAT = stone1;

	//---------------
	// 関数の定義
	//---------------
	// コンストラクタ
	Field();
	// デストラクタ
	~Field();





	//! @brief 初期処理
	void Init()override;


	//! @brief 更新処理
	void Update()override;
	//! @brief 描画処理
	void Draw()override;
	//! @brief 終了処理
	void Exit()override;

	//! @brief オブジェクトの座標、サイズの初期設定
	void ObjectInit()override;
	//! @brief 木のオブジェクトの初期設定関数
	void TreeInit();
	//! @brief フェンスのオブジェクトの初期設定
	void FenceInit();
	//! @brief 石のオブジェクトの初期設定
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
	};

	// 置かれるオブジェクトの数分用意する
	field m_field_object[MODEL_MAX];

	//! フェンス用のあたり判定
	BoxCollision m_hit_fence[FENCE_MAX];

	//! 木の当たり判定
	CapsuleCollision m_hit_tree[TREE_MAX];

	//! 石の当たり判定
	BoxCollision m_hit_stone[STONE_MAX];

	//! フィールドを囲む四角の当たり判定
	static constexpr int FIELD_HIT_MAX = 4;
	BoxCollision m_hit_field[FIELD_HIT_MAX];

private:
	//! モデルのパスのテキスト
	InfoText m_model_data_path;
	//! モデルの座標の情報
	InfoText m_object_pos_info;
	//! モデルの向きの情報
	InfoText m_object_rot_info;
	//! モデルのサイズの設定
	InfoText m_object_scale_info; 
};
