#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "Combo.h"

//---------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------
Combo::Combo()
{
}

//---------------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------------
Combo::~Combo()
{
}

//---------------------------------------------------------------------------
// �R���{���Ă悢���𔻒f����֐�
// �w��̃L�[�i�{�^���������ꂽ�Ƃ��j
//---------------------------------------------------------------------------

void Combo::ComboJudgmentKey(bool* combo_flag, int* mouse_flag, int key, PAD_ID pad_button_no, float play_anim_time, float anim_total, int* combo_count)
{
	// �R���{�t���O���������Ă�����
	if (*combo_flag == false)
	{
		// �w��̃}�E�X�{�^���������Ă�����
		if (PushMouseInput(key)|| IsPadOn(pad_button_no))
		{
			// �U���A�j���[�V�������I���O��������
			if (play_anim_time <= anim_total)
			{
				// �R���{�J�E���g�𑝂₷
				*combo_count += 1;
				// �ǂ̃{�^��������������ۑ�
				*mouse_flag = key;
				// �R���{�t���O���グ��
				*combo_flag = true;

				ChangeComboMode(COMBO_STANDBY);
			}
			
		}
	}
}

//---------------------------------------------------------------------------
// �R���{���Ă悢���𔻒f����֐�
// �w��̏����𖞂����Ă���Ƃ�
//---------------------------------------------------------------------------
void Combo::ComboJudgmentCondition(bool* combo_flag, bool condition, float play_anim_time, float anim_total)
{
	// �U���A�j���[�V�������I��鎞��
	if (play_anim_time == anim_total - 5.0f)
	{
		if (*combo_flag == false)
		{
			// �w��̏�����treu�̎�
			if (condition)
			{
				// �R���{�t���O���グ��
				*combo_flag = true;
				// �R���{�J�E���g�𑝂₷
			}
		}
	}
}

//---------------------------------------------------------------------------
// �R���{���[�h�̐؂�ւ��p�֐�
//---------------------------------------------------------------------------
void Combo::ChangeComboMode(int combo_mode)
{
	// �R���{���[�h�̕ύX
	m_combo_mode = combo_mode;
}

//---------------------------------------------------------------------------
// �R���{���n�߂Ă�������Ԃ��p�̊֐�
//---------------------------------------------------------------------------
void Combo::ComboStartJudgent()
{
	if (m_combo_mode == COMBO_FINISH)
	{
		m_combo_mode = COMBO_START;
	}
	
}


//---------------------------------------------------------------------------
// �R���{�̍X�V�𑱂��Ă�������Ԃ��֐�
//---------------------------------------------------------------------------
bool Combo::ComboCanContinueJudgent()
{
	// �R���{�t�B�j�b�V���ȊO�Ȃ�R���{�𑱂��Ă���
	if (m_combo_mode == COMBO_FINISH)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}


