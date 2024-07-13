#pragma once
// 指定されたターゲットの方向に移動するためのクラス

//! @fule TargetMove.h
//! @brief 指定されたターゲット方向に移動するためのクラス

class TargetMove
{
public:
	//! コンストラクタ
	TargetMove();
	//! デストラクタ
	~TargetMove();

	//=========================================================================
	// 構造体の定義
	//=========================================================================

	struct Information
	{
		//! 自身の情報を入れる変数
		Transform m_transform;
		//! 自身の半径を入れる用の変数
		float m_hit_r;
		//! 自身の移動スピード
		float M_MOV_SPEED;
		//! 自身回転速度
		float M_ROT_SPEED;
	};


	struct TargetInformation
	{
		//! ターゲットの座標を入れる変数
		Vector3 m_target;
		//! ターゲットの半径を入れる変数
		float m_target_hit_r;
	};



	//=========================================================================
	// 関数の定義
	//=========================================================================

	//! @ターゲットの設定
	//! @param 自身の情報
	//! @param 円の半径
	void SetInfo(Transform* transform, const float hit_r);

	//! @ターゲットの情報を設定
	//! @param ターゲットの座標
	//! @param 円の半径
	void SetTargetInfo(Vector3* taregt_pos, const float target_hit_r);

	//! @brief 移動全体の更新処理
	//! @param このクラスを使って移動する本体のtransform
	//! @param 移動スピード
	//! @param 振り向き速度
	void Update();

	//!
	//! @fn
	//! @brief 移動に関する当たり判定用の関数
	//! @param 自分 (キャラ) の座標(今の座標)
	//! @param 自分（キャラ）の座標(当たり判定前の)
	//! @param 自分（キャラ）の当たり判定のサイズ（中心から見たサイズ）
	//! @param 当たり判定をとりたいボックスの情報
	//! @detail 移動の壁擦り用関数
	void Move_Hit(Vector3* pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box);


	//! @brief ターゲットとの距離によって移動をやめるための関数
	void Target_Hit();

private:

	//=========================================================================
	// 変数の定義
	//=========================================================================
	//! 自身の情報を入れておくもの
	Information m_info;
	//! ターゲットの情報を入れておくもの
	TargetInformation m_target_info;



};
