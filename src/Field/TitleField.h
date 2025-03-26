#pragma once
//! @file TitleField.h
//! @brief  TitleField(タイトル画面のフィールド)クラス

class TitleField : public FieldBase
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
		tree13,
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
		fence10,
		fence11,
	
		// フィールドののオブジェクトの最大数
		MODEL_MAX
	};

	////! フェンスの数(当たり判定がいる数)
	//static constexpr int FENCE_MAX = fence11 - fence1 + 1;
	////! フェンスの配列番号が始まる番号
	//static constexpr int FENCE_INDEX_STRAT = fence1;
	//! 木の数(当たり判定がいる数)
	static constexpr int TREE_MAX = tree27 - tree1 + 1;
	//! 木の配列番号が始まる番号
	static constexpr int TREE_INDEX_STRAT = tree1;
	//! 石の数(当たり判定がいる数)
	//static constexpr int STONE_MAX = stone5 - stone1 + 1;
	//! 石の配列番号が始まる番号
	//static constexpr int STONE_INDEX_STRAT = stone1;

	//---------------
	// 関数の定義
	//---------------
	// コンストラクタ
	TitleField();
	// デストラクタ
	~TitleField();

	//! @brief 初期処理
	void Init()override;


	//! @brief 更新処理
	void Update()override;

	//! @brief 描画処理
	//! @param カメラ座標
	//! @param カメラとプレイヤーとの距離
	//! @param プレイヤーの座標
	void Draw(Vector3 camera_pos, float camera_length, Vector3 player_pos)override;

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

	
	//! 木の当たり判定
	CapsuleCollision m_hit_tree[TREE_MAX];

	//!  周りの当たり判定
	//! 周りを囲む当たり判定の数
	static constexpr int AROUND_MAX = 4;
	BoxCollision m_hit_around[AROUND_MAX]; // 当たり判定の箱
	Vector3 m_hit_around_pos[AROUND_MAX]; // 当たり判定の座標
	Vector3 m_hit_around_size[AROUND_MAX]; // 当たり判定のサイズ

	// フィールドに存在するモデルの種類
	enum ModelNum
	{
		field, // フィールドのモデル
		terr,  // 木のモデル
		fence, // フェンスのモデル
		
		model_num_max
	};

	//! モデルのパスのテキスト
	InfoText m_model_data_path;
	//! モデルの座標の情報
	InfoText m_object_pos_info;
	//! モデルの向きの情報
	InfoText m_object_rot_info;
	//! モデルのサイズの設定
	InfoText m_object_scale_info;
};