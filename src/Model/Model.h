#pragma once
#include "src/System/Transform.h"
/**
* @file Model.h
* @ brief モデル用のクラス
*/
class Model 
{
public:
	//-----------
	// 関数
	//-----------
	//! コンストラクタ
	Model( );
	//! で祖トラクタ
	~Model();

	//! @brief モデル読み込み用関数
	//! @param (data[256]) モデルデータのパス
	//! @detail モデルを読み込むための関数
	
	void LoadModel(const char data[256]);

	
	//! @brief モデル描画用の関数
	//! @detail モデルを描画するための関数
	//!
	void DrawModel(Transform* tansform);


	//! @brief モデルの座標をもらってくる用関数
	//! @return 現在のモデルの座標
	//! @detail 現在の座標のモデルを知るため用関数
	
	Vector3 GetModelPos();

	
	//! @brief モデルの座標をセットする用関数
	//! @param セットしたい座標
	//! @detail セットしたい座標をモデルをにセットする用関数
	
	void SetModelPos(Vector3 pos);

	
	//! @brief モデルの向きをもらってくる用の関数
	//! @return 現在にのモデルの向き
	//! @detail 現在のモデルの向きを知るための用の関数
	
	Vector3 GetModelRot(Vector3 rot);

	
	//! @brief モデルに向きをセットする用関数
	//! @param セットしたい向き
	//! @detail セットしたい向きをモデルをにセットする用関数
	
	void SetModelRot(Vector3 rot);

	
	//! @brief モデルのスケールをセットする用関数
	//! @param セットしたいスケール
	//! @detail セットしたいスケールをモデルをにセットする用関数
	void SetModelScale(Vector3 scale);

	//! @brief モデルのノードのローカル座標をワールド座標に直す関数
	//! @param ノードインデクス
	Vector3 GetNodePos(int node_index);
public:

	// 変数

	//! モデル用の変数
	int m_model;
	//! モデルの座標、向き、サイズ
	Transform m_transform;



};