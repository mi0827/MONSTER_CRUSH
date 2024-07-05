#pragma once

//! @brief 球同士の当たり判定
//! @param 球の座標１
//! @param 球の半径１
//! @param 球の座標２
//! @param 球の半径２
bool CheckBallHit(Vector3 center1, float radius1, Vector3 center2, float radius2);


//! @brief ボックス同士の当たり判定
//! @param ボックス１の座標　（当たり判定用の座標）
//! @param ボックス１のサイズ　（中心から見た時のサイズ）
//! @param ボックス２の座標　（当たり判定用の座標）
//! @param ボックス２のサイズ　（中心から見た時のサイズ）
//! @detail 壁擦り判定に使う
bool CheckBoxHit3D(Vector3 box_pos1, Vector3 box_size1, Vector3 box_pos2, Vector3 box_size2);

//! @brief ボックス同士の当たり判定
//! @param ボックス１の情報
//! @param ボックス２の情報
bool CheckBoxHit(BoxCollision box1, BoxCollision box2);

//! @brief カプセル同士のあたり判定
//! @param カプセル１の情報
//! @param カプセル２の情報
bool CheckCapsuleHit(CapsuleCollision capsule1, CapsuleCollision capsule2);