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

	//! 
	//! @brief 移動全体の更新処理
	//! @param このクラスを使って移動する本体のtransform
	//! @param 移動スピード
	//! @param 振り向き速度
	//! @param ターゲットにしたいものの座標
	void Update(Transform* transform, const float mov_speed, const float rot_speed, Vector3* target_pos);

	//!
	//! @fn
	//! @brief 移動に関する当たり判定用の関数
	//! @param 自分 (キャラ) の座標(今の座標)
	//! @param 自分（キャラ）の座標(当たり判定前の)
	//! @param 自分（キャラ）の当たり判定のサイズ（中心から見たサイズ）
	//! @param 当たり判定をとりたいボックスの情報
	//! @detail 移動の壁擦り用関数
	void Move_Hit(Vector3* pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box);
};
