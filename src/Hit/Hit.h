#pragma once

//! @brief 球同士の当たり判定
//! @param 球の座標１
//! @param 球の半径１
//! @param 球の座標２
//! @param 球の半径２
//! @return true : 当たった、false : 当たっていない
bool CheckBallHit(Vector3 center1, float radius1, Vector3 center2, float radius2);

//! @brief ボックス同士の当たり判定
//! @param ボックス１の座標　（当たり判定用の座標）
//! @param ボックス１のサイズ　（中心から見た時のサイズ）
//! @param ボックス２の座標　（当たり判定用の座標）
//! @param ボックス２のサイズ　（中心から見た時のサイズ）
//! @detail 壁擦り判定に使う
//! @return true : 当たった、false : 当たっていない
bool CheckBoxHit3D(Vector3 box_pos1, Vector3 box_size1, Vector3 box_pos2, Vector3 box_size2);

//! @brief ボックス同士の当たり判定
//! @param ボックス１の情報
//! @param ボックス２の情報
//! @return true : 当たった、false : 当たっていない
bool CheckBoxHit(BoxCollision box1, BoxCollision box2);

//! @brief カプセル同士のあたり判定
//! @param カプセル１の情報
//! @param カプセル２の情報
//! @return true : 当たった、false : 当たっていない
bool CheckCapsuleHit(CapsuleCollision capsule1, CapsuleCollision capsule2);

//! @brief 円同士の当たり判定
//! @param 円１の座標
//! @param 円１の半径
//! @param 円２の座標
//! @param 円２の半径
//! @return true : 当たった、false : 当たっていない
bool CheckCircleHit(Vector3 pos1, int radius1, Vector3 pos2, int radius2);