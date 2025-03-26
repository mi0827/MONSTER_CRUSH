#pragma once
#include "src/System/InfoText.h"

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

		// 石のモデル番号
		stone1,
		stone2,
		stone3,
		stone4,
		stone5,
		stone6,
		stone7,
		stone8,
		stone9,
		stone10,
		stone11,
		stone12,
		stone13,
		stone14,
		stone15,
		stone16,

		// フィールドののオブジェクトの最大数
		MODEL_MAX
	};

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
	//! @param カメラ座標
	//! @param カメラとプレイヤーとの距離
	//! @param プレイヤーの座標
	void Draw(Vector3 camera_pos, float camera_length, Vector3 player_pos)override;

	//! @brief 終了処理
	void Exit()override;

	//! @brief オブジェクトの座標、サイズの初期設定
	void ObjectInit()override;
	;

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

	//! 置かれるオブジェクトの数分用意する
	field m_field_object[MODEL_MAX];

	//! 壁の分用意する
	static constexpr int WALL_NUM_MAX = 4;
	//! 壁際の当たり判定用のボックス
	BoxCollision m_hit_wall[WALL_NUM_MAX];

private:
	// フィールドに存在するモデルの種類
	enum ModelNum
	{
		field, // フィールドのモデル
		terr,  // 木のモデル
		fence, // フェンスのモデル
		stone, // 石のモデル
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
