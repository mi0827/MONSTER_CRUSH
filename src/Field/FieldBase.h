#pragma once

//! @file FieldBase.h
//! @brief �t�B�[���h�̃x�[�X�N���X
class FieldBase
{
public:
	//! @brief ��������
	virtual void Init() = 0;

	//! @brief �X�V����
	virtual void Update() = 0;

	//! @brief �`�揈��
	//! @param �J�������W
	//! @param �J�����ƃv���C���[�Ƃ̋���
	//! @param �v���C���[�̍��W
	virtual void Draw(Vector3 camera_pos, float camera_length, Vector3 player_pos) = 0;

	//! @brief �I������
	virtual void Exit() = 0;


	//! @brief �I�u�W�F�N�g�̍��W�A�T�C�Y�̏����ݒ�
	virtual void ObjectInit() = 0;

	

	//! @brief �I�u�W�F�N�g���J�����ƃv���C���[�ƃJ�����̊Ԃɂ��邩�𒲂ׂ�֐�
	//! @param �J�����̍��W
	//! @param �J��������̔��a
	//! @param �v���C���[�̍��W
	//! @param �I�u�W�F�N�g�̍��W
	//! @return true : ���ʂɕ`�悵�Ă悢�Afalse : �������ɕ`�悷��K�v��������
	bool ObjectDrawSituation(Vector3 camera_pos, float camera_radius, Vector3 player_pos, Vector3 obj_pos);

	//�t�B�[���h���f���̓��ꕨ
	Model m_field_model;
	Transform m_field_transform;

};