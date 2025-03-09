#pragma once


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
		tree13, // �t�B�[���h�̎�����͂�ł���؂̔ԍ�

		// �O�̌����ڂ̕����̖�
		// �������牺�̖؂ɂ͓����蔻��͂���Ȃ�
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
		tree25,
		tree26,
		tree27,



		// �t�F���X�̃��f���ԍ�
		fence1,
		fence2,
		fence3,
		fence4,
		fence5,
		fence6,
		fence7,
		fence8,
		fence9,



		// �΂̃��f���ԍ�
		stone1,
		stone2,
		stone3,
		stone4,
		stone5,


		MODEL_MAX
	};

	//! �t�F���X�̐�(�����蔻�肪���鐔)
	static constexpr int FENCE_MAX = fence9 - fence1 + 1;
	//! �t�F���X�̔z��ԍ����n�܂�ԍ�
	static constexpr int FENCE_INDEX_STRAT = fence1;
	//! �؂̐�(�����蔻�肪���鐔)
	static constexpr int TREE_MAX = tree13 - tree1 + 1;
	//! �؂̔z��ԍ����n�܂�ԍ�
	static constexpr int TREE_INDEX_STRAT = tree1;
	//! �΂̐�(�����蔻�肪���鐔)
	static constexpr int STONE_MAX = stone5 - stone1 + 1;
	//! �΂̔z��ԍ����n�܂�ԍ�
	static constexpr int STONE_INDEX_STRAT = stone1;

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
	void Draw()override;
	//! @brief �I������
	void Exit()override;

	//! @brief �I�u�W�F�N�g�̍��W�A�T�C�Y�̏����ݒ�
	void ObjectInit()override;
	//! @brief �؂̃I�u�W�F�N�g�̏����ݒ�֐�
	void TreeInit();
	//! @brief �t�F���X�̃I�u�W�F�N�g�̏����ݒ�
	void FenceInit();
	//! @brief �΂̃I�u�W�F�N�g�̏����ݒ�
	void StoneInit();

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

	// �u�����I�u�W�F�N�g�̐����p�ӂ���
	field m_field_object[MODEL_MAX];

	//! �t�F���X�p�̂����蔻��
	BoxCollision m_hit_fence[FENCE_MAX];

	//! �؂̓����蔻��
	CapsuleCollision m_hit_tree[TREE_MAX];

	//! �΂̓����蔻��
	BoxCollision m_hit_stone[STONE_MAX];

	//! �t�B�[���h���͂ގl�p�̓����蔻��
	static constexpr int FIELD_HIT_MAX = 4;
	BoxCollision m_hit_field[FIELD_HIT_MAX];

private:
	//! ���f���̃p�X�̃e�L�X�g
	InfoText m_model_data_path;
	//! ���f���̍��W�̏��
	InfoText m_object_pos_info;
	//! ���f���̌����̏��
	InfoText m_object_rot_info;
	//! ���f���̃T�C�Y�̐ݒ�
	InfoText m_object_scale_info; 
};
