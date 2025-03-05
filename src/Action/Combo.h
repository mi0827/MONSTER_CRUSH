#pragma once

//! @file Combo.h
//! @brief �R���{�p�̃N���X
class Combo
{
public:
	//! @brief �R���X�g���N�^
	Combo();
	//! @brief �f�X�g���N�^
	~Combo();

	//! @brief �R���{���Ă悢�̂����w��̃L�[����Ŕ��f����֐�
	//! @param �R���{�����Ă悢���̃t���O
	//! @param �}�E�X�̂ǂ������������������
	//! @param ������̃L�[�i�}�E�X�̃{�^���ł��j
	//! @param �Q�[���p�b�h�̓���̃{�^��
	//! @param �R���{�������Ƃ��̃A�j���[�V�����̍Đ�����
	//! @param �R���{�������Ƃ��̃A�j���[�V�����̈�����̎���
	//! @detail �f�t�H���g�A�j���[�V�����̃Z�b�g�A�A�j���[�V�����̓��ꕨ�̊m��
	void ComboJudgmentKey(bool* combo_flag, int* mouse_flag, int key, PAD_ID pad_button_no, float play_anim_time, float anim_total,int *combo_count);


	//! @brief �R���{���Ă悢�̂����w��̏����Ŕ��f����֐�
	//! @param �R���{�����Ă悢���̃t���O
	//! @param �w������̃t���O�ibool�j
	//! @param �R���{�������Ƃ��̃A�j���[�V�����̍Đ�����
	//! @param �R���{�������Ƃ��̃A�j���[�V�����̈�����̎���
	//! @detail �f�t�H���g�A�j���[�V�����̃Z�b�g�A�A�j���[�V�����̓��ꕨ�̊m��
	void ComboJudgmentCondition(bool* combo_flag, bool condition, float play_anim_time, float anim_total);


	//! @brief �R���{���[�h�̐؂�ւ��p�֐�
	//! @param �ݒ肵�����R���{���[�h
	void ChangeComboMode(int combo_mode);

	//! @breif �R���{�����n�߂Ă������𔻒f����֐�
	void ComboStartJudgent();

	//! @brief �R���{�𑱂��Ă�����
	bool  ComboCanContinueJudgent();

	enum ComboMode
	{
		COMBO_START,    // �R���{�X�^�[�g
		COMBO_STANDBY,  // �R���{�ҋ@��
		COMBP_IN,       // �R���{��
		COMBO_FINISH,   // �R���{�I��
	};
	//ComboMode m_combo_mode;
	int m_combo_mode;
};
