#pragma once
#include "src/System/InfoText.h"

//! @file Field.h
//! @brief	Field(�t�B�[���h)�N���X
//! @brief �t�B�[���h�x�[�X�N���X���p��
class Field :public FieldBase
{
public:
	// ���f���̕ۑ��ԍ�
	enum
	{
		// �؂̃��f���ԍ�
		tree1,
		tree2,
		tree3,
		tree4,
		tree5,
		tree6,
		tree7,
		tree8,
		tree9,
		tree10,
		tree11,
		tree12,
		tree13, 
		tree14,
		tree15,
		tree16,
		tree17,
		tree18,
		tree19,
		tree20,
		tree21,
		tree22,
		tree23,
		tree24,

		// �΂̃��f���ԍ�
		stone1,
		stone2,
		stone3,
		stone4,
		stone5,
		stone6,
		stone7,
		stone8,
		stone9,
		stone10,
		stone11,
		stone12,
		stone13,
		stone14,
		stone15,
		stone16,

		// �t�B�[���h�̂̃I�u�W�F�N�g�̍ő吔
		MODEL_MAX
	};

	//---------------
	// �֐��̒�`
	//---------------
	// �R���X�g���N�^
	Field();
	// �f�X�g���N�^
	~Field();
	//! @brief ��������
	void Init()override;


	//! @brief �X�V����
	void Update()override;

	//! @brief �`�揈��
	//! @param �J�������W
	//! @param �J�����ƃv���C���[�Ƃ̋���
	//! @param �v���C���[�̍��W
	void Draw(Vector3 camera_pos, float camera_length, Vector3 player_pos)override;

	//! @brief �I������
	void Exit()override;

	//! @brief �I�u�W�F�N�g�̍��W�A�T�C�Y�̏����ݒ�
	void ObjectInit()override;
	;

	//---------------
	// �ϐ��̒�`
	//---------------

	//--------------- 
	// �e�N���X�̃I�u�W�F�N�g�̍쐬
	//---------------
	// �t�B�[���h�̃I�u�W�F�N�g
	struct field
	{
		Model model;
		Transform transform;
	};

	//! �u�����I�u�W�F�N�g�̐����p�ӂ���
	field m_field_object[MODEL_MAX];

	//! �ǂ̕��p�ӂ���
	static constexpr int WALL_NUM_MAX = 4;
	//! �Ǎۂ̓����蔻��p�̃{�b�N�X
	BoxCollision m_hit_wall[WALL_NUM_MAX];

private:
	// �t�B�[���h�ɑ��݂��郂�f���̎��
	enum ModelNum
	{
		field, // �t�B�[���h�̃��f��
		terr,  // �؂̃��f��
		fence, // �t�F���X�̃��f��
		stone, // �΂̃��f��
		model_num_max
	};

	//! ���f���̃p�X�̃e�L�X�g
	InfoText m_model_data_path;
	//! ���f���̍��W�̏��
	InfoText m_object_pos_info;
	//! ���f���̌����̏��
	InfoText m_object_rot_info;
	//! ���f���̃T�C�Y�̐ݒ�
	InfoText m_object_scale_info;
};
