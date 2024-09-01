#pragma once


//! @brinf 地面モデルの当たり判定の設定 
void SetHitGround(Model* ground_model);


//! @brinf 地面モデルとキャラクターモデルのモデル同士の当たり判定
//! @param キャラクターの座標
//! @param 地面のモデル
void HitGroundCharacter(Vector3* charcter_pos, Model*  ground_model);