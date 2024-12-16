#pragma once

//! @file Receptionist.h
//! @brief 受付嬢のクラス

class Receptionist
{
public:

	static constexpr float BDOY_R = 5.0f;


	//! @brief コンストラクタ
	Receptionist();
	//! @brief デストラクタ
	~Receptionist();

    //! @brief 初期設定
	void Init();

	//! @brief 更新処理
	void Update();

	//! @brief 描画処理
	void Draw();

	//! @brief 終了処理
	void Exit();

	// アニメーションの種類用の列挙体
	enum Animation_Max
	{
		idle, // アイドルアニメーション
		idle2,
		anim_max // アニメーションの最大数
	};


	//! 座標、向き、大きさ
	Transform m_transform;

	//!モデルクラス
	Model m_model;

	//! アニメーションクラス
	Animation m_animation;

	//! カプセルの当たり判定の設定
	CapsuleCollision m_hit_body;
	
};