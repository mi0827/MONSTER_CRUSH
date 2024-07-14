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
	// 関数の定義
	//=========================================================================

	//! @ターゲットの設定
	//! @param 自身の情報
	//! @param 円の半径
	void SetInfo(Transform* transform, const float hit_r ,const float MOV_SPEED,const float ROT_SPEED);

	//! @ターゲットの情報を設定
	//! @param ターゲットの座標
	//! @param 円の半径
	void SetTargetInfo(Transform* taregt_pos, const float target_hit_r);

	//! @brief 移動全体の更新処理
	//! @param このクラスを使って移動する本体のtransform
	//! @param 移動スピード
	//! @param 振り向き速度
	void Update();

	//! @brief ターゲットと一定の距離に入ったかを返す関数
	bool Target_Hit();

private:

	//=========================================================================
	// 構造体の定義
	//=========================================================================

	struct Information
	{
		//! 自身の情報を入れる変数
		Transform* m_transform;
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
		Transform* m_target;
		//! ターゲットの半径を入れる変数
		float m_target_hit_r;
	};


	//=========================================================================
	// 変数の宣言
	//=========================================================================
	//! 自身の情報を入れておくもの
	Information m_info;
	//! ターゲットの情報を入れておくもの
	TargetInformation m_target_info;

	//! 本人の向いている方向をわかりやすくするための座標
	Vector3 m_line_start;
	Vector3 m_line_goal;

	//=========================================================================
	// 定数の宣言
	//=========================================================================
	static constexpr int RANGE = 200;

};
