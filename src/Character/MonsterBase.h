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
	virtual void Init(Vector3* traget_pos) = 0;
	//! @brief 描画処理
	virtual void Draw() = 0;
	//! @brief 更新処理
	virtual void Update() = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;

	//! @brief 当たり判定の更新処理
	virtual void CDUpdate() = 0;

	//! @brief ベースクラスでの初期処理
	void BaseInit();

	//! @brief ベースクラスの更新処理
	
	void BaseUpdate();

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
	Information* m_info;

	
	struct TargetInformation
	{
		//! ターゲットの座標を入れる変数
		Vector3* m_target;
		//! ターゲットの半径を入れる変数
		float m_target_hit_r;
	};
	TargetInformation targetinfo;


	TargetMove move;
	Spotlight spot_light;
};
