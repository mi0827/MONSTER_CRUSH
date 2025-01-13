#pragma once
// 指定されたターゲットの方向に移動するためのクラス

//! @file TargetMove.h
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
	//! @param 移動スピード
	//! @param 回転スピード
	void SetInfo(Transform* transform, const float hit_r, const float MOV_SPEED, const float ROT_SPEED);

	//! @ターゲットの情報を設定
	//! @param ターゲットの座標
	//! @param 円の半径
	void SetTargetInfo(Transform* taregt_pos, const float target_hit_r);

	//! @brief 移動全体の更新処理
	//! @param 移動中かそうでないかのフラグ
	void Update(bool* run_flag);

	//! @brief 本体の向きを
	void SetDirection();

	//! @brief キャラの向いている方向にターゲットがいるかいないかを返す関数
	//! @param 入っていてほしい範囲
	//! @return true : 範囲内, false : 範囲外
	bool WithinRange(int range);

	//! @brief ターゲットと一定の距離に入ったかを返す関数
	bool TargetHit();

	//! @brief ターゲットとの距離を返す関数
	float GetTargetDistance();

	// @brief 向きの変更をしてよいかのフラグを受け取る
	void SetCanRotate(bool can = true);

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
public:
	//! ターゲットの情報を入れておくもの
	TargetInformation m_target_info;

	//! 本人の向いている方向をわかりやすくするための座標
	Vector3 m_line_start;
	Vector3 m_line_goal;

	//=========================================================================
	// 定数の宣言
	//=========================================================================
	//! 振り向きを行ってほしい値
	static constexpr int RANGE = 500;

public:
	//! ターゲットとの距離が一定より近づいたかを知るための変数
	// 動いていていい状態:true
	// 動いてはならないとき:false
	bool m_hit = false;
private:
	// 外積を使った判断をしたいので上で作った２つのベクトルの外積を求めます
	Vector3 m_cross;
	// 回転していいかのフラグようの変数
	bool m_can_rot = true;
};
