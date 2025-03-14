#pragma once
#include "src/System/Vector2.h"


//! @brief �_�Ɖ~�̓����蔻��
//! @param �_�̍��W
//! @param �~�̒��S�̍��W
//! @param �~�̔��a
//! @return true : ���������Afalse : �������Ă��Ȃ�
bool CheckPointCircleHit(Vector2& point, Vector2& center, float radius);


//! @brief �_�Ǝl�p�̓����蔻��
//! @param �_�̍��W
//! @param �{�b�N�X�̍��W
//! @param �{�b�N�X�̃T�C�Y
//! @return true : ���������Afalse : �������Ă��Ȃ�
bool CheckPointBoxHit(Vector2& point, Vector2& box_pos, Vector2& box_size);


//! @brief �����m�̓����蔻��
//! @param ���̍��W�P
//! @param ���̔��a�P
//! @param ���̍��W�Q
//! @param ���̔��a�Q
//! @return true : ���������Afalse : �������Ă��Ȃ�
bool CheckBallHit(Vector3 center1, float radius1, Vector3 center2, float radius2);

//! @brief �{�b�N�X���m�̓����蔻��
//! @param �{�b�N�X�P�̍��W�@�i�����蔻��p�̍��W�j
//! @param �{�b�N�X�P�̃T�C�Y�@�i���S���猩�����̃T�C�Y�j
//! @param �{�b�N�X�Q�̍��W�@�i�����蔻��p�̍��W�j
//! @param �{�b�N�X�Q�̃T�C�Y�@�i���S���猩�����̃T�C�Y�j
//! @detail �ǎC�蔻��Ɏg��
//! @return true : ���������Afalse : �������Ă��Ȃ�
bool CheckBoxHit3D(Vector3 box_pos1, Vector3 box_size1, Vector3 box_pos2, Vector3 box_size2);

//! @brief �{�b�N�X���m�̓����蔻��
//! @param �{�b�N�X�P�̏��
//! @param �{�b�N�X�Q�̏��
//! @return true : ���������Afalse : �������Ă��Ȃ�
bool CheckBoxHit(BoxCollision box1, BoxCollision box2);

//! @brief �J�v�Z�����m�̂����蔻��
//! @param �J�v�Z���P�̏��
//! @param �J�v�Z���Q�̏��
//! @return true : ���������Afalse : �������Ă��Ȃ�
bool CheckCapsuleHit(CapsuleCollision capsule1, CapsuleCollision capsule2);

//! @brief �~���m�̓����蔻��
//! @param �~�P�̍��W
//! @param �~�P�̔��a
//! @param �~�Q�̍��W
//! @param �~�Q�̔��a
//! @return true : ���������Afalse : �������Ă��Ȃ�
bool CheckCircleHit(Vector3 pos1, int radius1, Vector3 pos2, int radius2);