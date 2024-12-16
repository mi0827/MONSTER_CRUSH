#pragma once


//! @brief 地面モデルの当たり判定の設定 
void SetHitGround(Model* ground_model);


//! @brief 地面モデルとキャラクターモデルのモデル同士の当たり判定
//! @param キャラクターの座標
//! @param 地面のモデル
void HitGroundCharacter(Vector3* character_pos, Model*  ground_model);

//! @brief 地面モデルとキャラクターモデルのモデル同士の当たり判定(ジャンプを行う場合)
//! @param キャラクターの座標
//! @param ジャンプに使うVector3
//! @param ジャンプフラグ
//! @param 上に上がるスピード
//! @param 下に落ちるスピード
//! @param 地面のモデル
void HitGroundCharacterJump(Vector3* character_pos, Vector3* jump_mov, bool* jump_flag, float up_speed, float down_speed, Model* ground_model);



//VECTOR GetGroundPos(Vector3* character_pos, Model* ground_model);