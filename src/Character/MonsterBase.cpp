#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"

#include "src/Effect/Effect.h"
#include "src/System/UIBar.h"
#include "src/Sound/Sound.h"
#include "src/Camera.h"

#include "src/Action/Combo.h"
#include "src/System/TargetMove.h"
#include "src/Character/MonsterBase.h"



// �����X�^�[��p�̃L�����x�[�X�N���X

//---------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------
MonsterBase::MonsterBase()
{

}

//---------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------
MonsterBase::~MonsterBase()
{
	m_attack_hit_damage.clear();
	m_combo_pattern.clear();
}

//---------------------------------------------------------------------------
// �A�j���[�V�����̎w��̃t���[���ňړ������鏈��
//---------------------------------------------------------------------------
void MonsterBase::MoveBetween(float mov_speed, float mov_strat_frame, float mov_end_frame)
{
	if (m_animation.m_contexts[0].play_time >= mov_strat_frame
		&& m_animation.m_contexts[0].play_time <= mov_end_frame)
	{
		m_transform.pos.x += mov_speed * sinf(TO_RADIAN(m_transform.rot.y));
		m_transform.pos.z += mov_speed * cosf(TO_RADIAN(m_transform.rot.y));
	}
}

//---------------------------------------------------------------------------
// ���[�����O�A�N�V�������Z�b�g
//---------------------------------------------------------------------------
void MonsterBase::SetRollingAction(int rolling_anim, int target_distance)
{
	// �v���C���[�Ƃ̋�����������Ă���
	// �^�[�Q�b�g�Ƃ̋���
	float distance = m_move.GetTargetDistance();
	// �s���Ă��ȏ�̋����̎�
	// �^�[�Q�b�g�Ƃ̋��������ȉ���������
	if (distance >= target_distance)
	{
		// ���[�����O�t���O�𗧂Ă�
		m_rolling_flag = true;
		// �U���t���O�𗧂Ă�
		m_attack_flag = true;
		// �U�����[�h�ɂ��Ă���
		m_monster_mode = ATTACK;
		// �A�j���[�V�����ԍ���ۑ�
		m_now_attack_anim = rolling_anim;
		// �U���ԍ����ۑ�(�U���ԍ����~��������A�j���[�V�����ԍ�����U���X�^�[�g�ԍ�������)
		m_now_attack = m_now_attack_anim - M_ATTACK_ANIM_START;
		// �A�j���[�V�����̕ύX
		m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);
		// �����j���O�t���[��������������
		m_running_frame_count = 0;
	}
}

//---------------------------------------------------------------------------
// ���[�����O�A�N�V�����̒��̈ړ�����
//---------------------------------------------------------------------------
void MonsterBase::ActionRolling(const int rolling_speed, float rolling_start_frame, float rolling_end_frame)
{
	// ���[�����O���̈ړ�����
	// �����Ă�������� PLAYER_ROLLING_SPEED ���ړ�����
	MoveBetween(rolling_speed, rolling_start_frame, rolling_end_frame);
	// ���[�����O�A�j���[�V�������I�������
	if (m_animation.m_contexts[0].is_playing == false)
	{
		// ���[�����O�t���O������
		m_rolling_flag = false;
		// Run��ԂɕύX
		m_monster_mode = IDLE;
		// �A�j���[�V�����ύX�t���O�𗧂ĂĂ���
		m_animation.m_anim_change_flag = true;
	}
}

//---------------------------------------------------------------------------
// �X�^���������p�̍X�V����
//---------------------------------------------------------------------------
void MonsterBase::StunActionUpdate(int down_anim_num, int up_anim_num, int sutn_value_max)
{
	switch (m_stun_info_num)
	{
	case DOWN: // �X�^�����ē|�ꂽ�͂���
		// �����X�^�[�̗����Ă���t���O�����ׂĉ�����
		// m_attack_flag = false;
		// m_run_flag = false;
		// �_�E�����̃A�j���[�V����������
		if (m_animation.ChangeFlag(m_stun_flag))
		{
			m_animation.ChangeAnimation(&m_model, down_anim_num, false);
			// �A�j���[�V�����؂�ւ��p�̃t���O��؂�ւ���
			m_animation.m_anim_change_flag = false;
			// ��Ԃ����ɕύX
			m_stun_info_num = UPSTANDBY;
			// �����j���O�t���[��������������
			m_running_frame_count = 0;
		}
		break;
	case UPSTANDBY: // �X�^����
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// �A�j���[�V�����؂�ւ��p�̃t���O��؂�ւ���
			m_animation.m_anim_change_flag = true;
			// �N���オ��A�j���[�V�����ɕύX
			m_animation.ChangeAnimation(&m_model, up_anim_num, false);
			// �X�^���A�j���[�V�����ł��ꂽ���W������
			m_transform.pos.z += 22.0f * cosf(TO_RADIAN(m_transform.rot.y));
			m_transform.pos.x += 22.0f * sinf(TO_RADIAN(m_transform.rot.y));
			// ��Ԃ����̕ύX
			m_stun_info_num = UP;
		}
		break;

	case UP: // �N���オ��Ƃ�
		// �A�j���[�V�����̍Đ����x�ɍ��킹�ăX�^���l���񕜂�����
		static float stun_recovery_value = sutn_value_max / m_animation.m_contexts[0].animation_total_time;
		m_stun_value = stun_recovery_value * m_animation.m_contexts[0].play_time;

		if (m_animation.m_contexts[0].is_playing == false)
		{
			// �X�^����Ԃ̉���
			m_stun_flag = false;
			// �A�C�h����Ԃɂ��邽�߂ɃA�C�h���t���O�𗧂Ă�
			m_idle_flag = true;
			// �����X�^�[�̏�Ԃ��A�C�h���ɖ߂�
			m_monster_mode = IDLE;
			// �X�^����Ԃ����Z�b�g
			m_stun_info_num = DOWN;
			// �A�j���[�V�����؂�ւ��p�̃t���O��؂�ւ���
			m_animation.m_anim_change_flag = true;
			// �ق�Ƃ͏��X�ɑ��₵�Ă��������������ǂ��ꂪ���܂������Ȃ�
			m_stun_value = 150;
		}
		break;
	}

}


//---------------------------------------------------------------------------
// �x�[�X�N���X�̏�������
//---------------------------------------------------------------------------
void MonsterBase::BaseInit(int hp_num)
{
	// HP�ݒ�
	m_hp_value = hp_num;
	// HP�̍ő�l
	m_hp_max = hp_num;
	// �W�����v�֘A�̐ݒ�
	/*m_up_speed = up_speed;
	m_down_speed = down_speed;*/
}


//---------------------------------------------------------------------------
// �����X�^�[�̈ړ��Ɋւ���^�[�Q�b�g�̐ݒ�
//---------------------------------------------------------------------------
void MonsterBase::BaseSetTarget(Transform* target_pos, const float m_target_hit_r, CapsuleCollision body)
{
	// �ړ��̍ۂ̃^�[�Q�b�g�̂̐ݒ�
	m_move.SetTargetInfo(target_pos, m_target_hit_r,  body);
	// ���g�̏���ݒ�
	m_move.SetInfo(&m_transform, m_hit_r, M_MOV_SPEED, M_ROT_SPEED);
}


//---------------------------------------------------------------------------
// �x�[�X�N���X�̍X�V����
//---------------------------------------------------------------------------
void MonsterBase::MoveUpdate(bool* run_flag)
{

	// �ړ�����
	m_move.Update(run_flag);
	
}


//---------------------------------------------------------------------------
// �����蔻����Ƃ��Ăق����^�C�~���O��ۑ����邽�߂̊֐�
//---------------------------------------------------------------------------
void MonsterBase::SetHitTime(int attack_frame_start, int attack_frame_end, int attack_num)
{
	// �����蔻����Ƃ��Ăق����ŏ��̃t���[���̕ۑ�
	m_attack_hit_damage[attack_num]->start_time = attack_frame_start;
	// �I����Ăق����t���[���̕ۑ�
	m_attack_hit_damage[attack_num]->end_time = attack_frame_end;
}


//---------------------------------------------------------------------------
//  �����蔻����s���Ă����^�C�~���O��Ԃ��邽�߂̊֐�
//---------------------------------------------------------------------------
bool MonsterBase::AttackHitGoodTiming(int attack_num)
{
	// �X�^�[�g�̃t���[��
	int start_time = m_attack_hit_damage[attack_num]->start_time;
	// �I����Ăق����t���[��
	int end_time = m_attack_hit_damage[attack_num]->end_time;
	// �A�j���[�V�����̌��݂̃t���[��
	int play_anim_time = m_animation.m_contexts[0].play_time;
	if (play_anim_time >= start_time && play_anim_time <= end_time)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//---------------------------------------------------------------------------
// �����蔻����Ƃ��Ăق����^�C�~���O��ۑ����邽�߂̊֐�
//---------------------------------------------------------------------------
void MonsterBase::SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num)
{
	// �{���͓�܂Ƃ߂ĕۑ��ł������ł��Ȃ��Ȃ��Ă��邽�߂ЂƂÂ��Ă���
	// �����蔻��̕ۑ�
	m_attack_hit_damage[attack_num]->attack_hit = attack_hit;

	// �_���[�W�̕ۑ�
	m_attack_hit_damage[attack_num]->attack_damage = attack_damage;
}


//---------------------------------------------------------------------------
// �U�����̓����蔻���ݒ肷��p�̊֐�
//---------------------------------------------------------------------------
void MonsterBase::SetAttackHitDamage(int attack_anim_max)
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
// �U���A�j���[�V�����Ɋւ�����̐ݒ�
//---------------------------------------------------------------------------
void MonsterBase::SetAttackAnimInfo(int attack_anim_start, int attack_anim_max, int except_attack)
{
	// �U���A�j���[�V�����̃X�^�[�g�̕ۑ�
	M_ATTACK_ANIM_START = attack_anim_start;
	// �U���A�j���[�V�����̍ő吔�̕ۑ�
	M_ATTACK_ANIM_MAX = attack_anim_max;
	// �����_���ōU����I�ԍۂɂ͂Ԃ��Ăق����U���ԍ��̕ۑ�
	M_ATTACK_ANIM_EXCEPT = except_attack;
}

//---------------------------------------------------------------------------
// �A�C�h���A�N�V�����̍X�V����
//---------------------------------------------------------------------------
void MonsterBase::IdleActionUpdate(int idle_anim_num)
{
	//Player_Mode(IDLE);
	// �A�j���[�V�����ύX���\�Ȏ���
	if (m_animation.ChangeFlag(m_idle_flag))
	{
		// �A�j���[�V�������~�ɕύX����
		m_animation.ChangeAnimation(&m_model, idle_anim_num, true);
		m_animation.m_anim_change_flag = false;
	}

	// ��莞�Ԃ�������v���C���[��ǂ������n�߂�
	// �~�܂��Ăق����t���[�����ݒ肳��Ă��Ȃ��ꍇ
	if (!m_set_stop_frame_flag)
	{
		// �~�܂��Ă��Ăق����t���[������ݒ�
		m_stop_frame = GetRand(20) + m_combo_pattern[m_now_combo_pattern_num].m_rear_crevice_frame;
		// �ݒ肪�I������炩��t���O��������
		m_set_stop_frame_flag = true;
	}
	else // �~�܂��Ăق����t���[����ݒ肵�I����Ă�����
	{
		// �~�܂��Ăق����t���[�������炵�Ă���
		m_stop_frame--;
		// �t���[�����O�ȉ��ɂȂ�����
		if (m_stop_frame <= 0)
		{
			// �Z�b�g���������Ȃ��Ȃ����̂Ńt���O��������
			m_set_stop_frame_flag = false;
			// �~�܂��Ăق������Ԃ��I������̂Ńv���C���[��ǂ킹��
			m_monster_mode = RUN;
		}
	}
}

//---------------------------------------------------------------------------
// �ړ������֐�
//---------------------------------------------------------------------------
void MonsterBase::MoveAction(int run_anim)
{
	// ���񃊃Z�b�g
	m_run_flag = false;

	// �ړ��O�̍��W��U�ۑ����Ă���
	m_before_pos = m_transform.pos;

	// �x�[�X�N���X�̍X�V����
	// �ړ��̏��������ɓ����Ă���
	MoveUpdate(&m_run_flag);
	// �ړ�����������������܂��U���͈͂ɐN��������
	if (!m_move.m_hit || HitAttackArea())
	{
		// �����Ă���A�j���[�V���������Ă���̂ł�����~�߂�
		m_se.StopSound();
		// �����X�^�[�̏�Ԃ��U���ɕύX
		m_monster_mode = ATTACK;
	}
	// run_flag ���オ���Ă�Ƃ�����
	// �v���C���[���[�h��RUN�ȊO�̎�
	if (m_run_flag && m_animation.m_anim_change_flag == false)
	{
		// ����A�j���[�V�����ɕύX
		m_animation.ChangeAnimation(&m_model, run_anim, true);
		// �A�j���[�V�����̐؂�ւ��t���O���グ��
		m_animation.m_anim_change_flag = true;
	}
}


//---------------------------------------------------------------------------
// �R���{�p�^�[���̐���ݒ�
//---------------------------------------------------------------------------
void MonsterBase::ComboPatternNumberInit(int pattern_max)
{
	// �R���{�̃p�^�[���̍ő吔��ۑ�
	m_combo_pattern_max = pattern_max;

	// �p�^�[���̐������ꕨ��p�ӂ���
	m_combo_pattern.resize(pattern_max);
}


//---------------------------------------------------------------------------
// �e�R���{�p�^�[���̐ݒ�
//---------------------------------------------------------------------------
void MonsterBase::ComboPatternInfoInit(int pattern_num, int combo_num_max, int rear_crevice_frame, int* anim_num)
{
	// �z��̒�������R���{�̒����̕ۑ�
	m_combo_pattern[pattern_num].m_combo_num_max = combo_num_max;
	// �R���{��ۑ�����z��̊m��
	m_combo_pattern[pattern_num].m_combo_parts.resize(combo_num_max);
	// �R���{���I�������̂��ƌ��̐ݒ�
	m_combo_pattern[pattern_num].m_rear_crevice_frame = rear_crevice_frame;
	// �R���{�Ɏg�p����A�j���[�V�����ԍ���ۑ�����
	for (int i = 0; i < m_combo_pattern[pattern_num].m_combo_num_max; i++)
	{
		// �����ōU���ԍ�������
		m_combo_pattern[pattern_num].m_combo_parts[i] = anim_num[i];
	}
}


//---------------------------------------------------------------------------
// �U���͈͂ɓ��������ǂ������f����֐�
//---------------------------------------------------------------------------
bool MonsterBase::HitAttackArea()
{
	// �����X�^�[�̑O�������ɍU���͈͂̉~��u�������Ƀv������[����������U��������
	// �����X�^�[�̑O�������̍��W
	m_attack_area_1.x = (m_transform.pos.x+sinf(TO_RADIAN(m_transform.rot.y))* ATTACK_AREA_DISTANCE);
	m_attack_area_1.y = m_transform.pos.y + 10;
	m_attack_area_1.z = (m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y))* ATTACK_AREA_DISTANCE);
	// �J�v�Z����ڂ̍��W(�̓����X�^�[�̍��W
	m_attack_area_2.x = (m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * ATTACK_AREA_DISTANCE);
	m_attack_area_2.y = m_transform.pos.y + 10;
	m_attack_area_2.z = (m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * ATTACK_AREA_DISTANCE); 
	
	// �O�������U���G���A�p�J�v�Z��
	m_attack_area.CreateCapsuleCoordinatePos(m_attack_area_1, m_attack_area_2, ATTACK_AREA_R);
	// �J�v�Z���Ɠ����蔻�肪��������
	if (CheckCapsuleHit(m_attack_area, m_move.m_target_info.m_body))
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
// �ŏ��̍U�����s���p�֐�
//---------------------------------------------------------------------------
void MonsterBase::FirstAttackAction()
{
	// �U���t���O���グ��
	m_attack_flag = true;
	//// �U�����[�h�ɂ��Ă���
	//m_monster_mode = ATTACK;
	// �A�j���[�V�����̐؂�ւ��t���O���グ��
	m_animation.m_anim_change_flag = true;

	// �ǂ̍U���p�^�[�����g�p���邩�������_���Ŋe�m�ۂ���
	int m_combo_pattern_num = GetRand(m_combo_pattern_max - 1);

	// �R���{�̔ԍ����O�ɂ���
	m_combo_num = 0;
	// �R���{�̂Ɏg�p����U���ԍ���ۑ�
	m_now_attack = m_combo_pattern[m_combo_pattern_num].m_combo_parts[m_combo_num];
	// �U���A�j���[�V�����ԍ��̕ۑ�
	m_now_attack_anim = m_now_attack + M_ATTACK_ANIM_START;
	// �A�j���[�V�����̕ύX
	m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);
	// ���ݎg�p����Ă���R���{�p�^�[����ۑ�
	m_now_combo_pattern_num = m_combo_pattern_num;
}


//---------------------------------------------------------------------------
// �U���֘A�̍X�V����
//---------------------------------------------------------------------------
void MonsterBase::AttackActionComboUpdate()
{
	switch (m_attack_info_num)
	{
	case ATTACKSET:
		// �ǂ̃R���{���g�����������_���ŃZ�b�g
		FirstAttackAction();
		// ���̍U����ԂɈړ�
		m_attack_info_num = UNDERATTACK;
		break;
	case UNDERATTACK:
		// ���ݍs����A�j���[�V�������̍Đ����I�������
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// �R���{�ԍ��𑝂₷
			m_combo_num++;
			// �R���{�̂Ɏg�p����U���ԍ���ۑ�
			if (m_combo_pattern[m_combo_pattern_num].m_combo_parts[m_combo_num] != -1)
			{
				m_now_attack = m_combo_pattern[m_combo_pattern_num].m_combo_parts[m_combo_num];
			}
			else
			{
				// ���̍U���ԍ���-1�Ȃ�R���{�������Ȃ�����break����
				// �����X�^�[�̏�Ԃ�Idle��ԂɕύX����
				m_monster_mode = IDLE;
				// �A�j���[�V�����ύX�t���O���グ��
				m_animation.m_anim_change_flag = true;
				// �ŏ��̍U����ԂɈړ�
				m_attack_info_num = ATTACKSET;
				break;
			}

			// �ړ��������O�ꂽ��(�U���͈͂���o����)
			if (!HitAttackArea())
			{
				// ���̍U���ԍ���-1�Ȃ�R���{�������Ȃ�����break����
				// �����X�^�[�̏�Ԃ�Idle��ԂɕύX����
				m_monster_mode = IDLE;
				// �A�j���[�V�����ύX�t���O���グ��
				m_animation.m_anim_change_flag = true;
				// �ŏ��̍U����ԂɈړ�
				m_attack_info_num = ATTACKSET;
				break;
			}

			// �U���A�j���[�V�����ԍ��̕ۑ�
			m_now_attack_anim = m_now_attack + M_ATTACK_ANIM_START;
			// �A�j���[�V�����̕ύX
			m_animation.ChangeAnimation(&m_model, m_now_attack_anim, false);
		}

		break;
	}
}



//---------------------------------------------------------------------------
// ���̃_���[�W���󂯂����ɑ���Ƃ̋������Ƃ�p�̊֐�(���K�U��)
//---------------------------------------------------------------------------
void MonsterBase::RoarSet(int anim_num, int se_num, Camera* camera)
{

	if (m_roar_flag)
	{
		// �t���O�������Ă���Ԃ������̏������s��
		// �o��A�j���[�V�����̃Z�b�g(���[�v�����Ȃ�)

		// ��ʃV�F�C�N������
		camera->CameraShakeLimited(4.0f, 3.0f);

		// �A�j���[�V�������I��������ʃV�F�C�N���I���
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// ���K�t���O��������
			m_roar_flag = false;
			// �����X�^�[�̏�Ԃ��U����Ԃɂ���
			m_monster_mode = IDLE;
		}
	}
	// �����X�^�[��HP����萔����������K���������̂ň��̒l�Ŋ���
	int constant_hp = m_hp_max / 4;

	// HP�������Ē萔����������K�t���O�𗧂Ă�
	if(m_hp_value < constant_hp * m_roar_count)
	{
		// ���K�U���̃J�E���g��������
		m_roar_count--;
		// ���K�U���t���O���グ��
		m_roar_flag = true;
		// �_���[�W����ꂽ���̂ōU���t���O�𗧂Ă�
		m_attack_flag = true;
		// �����X�^�[�̏�Ԃ���K��Ԃɂ���
		m_monster_mode = ROAR;
		// �A�j���[�V�������ύX�ł���悤�ɂ���
		m_animation.m_anim_change_flag = true;
		if (m_animation.ChangeFlag(true))
		{
			// ���K�A�j���[�V����������
		m_animation.ChangeAnimation(&m_model, anim_num, false);
		}
		// �A�j���[�V��������t���[�����i�߂�
		m_animation.PlayAnimation(&m_model,false);
		// �Đ�����SE���I��点��
		m_se.StopSound();
		// ����SE���Đ��ł���悤�ɂ���
		m_se.m_playing_flag = true;
		// ���K�p��SE�̍Đ�
		SEUpdate(se_num);
	}
}

void MonsterBase::RoarAction(Camera* camera)
{
	if (m_roar_flag)
	{
		// �t���O�������Ă���Ԃ������̏������s��
		// �o��A�j���[�V�����̃Z�b�g(���[�v�����Ȃ�)

		// ��ʃV�F�C�N������
		camera->CameraShakeLimited(4.0f, 3.0f);
		// �Q�[���p�b�h���ڑ�����Ă���Ƃ��̓Q�[���p�b�h��U����������
		if (GetJoypadNum() >= 1)
		{
			PadVidation(DX_INPUT_PAD1, 1000, 3.0f, -1);
		}

		// �A�j���[�V�������I��������ʃV�F�C�N���I���
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// ���K�t���O��������
			m_roar_flag = false;
			// �����X�^�[�̏�Ԃ��U����Ԃɂ���
			m_monster_mode = IDLE;
		}
	}
}




//---------------------------------------------------------------------------
// �U���֘A�̍X�V����
//---------------------------------------------------------------------------
void MonsterBase::JumpAction(int jump_anim, int target_distance)
{
	// �v���C���[�Ƃ̋�����������Ă���
	// �^�[�Q�b�g�Ƃ̋���
	float distance = m_move.GetTargetDistance();
	// �s���Ă��ȏ�̋����̎�
	// �^�[�Q�b�g�Ƃ̋��������ȏゾ������
	if (distance >= target_distance)
	{
		// �U���t���O�𗧂Ă�
		m_jump_flag = true;
		// �U�����[�h�ɂ��Ă���
		m_monster_mode = ATTACK;
	}
	if (m_jump_flag)
	{
		// �U���ԍ��̕ۑ�(�U���ԍ����~��������A�j���[�V�����ԍ�����U���X�^�[�g�ԍ�������)
		m_now_attack = jump_anim - M_ATTACK_ANIM_START;
		// �W�����v�A�j���[�V����������
		m_animation.ChangeAnimation(&m_model, jump_anim, false);
	}
}


//---------------------------------------------------------------------------
// �W�����v�U���̍X�V����
//---------------------------------------------------------------------------
void MonsterBase::JumpActionUpdate(float jump_mov_speed, float jump_mov_strat_frame, float jump_mov_end_frame)
{
	// �W�����v���̃����X�^�[�̌����ɍ��킹�ăW�����v����
	// �����̈ړ��ʂ����܂��Ă��Ȃ�
	// �W�����v�A�j���[�V�����̒��n�ɍ��킹�Ĉړ�����߂�
	// ���ł���Ԃ���
	MoveBetween(jump_mov_speed, jump_mov_strat_frame, jump_mov_end_frame);


	// �W�����v�A�j���[�V�������I�������Run���[�h�ɂւ񂱂�����
	if (m_animation.m_contexts[0].is_playing == false)
	{
		// �W�����v���I������̂Ńt���O��������
		m_jump_flag = false;

		// Run��ԂɕύX
		m_monster_mode = RUN;
		// �A�j���[�V�����ύX�t���O�𗧂ĂĂ���
		m_animation.m_anim_change_flag = true;
	}

}


//---------------------------------------------------------------------------
// �R���{�U���X�V����
//---------------------------------------------------------------------------
void MonsterBase::ComboUpdate()
{
	// �R���{���s���Ă�����ԂȂ̂͂���ۑ�����ϐ�
	bool combo_jug;
	// TargetMove���^�[�Q�b�g�Ɛڂ��Ă��邻���łȂ����ŕς��
	// �ڂ��Ă����ړ��\��ԂɂȂ��
	if (m_move.m_hit)
	{
		// �R���{���ł����ԂłȂ�
		combo_jug = true;
	}
	// �ڂ��Ă��Ď~�܂��Ă���ꍇ
	if (!m_move.m_hit)
	{
		// �R���{�ł�����
		combo_jug = true;
	}

	// �R���{�\�����f�p�֐�
	m_combo.ComboJudgmentCondition
	(
		&m_combo_flag,
		combo_jug,
		m_animation.m_contexts[0].play_time,
		m_animation.m_contexts[0].animation_total_time
	);

	// �R���{�t���O���オ���Ă���Ƃ�
	if (m_combo_flag)
	{
		// ���̍U���A�j���[�V�����������_���ŃZ�b�g
		m_next_anim = SetRandAttack();

		// �R���{�p�̃A�j���[�V����������
		m_animation.ActionComboChangeAnimation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// ���݂̍U���A�j���[�V������ۑ�
			m_now_attack_anim = m_next_anim;
			// ���݂̍U���ԍ���ۑ�����
			m_now_attack = m_now_attack_anim - M_ATTACK_ANIM_START;
		}
	}
}


//---------------------------------------------------------------------------
// �s�������U���A�j���[�V�����������_���őI�Ԃ��߂̊֐�
//---------------------------------------------------------------------------
int MonsterBase::SetRandAttack()
{
	// ���ɍs���Ăق����A�j���[�V����������ϐ�
	int next_anim = 0;

	// �A�j���[�V���������܂閼�Ŗ������[�v
	while (true)
	{
		// ���̃A�j���[�V�����������_���œ����
		// �U���A�j���[�V�����X�^�[�g����U���A�j���[�V�����̍ő�܂ł�
		next_anim = GetRand(M_ATTACK_ANIM_MAX) + M_ATTACK_ANIM_START;
		// ���ɍs�������A�j���[�V�����ƍ��̃A�j���[�V���������Ԃ�����
		if (next_anim == m_now_attack_anim || next_anim == 0)
		{
			// �܂������_���œ���Ȃ���
			next_anim = GetRand(M_ATTACK_ANIM_MAX) + M_ATTACK_ANIM_START;
		}
		break;
	}

	// ���ɍs���Ăق����U���A�j���[�V������Ԃ�
	// ���̓v���C���[�̃J�E���^�[�����܂������Ă��邩�����邽�߂ɌŒ�ɂ��Ă���
	return next_anim;
}
