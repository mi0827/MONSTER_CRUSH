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
	//! //! @param 移動の際のターゲット座標
	virtual void Init() = 0;
	//! @brief 描画処理
	virtual void Draw() = 0;
	//! @brief 更新処理
	virtual void Update(Transform* traget_pos, float traget_r) = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;

	
	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ステータスバーの設定用関数
	virtual void Status_Bar_Init() = 0;
	//! @brief ステータスバーの描画関数
	virtual void Status_Bar_Draw() = 0;

	//! @brief ベースクラスでの初期処理
	void BaseInit(Transform* target_pos, const  float m_target_hit_r);

	//! @brief ベースクラスの更新処理

	void BaseUpdate(bool* run_flag);

	//! @brief キャラの壁擦り判定用の関数
	//! @param キャラの座標
	//! @param キャラの動く前の座標
	//! @param キャラの当たり判定のサイズ
	//! @param 当たり判定相手のボックスの情報
	//void MoveHitUpdate(Vector3* monster_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box);
public:
	//-----------------------------------------------
	//! クラスのオブジェクトを宣言
	//-----------------------------------------------


    //! 自身の情報を入れる変数
	Transform m_transform;
	// 移動に使うクラス
	TargetMove move;

	
	//-----------------------------------------------
	//! 変数を宣言
	//-----------------------------------------------

	//! 自身の半径を入れる用の変数
	float m_hit_r = 10.0f;
	//! 自身の移動スピード
	static constexpr float M_MOV_SPEED = 1.0f;
	//! 自身回転速度
	static constexpr float M_ROT_SPEED = 5.0f;

	
};
