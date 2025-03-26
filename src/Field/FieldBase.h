#pragma once

//! @file FieldBase.h
//! @brief フィールドのベースクラス
class FieldBase
{
public:
	//! @brief 初期処理
	virtual void Init() = 0;

	//! @brief 更新処理
	virtual void Update() = 0;

	//! @brief 描画処理
	//! @param カメラ座標
	//! @param カメラとプレイヤーとの距離
	//! @param プレイヤーの座標
	virtual void Draw(Vector3 camera_pos, float camera_length, Vector3 player_pos) = 0;

	//! @brief 終了処理
	virtual void Exit() = 0;


	//! @brief オブジェクトの座標、サイズの初期設定
	virtual void ObjectInit() = 0;

	

	//! @brief オブジェクトがカメラとプレイヤーとカメラの間にいるかを調べる関数
	//! @param カメラの座標
	//! @param カメラからの半径
	//! @param プレイヤーの座標
	//! @param オブジェクトの座標
	//! @return true : 普通に描画してよい、false : 半透明に描画する必要性がある
	bool ObjectDrawSituation(Vector3 camera_pos, float camera_radius, Vector3 player_pos, Vector3 obj_pos);

	//フィールドモデルの入れ物
	Model m_field_model;
	Transform m_field_transform;

};