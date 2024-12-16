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
	virtual void Draw() = 0;

	//! @brief 終了処理
	virtual void Exit() = 0;


	//! @brinf オブジェクトの座標、サイズの初期設定
	virtual void ObjectInit() = 0;


	//フィールドモデルの入れ物
	Model m_field_model;
	Transform m_field_transform;

};