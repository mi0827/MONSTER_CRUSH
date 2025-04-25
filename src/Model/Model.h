#pragma once
#include "src/System/Transform.h"

//! @file Model.h
//! @ brief ���f���p�̃N���X
class Model
{
public:
	//-----------
	// �֐�
	//-----------
	//! �R���X�g���N�^
	Model();
	//! �őc�g���N�^
	~Model();

	//! @brief ���f���ǂݍ��ݗp�֐�
	//! @param (data[256]) ���f���f�[�^�̃p�X
	//! @detail ���f����ǂݍ��ނ��߂̊֐�

	void LoadModel(const char data[256]);


	//! @brief ���f���`��p�̊֐�
	//! @detail ���f����`�悷�邽�߂̊֐�
	//!
	void DrawModel(Transform* tansform);


	//! @brief ���f���̍��W��������Ă���p�֐�
	//! @return ���݂̃��f���̍��W
	//! @detail ���݂̍��W�̃��f����m�邽�ߗp�֐�

	Vector3 GetModelPos();


	//! @brief ���f���̍��W���Z�b�g����p�֐�
	//! @param �Z�b�g���������W
	//! @detail �Z�b�g���������W�����f�����ɃZ�b�g����p�֐�

	void SetModelPos(Vector3 pos);


	//! @brief ���f���̌�����������Ă���p�̊֐�
	//! @return ���݂ɂ̃��f���̌���
	//! @detail ���݂̃��f���̌�����m�邽�߂̗p�̊֐�

	Vector3 GetModelRot(Vector3 rot);


	//! @brief ���f���Ɍ������Z�b�g����p�֐�
	//! @param �Z�b�g����������
	//! @detail �Z�b�g���������������f�����ɃZ�b�g����p�֐�

	void SetModelRot(Vector3 rot);


	//! @brief ���f���̃X�P�[�����Z�b�g����p�֐�
	//! @param �Z�b�g�������X�P�[��
	//! @detail �Z�b�g�������X�P�[�������f�����ɃZ�b�g����p�֐�
	void SetModelScale(Vector3 scale);

	//! @brief ���f���̃m�[�h�̃��[�J�����W�����[���h���W�ɒ����֐�
	//! @param �m�[�h�C���f�N�X
	Vector3 GetNodePos(int node_index);

	//! @brief ���f���̃m�[�h�̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����֐�
	//! @param �m�[�h�C���f�N�X
	MATRIX GetNodeWorldMatrix(int node_index);

	//! @brief ���f���̃m�[�h�̉�]���̏����K�����邽�߂̊֐�
	//! @param �m�[�h�C���f�N�X
	Vector3 GetNodeRot(int node_index);

	//---------------------------------
	// �}�e���A���̊e�ݒ�p�֐�
	//---------------------------------
	//! @brief �}�e���A���̊e�����m�ۂ���
	void GetMaterilaInfo();

	//! @brief �}�e���A����Diffuse�̐ݒ�p�֐�
	//! @param �ݒ肵�����J���[�i0.0f �` 1.0f�j
	void SetMaterialDiffuse(COLOR_F color);

	//! @brief �}�e���A����Ambient�̐ݒ�p�֐�
	//! @param �ݒ肵�����J���[�i0.0f �` 1.0f�j
	void SetMaterialAmbient(COLOR_F color);

	//! @brief �}�e���A����Specular�̐ݒ�p�֐�
	//! @param �ݒ肵�����J���[�i0.0f �` 1.0f�j
	void SetMaterialSpecular(COLOR_F color);

	//! @brief �}�e���A����Emissive�̐ݒ�p�֐�
	//! @param �ݒ肵�����J���[�i0.0f �` 1.0f�j
    void SetMaterialEmissive(COLOR_F color);

	//! @brief �}�e���A���̃X�y�L�����n�C���C�g�̑N���x�̐ݒ�p�֐�
	//! @param �ݒ肵���������i0.0f �` 1.0f�j
	void SetMaterialPower(float power);

	//! @brief ���f����Y��]�ɐ���������֐�
	void RotationYLimits();



public:

	// �ϐ�

	//! ���f���p�̕ϐ�
	int m_model;
	//! ���f���̍��W�A�����A�T�C�Y
	Transform m_transform;

private:

	//! �}�e���A���̍ő吔��ۑ�����
	int m_material_max;
	//! ���f���̃}�e���A�����
	std::vector<MATERIALPARAM> m_material;



};