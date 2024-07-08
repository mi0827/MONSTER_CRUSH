#pragma once
// モンスター専用のキャラベースクラス

//! 
//! @fule MonsterBase.h
//! @brief モンスターのベースクラス


class MonsterBase
{
public:
	//! コンストラクタ
	MonsterBase();

	//! デストラクタ
	~MonsterBase();




	//! 純粋仮想関数
	//! @brief 初期処理
	virtual void Init() = 0;
	//! @brief 描画処理
	virtual void Draw() = 0;
	//! @brief 更新処理
	//! @param 移動の際のターゲット座標
	virtual void Update(Vector3* target_pos) = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;

	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ベースクラスでの初期処理
	void BaseInit();

	//! @brief ベースクラスの更新処理
	//! @param 移動スペード
	//! @param 回転速度
	//! @param 移動の際のターゲットの座標
	void BaseUpdate(const float mov_speed, const float rot_speed, Vector3* target_pos);


	//! @brief キャラの壁擦り判定用の関数
	//! @param キャラの座標
	//! @param キャラの動く前の座標
	//! @param キャラの当たり判定のサイズ
	//! @param 当たり判定相手のボックスの情報
	//void MoveHitUpdate(Vector3* monster_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box);
public:
	//-----------------------------------------------
	//! クラスのオブジェクトを定義
	//-----------------------------------------------
	Transform transform;
	TargetMove move;
	Spotlight spot_light;
};
