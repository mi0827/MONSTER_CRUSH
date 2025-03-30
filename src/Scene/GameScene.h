#pragma once
// #include "Character_Base.h"

//! @file GameScene.h
//! @brief ���C���̃Q�[���V�[���N���X
class GameScene :public Scene_Base
{
public:
	//! @brief �R���X�g���N�^
	GameScene();
	//! @brief �f�X�g���N�^
	~GameScene();

	//! �Q�[���V�[���ɂ����g����������
	void GameSceneInit();

	//! @brief ��������
	void Init()override;

	//! @brief �X�V����
	void Update()override;

	//! @brief �����X�^�[�̓o�ꉉ�o
	void EntryUpdate();

	//! @brief �o�g���̍X�V����
	void GameUpdate();
	//! @brief �o�g����ʂ��I������Ƃ��̏���
	void EndUpdate();

	//! @brief  �`�揈��
	void Draw()override;

	//! @brief �I������
	void Exit()override;

	//! @brief �X�e�[�^�X�o�[�̕`��
	void StatusDraw();

	// �����_�ŕK�v���킩��Ȃ�
	//// SE�̏�����
	//void SE_Init();
	//// BGM�̏�����
	//void BGM_Init();

	// ���C�g�̏�����
	void LightInit();

	//! @brief �t�B�[���h�ƃL�����N�^�[�Ƃ̓����蔻��
	void HitField()override;

	//! @brief �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
	//! //! @param BGM�̉���
	//! @param SE�̉���
	//! @param �}�E�X���x
	void OptionValuesReflect(int bgm, int se, int mouse)override;

	//! @brief �L�����N�^�[�̏�������
	void CharacterInit();

	//! @brief �L�����N�^�[�̍X�V����
	void CharacterUpdate();

	//! @brief �L�����N�^�[�̍U���Ɋւ��Ă̍X�V����
	void AttackUpdate();

	//! @brief ���s���t�����炻���m�点�郁�b�Z�[�W��`�悷�邽�߂̊֐�
	void VDMessage();

	////! @brief �����X�^�[�̙��K�U���̍ۂɃv���C���[���������邽�߂̏���
	////! @param �����X�^�[�̙��K���؂̃t���O
	////! @param �����X�^�[�̍��W�ƌ���
	////! @param �����������L�����̌����ƍ��W
	////! @param �L�����N�^�[�̈ړ��X�s�[�h
	//void CharacterKeepAway(bool moster_roar_flag, Transform monster,Transform character,float character_move_speed);

	//! @brief �����X�^�[�̙��K�U���̍ۂɃv���C���[���������邽�߂̏���
	void CharacterKeepAway();

public:

	//! �J�����N���X�̃I�u�W�F�N�g
	Camera camera;

	//! �v���C���[�L�����p�̃I�u�W�F�N�g
	// CharacterBase* player;

	//! �����X�^�[�p�̃I�u�W�F�N�g
	MonsterBase* monster;

	//// �t�B�[���h�I�u�W�F�N�g
	//Field field;

	// �q�b�g�X�g�b�v�p�̃I�u�W�F�N�g
	HitStop hit_stop;

	//=================
	// �J�����Ɋւ��Ă̕ϐ�
	//! �J�����̈ړ��X�s�[�h

	//! �^�[�Q�b�g�J�������g�p���邩�}�E�X�ŃJ�����𑀍삷�邩�̔��f�p�ϐ�
	bool m_camera_change = true;

	static constexpr float CAMERA_ROT_SPEED = 2.0f;
	//! �J�����̉�]�������
	static constexpr int CAMERA_DIRECTIN_RIGHT = 0; // �E��]
	static constexpr int CAMERA_DIRECTIN_FLET = 1;   // ����]
	// �ǂ��炪���񂾂����킩��₷���Ǘ�
	enum Ditection
	{
		player_die,     // �v���C���[�����񂾂Ƃ� , ����
		monster_die,  // �����X�^�[�����񂾂Ƃ� , ����

		max,
	};
	// �N�����񂾂���ۑ����邽�߂̂���
	int m_who_died = 0;


	//==========================
	// �o�g���V�[���������łȂ����Ɋւ��Ă̕ϐ�
	// �ǂ̃V�[�������킩��₷���ۑ�
	enum WhatScene
	{
		entry, // �o�ꉉ�o(�����X�^�[)
		battle, // �o�g�����
		power_up, // �p���[�A�b�v(�����X�^�[)
		result, // �o�g�����I�������̏��
	};
	//! �ǂ̃o�g���V�[������ۑ����邽�߂̂���
	int m_what_scene = entry; //!< �ŏ��̓o�g���V�[������n�߂�(����ύX�\��)
	//! ��b������̃t���[���̐�
	static constexpr int ONE_SECOND_FLAME = 60;
	int m_count_flame = 0; // �t���[���J�E���g�p�̕ϐ� 
	int m_count_time = 0;  // �t���[��������ۂ̎��Ԃ�����o���ē����p�̎���


	// �o�g�����I����Ă���ǂꂭ�炢�̎��ԂŎ��̃V�[���Ɉړ����邩�̎���
	static constexpr int CHANGE_TIME = 3;

	//================================
	//  �o�g�����I�������ɕ`�悷�郁�b�Z�[�W�Ɋւ���ϐ�
	struct Message
	{
		//! ���b�Z�[�W������ϐ�
		const char* message;
	};
	Message m_massage[max]
	{
		{ "QUEST : FAILED"}, // ���s
		{ "QUEST : CLEAR" } // �N���A

	};
	//! ���b�Z�[�W�̕`����W
	Vector2 m_massage_pos{ 0.0f,0.0f };
};