#pragma once

//! @fule CapsuleCollision.h
//! @brief カプセルコンポーネントのクラス


class CapsuleCollision
{
public:
	//! @brief コンストラクタ
	CapsuleCollision();
	//! @brief デストラクタ
	~CapsuleCollision();

	//! @brief カプセルを描画する関数
	void Draw();

	//! @brief モデルのノードの位置にカプセルをつける関数
	//! @param モデルクラス
	//! @param モデルのつけたい座標番号
	//! @param モデルのつけたい座標番号2
	//! @param 半径の大きさ
	void CreateNodoCapsule(Model* model, int node_index , int node_index2, float radius = 1);

	//! @brief カプセルを作る関数
	//! @param セットしたい座標(Vector3)
	//! @param セットしたい座標２(Vector3)
	//! @param セットしたい半径
	void CreateCapsule(Vector3 pos, Vector3 pos2, float radius=1);

	//! @brief カプセルのもう一つの座標をモデルのノードからもたってくる
	//! @param モデルの情報
	//! @param ノードのインデックス
	//! @param 半径
	void NodoSetSize(Model* model, int node_index, float radius);

	//! @brief カプセルのサイズを設定する関数
	//! @param 設定したいサイズ(Vector3)
	//! @param 半径(float)
	void SetSize(Vector3 pos, float radius);


public:
	// カプセルの基本的情報
	struct Capsule
	{
		Vector3 pos1 = { 0.0f, 0.0f, 0.0f }; //!< カプセルの座標１
		Vector3 pos2 = { 0.0f, 0.0f, 0.0f }; //!< カプセルの座標２
		float radius = 0.0f;             //!< カプセルの半径
	};
	Capsule m_capsule;
};
