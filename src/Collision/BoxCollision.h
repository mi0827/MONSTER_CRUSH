#pragma once
//! @fule BoxCollision.h
//! @brief ボックスコンポネントのクラス 


class BoxCollision
{
public:
	//! @brief コンストラクタ
	BoxCollision();
	//! @brief デストラクタ
	~BoxCollision();

	//! @brief ボックスを描画する関数
	void Draw(int color, int line_color);

	//! @biref ボックスを作る関数
	//! @param 設置したいBOXの中心座標
	//! @param 設定したいサイズ
	void CreateBox(Vector3 pos, Vector3 size);

	//! @brief 座標の設定
	void SetPos(Vector3 pos);

	//! @biref サイズ設定
	//! @param 設定したイサイズ
	void SetSize(Vector3 size);

public:
	// ボックスの基本的な情報
	struct Box
	{
		Vector3 pos1 = { 0.0f,0.0f,0.0f };          //!< 座標１
		Vector3 pos2 = { 0.0f,0.0f,0.0f };		   //!< 座標２
		Vector3 size = { 0.0f ,0.0f,0.0f };		   //!< ボックスのサイズ

		// 当たり判定はボックスの真ん中から半分のサイズ分ずらして判定をとる
		Vector3 half_size = { 0.0f ,0.0f,0.0f };   //!< 当たり判定で使うために半分のサイズ
		Vector3 hit_pos= { 0.0f ,0.0f,0.0f };	   //!< 当たり判定用の座標
	
		int   color = 0;         // 立方体の色
		int   line_color = 0;         // ラインの色
	};
	Box m_box;
};