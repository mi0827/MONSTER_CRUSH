#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/System/Move.h"
#include "src/Action/Combo.h"
#include "CharacterBase.h"

//---------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------
CharacterBase::CharacterBase()
{

}

//---------------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------------
CharacterBase::~CharacterBase()
{
	m_attack_hit_damage.clear();
}

//---------------------------------------------------------------------------
// �v���C���[�̈ړ��p�֐�
//---------------------------------------------------------------------------
void CharacterBase::MoveAction(int run_anim_no, const float move_speed, Vector3* camera_rot)
{
	// ���񃊃Z�b�g
	m_run_flag = false;

	//// �ړ��O�̍��W��U�ۑ����Ă���
	//m_before_pos = m_transform.pos;

	// �x�[�X�N���X�̍X�V����
	// �ړ��̏��������ɓ����Ă���
	BaseUpdate(&m_run_flag, camera_rot, &move_speed);

	// run_flag ���オ���Ă�Ƃ�����
	// �v���C���[���[�h��RUN�ȊO�̎�
	if (m_run_flag && m_player_mode != RUN)
	{
		// �A�j���[�V�����̐؂�ւ��t���O���グ��
		m_animation.m_anim_change_flag = true;
	}

	// �A�j���[�V�����ύX���\�Ȏ���
	if (m_animation.ChangeFlag(m_run_flag))
	{
		// ����A�j���[�V�����ɕύX
		m_animation.ChangeAnimation(&m_model, run_anim_no, true);
		// �A�j���[�V�������ς��������
		// �v���C���[���[�h�̐؂�ւ�������
		m_player_mode = RUN;
	}
	
}

//---------------------------------------------------------------------------
// �ړ��p�̂����蔻��
//---------------------------------------------------------------------------
void CharacterBase::MoveHitUpdate(BoxCollision* box)
{
	// �����蔻���
	m_move_hit_box.CreateBox(m_transform.pos, m_move_hit_size);
	m_move.MoveHit(&m_transform.pos, &m_before_pos, &m_move_hit_size, box);
}


//---------------------------------------------------------------------------
// �L�����N�^�[�̍��W�ݒ�
//---------------------------------------------------------------------------
void CharacterBase::SetCharacterPos(Vector3 pos)
{
	m_transform.pos.set(pos);
}


//---------------------------------------------------------------------------
// �L�����N�^�[�̌����̐ݒ�
//---------------------------------------------------------------------------
void CharacterBase::SetCharacterRot(Vector3 rot)
{
	m_transform.rot.set(rot);
}

//---------------------------------------------------------------------------
//  �U���Ɋւ�������Z�b�g���邽�߂̊֐�
//---------------------------------------------------------------------------
void CharacterBase::SetAttackInfo(const int attack_start_anim, int attack_anim_1, int attack_anim_2, const int combo_max)
{
	// �U���A�j���[�V�������n�܂�ԍ���ݒ肷�邽�߂̊֐�
	m_ATTAK_START_ANIM_NO = attack_start_anim;
	// �U���A�j���[�V�����̔ԍ��̕ۑ�
	m_first_attack_anim_1 = attack_anim_1;
	m_first_attack_anim_2 = attack_anim_2;
	// �ő�R���{���̕ۑ�
	m_COMBO_MAX = combo_max;

}

//---------------------------------------------------------------------------
// �ŏ��̍U�����s�����߂̊֐�
//---------------------------------------------------------------------------
void CharacterBase::AttackFirst()

{
	// �w��̃}�E�X�{�^���������ꂽ��
	if (PushMouseInput(MOUSE_INPUT_LEFT) || IsPadOn(PAD_ID::PAD_B))
	{
		// attack_flag ���オ���Ă�Ƃ�����
		// �v���C���[���[�h��ATTACK�ȊO�̎�
		if (m_attack_flag && m_player_mode != ATTACK)
		{
			// �A�j���[�V�����̐؂�ւ��t���O���グ��
			m_animation.m_anim_change_flag = true;

		}
		// �U�����[�h�ɂ��Ă���
		m_player_mode = ATTACK;
		// �U���A�j���[�V�����ԍ��̕ۑ�
		m_now_attack_anim = m_first_attack_anim_1;
		m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);


		// ���݂̍U���ԍ���ۑ�����
		m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;

		// �R���{�̉񐔂����Z�b�g
		m_combo_count = 0;

		// �R���{�����Ă����悤�ɂ���
		m_combo.ComboStartJudgent();
		// m_stop_combo_flag = false;
	}
	// �w��̃}�E�X�{�^���������ꂽ��
	if (PushMouseInput(MOUSE_INPUT_RIGHT) || IsPadOn(PAD_ID::PAD_Y))
	{
		// attack_flag ���オ���Ă�Ƃ�����
		// �v���C���[���[�h��ATTACK�ȊO�̎�
		if (m_attack_flag && m_player_mode != ATTACK)
		{
			// �A�j���[�V�����̐؂�ւ��t���O���グ��
			m_animation.m_anim_change_flag = true;


		}
		// �U�����[�h�ɂ��Ă���
		m_player_mode = ATTACK;
		// �U���A�j���[�V�����ԍ��̕ۑ�
		m_now_attack_anim = m_first_attack_anim_2;

		m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);


		// ���݂̍U���ԍ���ۑ�����
		m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;

		// �R���{�̉񐔂����Z�b�g
		m_combo_count = 0;

		// �R���{�����Ă����悤�ɂ���
		m_combo.ComboStartJudgent();
		
		//m_stop_combo_flag = false;

	}
}

//---------------------------------------------------------------------------
// �ŏ��̍U�����s�����߂̊֐�
//---------------------------------------------------------------------------
void CharacterBase::AttackUpdate()
{
	// �R���{�֐����Ă�
	ComboActionUpdate();
}

//---------------------------------------------------------------------------
// �����蔻����Ƃ��Ăق����^�C�~���O��ۑ����邽�߂̊֐�
//---------------------------------------------------------------------------
void CharacterBase::SetHitTiming(float attack_frame_start, float attack_frame_end, bool can_hitstop, int attack_num )
{
	// �����蔻����Ƃ��Ăق����ŏ��̃t���[���̕ۑ�
	m_attack_hit_damage[attack_num]->start_time = (int)attack_frame_start;
	// �I����Ăق����t���[���̕ۑ�
	m_attack_hit_damage[attack_num]->end_time = (int)attack_frame_end;
	// �q�b�g�X�g�b�v���s���Ă������ǂ����̕ۑ�
	m_attack_hit_damage[attack_num]->can_hit_stop = can_hitstop;
}


//---------------------------------------------------------------------------
// �����蔻����^�C�~���O��Ԃ��邽�߂̊֐�
//---------------------------------------------------------------------------
bool CharacterBase::AttackHitGoodTiming(int attack_num)
{
	// �X�^�[�g�̃t���[��
	int start_time = m_attack_hit_damage[attack_num]->start_time;
	// �I����Ăق����t���[��
	int end_time = m_attack_hit_damage[attack_num]->end_time;
	// �A�j���[�V�����̌��݂̃t���[��
	float play_anim_time = m_animation.m_contexts[0].play_time;
	// starttime�ȏ�@���@endtime�ȉ��̎�
	if (start_time <= play_anim_time && play_anim_time <= end_time)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//---------------------------------------------------------------------------
// �����蔻��ƃ_���[�W��ۑ�����֐�
//---------------------------------------------------------------------------
void CharacterBase::SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num)
{
	// �{���͓�܂Ƃ߂ĕۑ��ł������ł��Ȃ��Ȃ��Ă��邽�߂ЂƂÂ��Ă���
	// �����蔻��̕ۑ�
	m_attack_hit_damage[attack_num]->attack_hit = attack_hit;

	// �_���[�W�̕ۑ�
	m_attack_hit_damage[attack_num]->attack_damage = attack_damage;
}

//---------------------------------------------------------------------------
// ���[�����O�A�N�V�������J�n����邽�߂̊֐�
//---------------------------------------------------------------------------
void CharacterBase::RollingActionStart()
{
	// �w��̃L�[�������ꂽ��
	if (PushHitKey(KEY_INPUT_SPACE)|| IsPadOn(PAD_ID::PAD_A))
	{
		// �A�N�V�������[�h�����[�����O�ɂ���
		m_player_mode = ROLLING;
	}
}

//---------------------------------------------------------------------------
// ���[�����O�A�N�V�����̍X�V����
//---------------------------------------------------------------------------
void CharacterBase::RollingActionUpdate(int rolling_anim_no, const int rolling_speed)
{
	// ���[�����O�A�j���[�V�����̃Z�b�g
	// ���[�����O�t���O���オ���Ă��Ȃ��Ƃ�
	if (!m_rolling_flag)
	{
		// ���[�����O�A�j���[�V�������Z�b�g����
		m_animation.ChangeAnimation(&m_model, rolling_anim_no, false);
		// ���[�����O�t���O��������
		m_rolling_flag = true;
	}

	// ���[�����O���ɍU���{�^������������J�E���^�[�U���ɔh������
	if (PushMouseInput(MOUSE_INPUT_LEFT) || PushMouseInput(MOUSE_INPUT_RIGHT)|| IsPadOn(PAD_ID::PAD_B)|| IsPadOn(PAD_ID::PAD_Y))
	{
		m_counter_flag = true;
	}

	// ���[�����O���̈ړ�����
	// �����Ă�������� PLAYER_ROLLING_SPEED ���ړ�����
	m_transform.pos.z += rolling_speed * cosf(TO_RADIAN(m_transform.rot.y));
	m_transform.pos.x += rolling_speed * sinf(TO_RADIAN(m_transform.rot.y));

	// ���[�����O�A�j���[�V�������I�������(�I��肾�Ƃ��܂�����Ȃ���������I��鏭���O�ɂ���)
	// �܂��̓_���[�W��H������t���O���オ����������
	if (m_animation.m_contexts[0].play_time >= m_animation.m_contexts[0].animation_total_time - 10)
	{
		// �A�j���[�V�����̃`�F���W�t���O���グ��
		m_animation.m_anim_change_flag = true;
		// ���[�����O�t���O��������
		m_rolling_flag = false;
		if (m_counter_flag)
		{
			m_player_mode = COUNTER;
		}
		else
		{
			// ��U�A�N�V�������[�h��IDLE�ɂ��Ă���
			m_player_mode = IDLE;
		
		}
	}
}


//---------------------------------------------------------------------------
// �J�E���^�[�A�N�V����
//---------------------------------------------------------------------------
void CharacterBase::CounterAction(int counter_anim_no)
{
	// �U�����[�h�ɂ��Ă���
	m_player_mode = ATTACK;
	// �U���A�j���[�V�����ԍ��̕ۑ�
	m_now_attack_anim = counter_anim_no;
	m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);

	// ���݂̍U���ԍ���ۑ�����
	m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;
}

//---------------------------------------------------------------------------
// �R���{�A�N�V�����̍X�V����
//---------------------------------------------------------------------------
void CharacterBase::ComboActionUpdate()
{
	// �R���{�\�����f�p�֐�
// �w��̃L�[���삪���ꂽ�ꍇ
	switch (m_combo.m_combo_mode)
	{
	
	case m_combo.COMBO_START: // �R���{�X�^�[�g
		m_combo.ComboJudgmentKey
		(
			&m_combo_flag,
			&m_mouse_flag,
			MOUSE_INPUT_LEFT,
			PAD_ID::PAD_B,
			m_animation.m_contexts[0].play_time,
			m_animation.m_contexts[0].animation_total_time,
			&m_combo_count
		);
		m_combo.ComboJudgmentKey
		(
			&m_combo_flag,
			&m_mouse_flag,
			MOUSE_INPUT_RIGHT,
			PAD_ID::PAD_Y,
			m_animation.m_contexts[0].play_time,
			m_animation.m_contexts[0].animation_total_time,
			&m_combo_count
		);
		break;
		
	case m_combo.COMBO_STANDBY: // �R���{�X�^���o�C

		// ���̃A�j���[�V�����ԍ��������̃A�j���[�V����
		if (m_mouse_flag == MOUSE_INPUT_LEFT)
		{
			m_next_anim = m_first_attack_anim_1 + m_combo_count;
			aaa = m_next_anim;
			m_combo.ChangeComboMode(m_combo.COMBP_IN);
			break;
		}
		if (m_mouse_flag == MOUSE_INPUT_RIGHT)
		{
			m_next_anim = m_first_attack_anim_2 + m_combo_count;
			bbb = m_next_anim;
			m_combo.ChangeComboMode(m_combo.COMBP_IN);
			break;
		}
		
	case m_combo.COMBP_IN: // �R���{��
		// �R���{���A�j���[�V�����̍ő�Ɠ����ɂȂ�����(�w�肳�ꂽ)
		if (m_combo_count >= m_COMBO_MAX)
		{
			// �R���{�̉񐔂����Z�b�g
			m_combo_count = 0;

			// �����蔻��̐ݒ肪�o�b�N���Ă���̂ňꉺ����
			m_next_anim--;

			m_combo.ChangeComboMode(m_combo.COMBO_FINISH);
			break;

		}
		// �R���{�p�̃A�j���[�V����������
		m_animation.ActionComboChangeAnimation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// ���݂̍U���A�j���[�V������ۑ�
			m_now_attack_anim = m_next_anim;
			// ���݂̍U���ԍ���ۑ�����
			m_now_attack = m_now_attack_anim - m_ATTAK_START_ANIM_NO;

			m_combo.ChangeComboMode(m_combo.COMBO_START);
		}
		break;
	case m_combo.COMBO_FINISH: // �R���{�I��

		// �R���{�t���O��������
		m_combo_flag = false;

		break;

	}
}

//---------------------------------------------------------------------------
// �U�����󂯂������`�F�b�N���邽�߂̊֐�
//---------------------------------------------------------------------------
void CharacterBase::CheckHitDamage()
{
	// �_���[�W��H������t���O�������Ă�����
	if (m_damage_flag)
	{
		// �v���C���[�̏�Ԃ��_���[�W���󂯂���Ԃɂ���
		m_player_mode = HIT_DAMAGE;
		// �A�j���[�V�����̐؂�ւ��t���O���グ��
		m_animation.m_anim_change_flag = true;
	}
}

//---------------------------------------------------------------------------
// �U�����󂯂Ď��̃A�j���[�V��������
//---------------------------------------------------------------------------
void CharacterBase::HitDamageUpdate(int hit_damage_anim)
{
	// �U�����󂯂������U�����󂯂����̃A�j���[�V�������t���ĂȂ��Ƃ�(�t���O���������Ă�Ƃ�)
	if (m_damage_flag && m_damage_anim_flag == false)
	{
		// �U�����󂯂����̃A�j���[�V�������t�����̂Ńt���O���グ��
		m_damage_anim_flag = true;
		// �_���[�W�A�j���[�V����������
		if (m_animation.ChangeFlag(m_damage_anim_flag))
		{
			// �A�j���[�V�������~�ɕύX����
			m_animation.ChangeAnimation(&m_model, hit_damage_anim, true);

		}
	}
	// �U�����󂯂���Q�[���p�b�h��h�炷
	if (GetJoypadNum() >= 1)
	{
		PadVidation(DX_INPUT_PAD1, 1000, 3, -1);
	}
	// �_���[�W��H������A�j���[�V�������I���ɂ���������
	if (m_animation.m_contexts[0].play_time >= m_animation.m_contexts[0].animation_total_time - 10)
	{
		// ��U�����Ń_���[�W�󂯂��t���O�������Ă���
		m_damage_flag = false;
		// �U�����󂯂����̃A�j���[�V�������I���̂Ńt���O��������
		m_damage_anim_flag = false;
		// �v���C���[�̃��[�h��IDLE��Ԃɂ���
		m_player_mode = IDLE;
		// �A�C�h����Ԃɂ������̂ŃA�C�h���t���O�𗧂Ă�
		m_idle_flag = true;
		// �A�j���[�V�����ύX�t���O�����ĂĂ���
		m_animation.m_anim_change_flag = true;
	}
}


//---------------------------------------------------------------------------
// �x�[�X�N���X�ł̏�������
//---------------------------------------------------------------------------
void CharacterBase::BaseInit(float r, int HP)
{
	// ���W�A�����A��]�̏�����
	m_transform.pos.clear();
	// �����蔻��̔��a�̐ݒ�
	m_hit_r = r;
	// HP�̐ݒ�
	m_hp_value = HP;
}


//---------------------------------------------------------------------------
// �U�����̓����蔻���ݒ肷��p�̊֐�
//---------------------------------------------------------------------------
void CharacterBase::NEWSetAttackHitDamage(int attack_anim_max)
{
	// �U���A�j���[�V�����̐��m�ۂ���
	for (int i = 0; i < attack_anim_max; i++)
	{
		Attack_Hit_Damage* attack_hit_damage = new Attack_Hit_Damage;
		m_attack_hit_damage.push_back(attack_hit_damage);
		// �����l�͓K���Ȃ��̂����Ă���
		m_attack_hit_damage[i]->attack_hit = m_body;
		m_attack_hit_damage[i]->attack_damage = 0;
	}
}


//---------------------------------------------------------------------------
//  �x�[�X�N���X�ł̍X�V����
//---------------------------------------------------------------------------
void CharacterBase::BaseUpdate(bool* m_check_move, Vector3* camera_rot, const float* mov_speed)
{
	// �ړ��̏���
	m_move.Update(m_check_move, camera_rot, &m_transform.rot, mov_speed, &m_transform.pos, m_pad_no, m_mov);
	// ���f����Y����]�ɐ����������Ă���
	//m_model.RotationYLimits();
}


