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

#include "Monster.h"



namespace
{
	// �v���C���[�̂Ƃ̂����蔻��Ŏg��box�̂����蔻��̃T�C�Y
	const Vector3 M_MOVE_SIZE{ 15.0f,0,15.0f };
	// ��L�ō쐬�����T�C�Y�̔����̃T�C�Y
	const Vector3 M_MOVE_SIZE_HALF{ M_MOVE_SIZE / 2 };
}


//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
Monster::Monster()
{
	// �ŏ��̓A�C�h����Ԃɂ��Ă���
	m_idle_flag = true;
	// �ŏ��̓A�C�h����Ԃɂ��Ă���
	m_monster_mode = IDLE;

	// �������W�̐ݒ�
	m_transform.pos.set(150.0f, 0.0f, 150.0f);
	// ���f���̃X�P�[���̐ݒ�
	m_transform.scale.set(0.25f, 0.25f, 0.25f);
}


//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
Monster::~Monster()
{
	Exit();
}

//-----------------------------------------------
// ����������
//-----------------------------------------------
void Monster::Init()
{
	// ���f���摜�̓ǂݍ���
	m_model.LoadModel("Data/Model/Monster/Monster.mv1");
	// �L�����N�^�[�̃}�e���A������
	// ���f���̃}�e���A�����m��
	// �}�e���A���̃A���r�G���g��������Ǝ����ɂ������Ă���e�����邭�Ȃ�
	m_model.SetMaterialAmbient({ 1.0f,1.0f,1.0f,0.0f });

	// �A�j���[�V�����̏����ݒ�
	AnimLoadInit();
	// �U���A�j���[�V�����Ɋւ�����̐ݒ�
	SetAttackAnimInfo(ATTACK_ANIM_START, ATTACK_ANIM_MAX, attack_rolling);
	// �U���A�j���[�V�����̐����̂����蔻��p�̓��ꕨ���m�ۂ���
	SetAttackHitDamage(ATTACK_ACTION_MAX);

	// �G�t�F�N�g�̏�����
	EffectLoadInit();
	// SE�̏�����
	SELoadInit();

	// �X�e�[�^�X�o�[�̐ݒ�
	StatusBarInit();

	// �U���Ɋւ��邱�Ƃ̏����֐�
	SetAttackInfo();
	// �����X�^�[�̃X�e�[�^�X�̏����ݒ�
	BaseInit(HP_VALUE_MAX/* JUMP_UP_SPEED, JUMP_DOWN_SPEED*/);
	// �A�j���[�V��������̃t���O���グ�Ă���
	m_animation.m_anim_change_flag = true;
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void Monster::Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera)
{

	// �����X�^�[�̏�Ԃɂ���Ẵt���O�Ǘ�
	MonsterMode(m_monster_mode);

	// �X�e�[�^�X�o�[�̒l�����������ǂ���
	m_hp_bra.Update(m_hp_value);
	if (m_stun_flag == false)
	{
		m_stun_bra.Update(m_stun_value);
	}


	switch (m_life_and_death)
	{

	case alive: // ��������Ƃ�
		// �ړ���̃^�[�Q�b�g�̐ݒ�
		BaseSetTarget(target_pos, target_r, body);
		LiveUpdate(target_pos, target_r, camera);
		// �����X�^�[��HP����葽������
		if (m_hp_value > 0)
		{
			// �����Ă�
			m_life_and_death = alive;
		}
		else // ����ȊO�̎�
		{

			// ����
			m_life_and_death = die_anim;
			// �v���C���[�̏�Ԃ���DIE�ύX
			m_monster_mode = DIE;
			// ���ׂẴt���O��������
			m_idle_flag = false;
			m_run_flag = false;
			m_attack_flag = false;

			// �A�j���[�V�����̐؂�ւ��t���O���グ��
			m_animation.m_anim_change_flag = true;
			// �ҋ@�t���O�𖈉񃊃Z�b�g
			m_idle_flag = true;
			// ���񂾃A�j���[�V����������
			if (m_animation.ChangeFlag(m_idle_flag))
			{
				// �A�j���[�V���������񂾂̂ɕύX����
				m_animation.ChangeAnimation(&m_model, die_anim, true);
			}
		}

		break;

	case die_anim: // ���񂾂Ƃ�
		DieUpdate();
		break;
	}

	// �A�j���[�V�����̍Đ�
	m_animation.PlayAnimation(&m_model, m_combo_flag);
	// �����蔻��̍X�V����
	CDUpdate();
}

//-----------------------------------------------
// �����Ă���Ƃ��̍X�V����
//-----------------------------------------------
void Monster::LiveUpdate(Transform* target_pos, float target_r, Camera* camera)
{

	// �X�^����ԂɂȂ邩�Ȃ�Ȃ����̔��f
	if (m_stun_value <= 0 && m_stun_flag == false)
	{
		// �X�^����ԂɈړ�
		m_monster_mode = STUN;
		// �X�^���t���O���グ��
		m_stun_flag = true;
		m_attack_flag = false;
		// �A�j���[�V�����ύX�t���O�𗧂Ă�
		m_animation.m_anim_change_flag = true;
	}
	// �X�^���l�������ȓ��łȂ����U�����󂯂Ă��Ȃ��Ƃ��ɃX�^���l���񕜂�����
	else if (m_stun_value > 0 && m_stun_value < STUN_VALUE_MAX && m_damage_anim_flag == false)
	{
		m_not_damaged_frame++;
		if (m_not_damaged_frame % STUN_VALUE_RECOVERY_FRAME == 0)
		{
			m_stun_value += RECOVERY_STUN_VALUE;
		}

	}

	// ���K�U���̏���
	if (m_stun_flag == false)
	{
		RoarSet(shout_anim, roar_se_info, camera);
	}


	switch (m_monster_mode)
	{
	case IDLE: // ��~��� 
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// �����X�^�[�̏�Ԃ��U����Ԃɂ���
			m_monster_mode = IDLE;
		}
		IdleActionUpdate(idle_anim);
		// �J�E���g�����Z�b�g����
		m_running_frame_count = 0;
		break;
	case RUN: // �����Ă�����
		// �ҋ@�t���O�𖈉񃊃Z�b�g
		m_idle_flag = false;
		// �ҋ@��Ԃ܂��͑���̎�������
		// �ړ�����
		// �����X�^�[�̉�]���Ă悢�悤�ɂ���
		m_move.SetCanRotate(true);
		// �ړ�����
		MoveAction(run_anim);

		// �����Ă���Ԃ̃t���[�������Z����
		m_running_frame_count++;

		// �����Ă��鎞�Ԃ����ȏ�ɂȂ�����
		if (m_running_frame_count >= CHANGE_JUMP_RUNNIG_FRAME)
		{
			// �W�����v�A�N�V�������Z�b�g����
			JumpAction(jump_anim, JUMP_TARGET_DISTANCE);
		}
		// �����Ă��鎞�Ԃ����ȏ�ɂȂ�����
		if (m_running_frame_count >= CHANGE_JUMP_RUNNIG_FRAME)
		{
			// ���[�����O�A�N�V�������Z�b�g����
			SetRollingAction(rolling_anim, ROLLING_TARGET_DISTANCE);
		}

		// �����t���O�������Ă���Ԃ���
		if (m_run_flag == true && m_rolling_flag == false && m_jump_flag == false)
		{
			// SE�̍Đ�
			SEUpdate(run_se_info);
		}
		else
		{
			// �Đ�����SE���~�߂�
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}

		// �����X�^�[�̏�Ԃ��U����ԂɂȂ��Ă�����
		if (m_monster_mode == ATTACK)
		{
			// �Đ�����SE���~�߂�
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}
		break;

	case ATTACK: // �U�����
		// �X�^�����͂ق��̍U�����������Ăق����Ȃ�
		if (m_stun_flag == true)
		{
			m_monster_mode = STUN;
			break;
		}

		// ���K�U�����͂ق��̏��������Ăق����Ȃ�
		if (m_roar_flag == true)
		{
			break;
		}

		// �W�����v�t���O�������Ă���Ƃ�
		if (m_jump_flag)
		{

			// �W�����v�̍X�V����
			JumpActionUpdate(JUMP_MOV_SPEED, JUMP_STRAT_FRAME, JUMP_END_FRAME);
			// �W�����v�A�j���[�V�������I������Ƃ��ɃA�j���[�V�����ł������W�̂����͂����Œ���
			if (m_animation.m_contexts[0].is_playing == false)
			{
				m_transform.pos.x += JUMP_DEVIATION_POS * sinf(TO_RADIAN(m_transform.rot.y));
				m_transform.pos.z += JUMP_DEVIATION_POS * cosf(TO_RADIAN(m_transform.rot.y));
			}
		}

		// ���[�����O�A�N�V�������̏���
		if (m_now_attack_anim == rolling_anim)
		{
			ActionRolling(ROLLING_SPEED, ROLLING_STRAT_FRAME, ROLLING_END_FRAME);
			// ���[�����O�A�N�V�����̂��ƂɃv���C���[���U���͈͂ɂ͂��Ă��Ȃ�������
			if (m_rolling_flag == false && HitAttackArea() == false)
			{
				break;
			}
		}

		// �U���p�̊֐�
		// �W�����v�ƃ��[�����O�̂��s���Ă��Ȃ��Ƃ�
		if (m_jump_flag == false && m_rolling_flag == false)
		{
			// �����X�^�[�̈ړ����ł��Ȃ������ɓG��������
			// �U�����n�߂�
			AttackActionComboUpdate();
		}

		// �U���̂������G�t�F�N�g�̍Đ�
		// �U���̃G�t�F�N�g���L�b�N�ȊO�̎�
		// �U�����̃G�t�F�N�g�͍U���ƃ^�C�~���O�����킹��
		if (m_animation.m_contexts[0].play_time >= m_effect_info[m_now_attack].effect_start_anim_frame &&
			m_effect.m_play_effect_flag == true
			/*m_rolling_flag == false*/)
		{
			if (m_now_attack == attack_bigpunch || m_now_attack == attack_punch || m_now_attack == attack_upperpunch)
			{
				EffectUpdate(big_punch_attack_effect, m_now_attack);
			}
			/*else if (m_now_attack == attack_upperpunch )
			{
				EffectUpdate(punch_attack_effect, m_now_attack);
			}*/
			else if (m_now_attack == attack_breath)
			{
				EffectUpdate(breath_attack_effect, m_now_attack);
			}

		}

		// SE���ݒ肳��Ă��Ȃ��U���̎��͍Đ����Ȃ�
		if (m_rolling_flag == false && m_jump_flag == false)
		{
			// �U���ɂ������T�E���h���Đ�
			if (m_animation.m_contexts[0].play_time >= m_se_info[m_now_attack].se_start_frame)
			{
				SEUpdate(m_now_attack);
			}
		}

		// �U���A�j���[�V�������I���������
		if (m_animation.m_contexts[0].is_playing == false)
		{
			// ���̃G�t�F�N�g���Đ��ł���悤�ɂ���
			m_effect.m_play_effect_flag = true;
			// ���ݍĐ����̍U���G�t�F�N�g���I��点��
			m_effect.StopEffect();
			// ����SE���Đ��ł���悤�ɂ���
			m_se.m_playing_flag = true;
			// �Đ�����SE���I��点��
			m_se.StopSound();
		}
		// �U����(�A�j���[�V������)�͉�]���Ăق����Ȃ�
		m_move.SetCanRotate(false);

		break;
	case STUN: // �X�^�����
		// �W�����v���ɃX�^������Ă��܂����Ƃ�
		if (m_jump_flag == true)
		{
			m_transform.pos.x += JUMP_DEVIATION_POS * sinf(TO_RADIAN(m_transform.rot.y));
			m_transform.pos.z += JUMP_DEVIATION_POS * cosf(TO_RADIAN(m_transform.rot.y));
			// �W�����v�t���O��������
			m_jump_flag = false;
		}
		// �X�^���̍X�V����
		StunActionUpdate(stun_down_anim, stun_up_anim, STUN_VALUE_MAX);

		break;
	case ROAR: // ���K���
		// ���K�U�����̏���
		RoarAction(camera);
		break;
	}
}


//-----------------------------------------------
// ���񂾂Ƃ��̍X�V����(�Q�[���V�[���Œ��ڌĂ�)
//-----------------------------------------------
void Monster::DieUpdate()
{
	// ���񂾃A�j���[�V�������ǂ�Ȃ��悤��
	// ���̂Ƃ���܂Ői�񂾂�
	if (m_animation.m_contexts[0].play_time >= 260)
	{
		// �����O�ɖ߂�
		m_animation.m_contexts[0].play_time = 200;
	}
}

//-----------------------------------------------
// �`�揈��
//-----------------------------------------------
void Monster::Draw()
{
	// �U���^�C�~���O���������蔻���`��
	if (m_attack_flag)
	{
		if (AttackHitGoodTiming(m_now_attack))
		{
			//m_right_hand.Draw();
			m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
		}
	}

	// �J�v�Z���̕`��(�����蔻��)
	/*m_body.Draw();
	m_left_hand.Draw();
	m_right_hand.Draw();
	m_left_feet.Draw();
	m_right_feet.Draw();
	m_big_punch_hit.Draw();
	m_breath_hit.Draw();*/
	// ���f���̕`�� (�`�����ɂ��Ȃ��Ɠ����蔻�肪������������)
	m_model.DrawModel(&m_transform);

}

//-----------------------------------------------
// �I������
//-----------------------------------------------
void Monster::Exit()
{
	m_se.StopSound();
}

//-----------------------------------------------
// �o�ꉉ�o�p�̍X�V����
//-----------------------------------------------
void Monster::EntryUpdate()
{
	// �����ł��邱��
	// �o��V�[���ɂ��������т̃A�j���[�V����������

	// �o��A�j���[�V�����̃Z�b�g(���[�v�����Ȃ�)
	if (m_animation.ChangeFlag(true))
	{
		m_animation.ChangeAnimation(&m_model, shout_anim, true);
	}

	// �A�j���[�V�����̍Đ�
	m_animation.PlayAnimation(&m_model, m_combo_flag);
	// ���K�p��SE�̍Đ�
	SEUpdate(roar_se_info);

}

//-----------------------------------------------
// HP�����܂Ō������Ƃ��̃��x���A�b�v����
//-----------------------------------------------
void Monster::ReinforceUpdate()
{
	// HP�������ɂȂ�����
	// �_���\�W��H������A�j���[���[�V����������
	// ���̂��Ƃɋ��ԃA�j���[�V����������
	// ���̌�ōU���𑝂₵����_���[�W�ʂ𑝂₵������G�t�F�N�g��h��ɂ�����ω���������

}

//-----------------------------------------------
// �U�����󂯂����̍X�V����
//-----------------------------------------------
void Monster::ComeAttackUpdate()
{
	m_effect.m_play_effect_flag = true;

	// �Đ����̃G�t�F�N�g����������I��������
	m_effect.StopEffect();
	//�_���[�W���󂯂����̃G�t�F�N�g
	EffectUpdate(damage_effect, damage_effect_info);

	// �U�����󂯂�������SE���d�Ȃ��Ăق������ߑ��Ƃ͈Ⴄ���@�ōĐ�
	m_se.m_playing_flag = true;
	// �_���[�W�󂯂����p��SE�̍Đ�
	m_se.PlaySound_(m_se_info[damage_se_info].se_num, m_se_info[damage_se_info].play_type, m_se_info[damage_se_info].loop);
	// SE���Đ����ꂽ�̂ōĐ��\��Ԃ�ύX����
	m_se.m_playing_flag = false;

}

//-----------------------------------------------
// �����蔻��
//-----------------------------------------------
void Monster::CDUpdate()
{
	// �L�����{�̂̓����蔻��̃J�v�Z���i��ŏ����j
	// ���̍��W�����f���̃m�[�h���łƂ��Ă���Ƃ�������
	m_body.CreateNodoCapsule(&m_model, 5, 81, 12.0f);
	// ����̂����蔻��
	m_left_hand.CreateNodoCapsule(&m_model, 13, 25, 8.0f);
	// �E��̓����蔻��
	m_right_hand.CreateNodoCapsule(&m_model, 37, 49, 8.0f);
	// ����
	m_left_feet.CreateNodoCapsule(&m_model, 67, 64, 6.0f);
	// �E��
	m_right_feet.CreateNodoCapsule(&m_model, 69, 72, 6.0f);

	// ��p���`�U����p�̓����蔻��
	m_big_punch_hit.CreateNodoCapsule(&m_model, 25, 49, 15.0f);

	// �J�v�Z���̍��W�P
	Vector3 top_pos;
	top_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 20,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 20);

	// �J�v�Z���̍��W�Q
	Vector3 under_pos;
	under_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 10,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 10);
	// �u���X�U���̓����蔻��̍쐬
	m_breath_hit.CreateCapsuleCoordinatePos(top_pos, under_pos, 10);

	// �U�����̓����蔻��̕ۑ�
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch], (attack_punch));
	SetHitDamage(m_big_punch_hit, m_attack_damage[attack_bigpunch], (attack_bigpunch));
	SetHitDamage(m_right_hand, m_attack_damage[attack_upperpunch], (attack_upperpunch));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick], (attack_kick));
	SetHitDamage(m_right_hand, m_attack_damage[attack_takle], (attack_takle));
	SetHitDamage(m_breath_hit, m_attack_damage[attack_breath], (attack_breath));
	SetHitDamage(m_body, m_attack_damage[attack_rolling], (attack_rolling));
	SetHitDamage(m_body, m_attack_damage[attack_jump], (attack_jump));

}

//-----------------------------------------------
// �X�e�[�^�X�o�[�̐ݒ�p�֐�
//-----------------------------------------------
void Monster::StatusBarInit()
{
	// HP�̎c�ʂ�ݒ�
	m_hp_value = HP_VALUE_MAX;
	// HP�o�[�̐ݒ�
	m_hp_bra.Set({ 25,25 }, { SCREEN_W - 50, 25 }, &m_hp_value, true);
	m_hp_bra.SetColor(255, 100, 50, &m_hp_bra.m_color);
	m_hp_bra.SetColor(128, 128, 128, &m_hp_bra.m_back_color);
	m_hp_bra.SetColor(0, 0, 0, &m_hp_bra.m_line_color);
	m_hp_bra.SetColor(255, 255, 255, &m_hp_bra.m_character_color);
	m_hp_bra.SetName("HP");

	// �X�^���l�̎c�ʂ�ݒ�
	m_stun_value = STUN_VALUE_MAX;
	// �X�^���o�[�̐ݒ�
	m_stun_bra.Set({ 25,70 }, { SCREEN_W - 50, 20 }, &m_stun_value, true);
	m_stun_bra.SetColor(255, 255, 0, &m_stun_bra.m_color);
	m_stun_bra.SetColor(128, 128, 128, &m_stun_bra.m_back_color);
	m_stun_bra.SetColor(0, 0, 0, &m_stun_bra.m_line_color);
	m_stun_bra.SetColor(255, 255, 255, &m_stun_bra.m_character_color);
	//m_stun_bra.SetName("STUN");
}

//-----------------------------------------------
// �X�e�[�^�X�o�[�`��p�֐�
//-----------------------------------------------
void Monster::StatusBarDraw()
{

	//===================
	// UI�̕`��
	//===================
	m_hp_bra.Draw();
	m_stun_bra.Draw();

}


//-----------------------------------------------
// �����蔻����s���ė~�����^�C�~���O��ۑ�����֐�
// �S���̍U�������蔻��ɐݒ肷��(��ł��)
//-----------------------------------------------
void Monster::SetAttackInfo()
{

	//------------------------------------------
	// �R���|�p�^�[�������p�^�[�����邩�̐ݒ�
	//------------------------------------------
	ComboPatternNumberInit(M_COMBO_PATTERN_MAX);

	//------------------------------------------
	// �e�R���{�p�^�[���̃R���{�̐ݒ�
	//------------------------------------------
	for (int i = 0; i < M_COMBO_PATTERN_MAX; i++)
	{
		// �e�R���{�̌㌄��ۑ�
		int frame = m_combo_rear_crevice_frame[i];
		// �e�R���{�p�^�[���̐ݒ�
		ComboPatternInfoInit(i, M_COMBO_NUM_MAX, frame, m_combo_pattern[i]);
	}


	//------------------------------------------
	// �����蔻��̃^�C�~���O�̐ݒ�
	//------------------------------------------
	SetHitTime(attack_frame[attack_punch].start_frame, attack_frame[attack_punch].end_frame, attack_punch);
	SetHitTime(attack_frame[attack_kick].start_frame, attack_frame[attack_kick].end_frame, attack_kick);
	SetHitTime(attack_frame[attack_bigpunch].start_frame, attack_frame[attack_bigpunch].end_frame, attack_bigpunch);
	SetHitTime(attack_frame[attack_breath].start_frame, attack_frame[attack_breath].end_frame, attack_breath);
	SetHitTime(attack_frame[attack_takle].start_frame, attack_frame[attack_takle].end_frame, attack_takle);
	SetHitTime(attack_frame[attack_upperpunch].start_frame, attack_frame[attack_upperpunch].end_frame, attack_upperpunch);
	SetHitTime(attack_frame[attack_rolling].start_frame, attack_frame[attack_rolling].end_frame, attack_rolling);
	SetHitTime(attack_frame[attack_jump].start_frame, attack_frame[attack_jump].end_frame, attack_jump);
}


//-----------------------------------------------
// �t���O�Ǘ��p�֐�
// ���̊֐����Ȃ��ƐF�X�ȃo�O���o��
//-----------------------------------------------
void Monster::MonsterMode(int mode)
{
	switch (mode)
	{
	case IDLE:
		m_idle_flag = true;
		m_run_flag = false;
		m_attack_flag = false;

		break;
	case RUN:
		m_idle_flag = false;
		m_run_flag = true;
		m_attack_flag = false;
		m_stun_flag = false;
		break;
	case ATTACK:

		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = true;

		break;
	case DIE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;

		break;
	case STUN:

		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_roar_flag = false;

		break;

	case ROAR:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_roar_flag = true;
		break;
	}
}

//-----------------------------------------------
// �A�j���[�V�����̏�������
//-----------------------------------------------
void Monster::AnimLoadInit()
{
	// �A�j���[�V�����̏����ݒ�
	m_animation.InitAnimation(anim_max, idle_anim);
	// �A�j���[�V�����̓ǂݍ���
	m_animation.LoadAnimation("Data/Model/Monster/Animation/idle.mv1", idle_anim, 0, 1.0f);             //!< �A�C�h��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Run.mv1", run_anim, 0, 1.0f);               //!< ����
	m_animation.LoadAnimation("Data/Model/Monster/Animation/die.mv1", die_anim, 0, 1.0f);               //!< ���S
	m_animation.LoadAnimation("Data/Model/Monster/Animation/shout.mv1", shout_anim, 0, 0.5f);           //!< ����
	m_animation.LoadAnimation("Data/Model/Monster/Animation/hit_damage.mv1", hit_damage_anim, 0, 1.0f); //!< �_���[�W���󂯂���
	m_animation.LoadAnimation("Data/Model/Monster/Animation/stun_down.mv1", stun_down_anim, 0, 1.5f);   //!< �X�^����H��������̃_�E��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/stun_up.mv1", stun_up_anim, 0, 2.0f);       //!< �X�^����H��������̋N���オ��

	// �U���A�j���[�V����
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Punch.mv1", punch_attack_anim, 0, 1.5f);      //!< �p���`�U��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Kick.mv1", kick_attack_anim, 0, 1.0f);            //!< �L�b�N�U��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/BigPunch.mv1", bigpunch_attack_anim, 0, 1.3f);   //!< ��p���`�U��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Breath.mv1", breath_attack_anim, 0, 1.5f);     //!< �u���X�U��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/tackle.mv1", tackle_attack_anim, 0, 1.0f);      //!< �^�b�N���U��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/UpperPunch.mv1", upperpunch_attack_anim, 0, 1.5f); //!< �A�b�p�[�U��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/Rolling.mv1", rolling_anim, 0, 1.0f);           //!< ���[�����O
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Attack/JumpAttack.mv1", jump_anim, 0, 1.0f);           //!< �W�����v


	// �ŏ��̓f�t�H���g�A�j���[�V���������Ă���
	m_animation.InitAttachAnimation(&m_model, idle_anim, true);
}

//-----------------------------------------------
// �G�t�F�N�g�̏�������
//-----------------------------------------------
void Monster::EffectLoadInit()
{
	// �G�t�F�N�g������
   // �G�t�F�N�g�̍ő吔��ݒ�
	m_effect.NewArraySecure(effect_max);
	// �G�t�F�N�g�̓ǂݍ���
	m_effect.LoadEffect("Data/Model/Monster/Effect/Punch2.efkefc", punch_attack_effect, 1.0f);      // �p���`�U�����̃G�t�F�N�g
	m_effect.LoadEffect("Data/Model/Monster/Effect/Punch.efkefc", big_punch_attack_effect, 1.0f);  // ��p���`�U�����̃G�t�F�N�g
	m_effect.LoadEffect("Data/Model/Monster/Effect/Breath.efkefc", breath_attack_effect, 1.0f);      // �u���X�U�����̃G�t�F�N�g
	m_effect.LoadEffect("Data/Model/Monster/Effect/blood.efkefc", damage_effect, 1.0f);                // �_���[�W���󂯂����̃G�t�F�N�g
}

//-----------------------------------------------
// �G�t�F�N�g�̍X�V����
//-----------------------------------------------
void Monster::EffectUpdate(int nodo_index, int effect_num, int effect_info_num)
{
	// �G�t�F�N�g���Đ��\��ԂȂ�
	if (m_effect.m_play_effect_flag == true)
	{
		// �G�t�F�N�g�̍Đ�
		m_effect.PlayEffect(effect_num, m_transform.pos);
		// �G�t�F�N�g���Đ����ꂽ�̂ōĐ����Ă͂����Ȃ��悤�ɂ���
		m_effect.m_play_effect_flag = false;
	}

	

	// �G�t�F�N�g�ɂ���č��W�����킹��
	//m_effect.SetEffectPos(attack_effect[m_now_attack].pos);
	if (nodo_index == -1)
	{
		m_effect.SetEffectRotPos(m_transform.pos, m_effect_info[effect_info_num].pos, m_transform.rot);
	}
	else
	{
		m_model.GetNodePos(nodo_index);
		m_effect.SetEffectRotPos(m_transform.pos, m_effect_info[effect_info_num].pos, m_transform.rot);
	}
	
	// �G�t�F�N�g�̃T�C�Y�����킹��
	m_effect.SetEffectSize(m_effect_info[effect_info_num].size);
	
	// �G�t�F�N�g�̌��������킹��
	// �v���C���[�̌����ɂ����킹��
	m_effect.SetEffectRot(m_effect_info[effect_info_num].rot.x, m_effect_info[effect_info_num].rot.y + m_transform.rot.y, m_effect_info[effect_info_num].rot.z);
}

//-----------------------------------------------
// SE�̏�������
//-----------------------------------------------
void Monster::SELoadInit()
{
	// SE���m��
	// SE�̍ő吔��ݒ�
	m_se.NewArraySecureSound(se_max);
	// SE�̓ǂݍ���
	m_se.LoadSound("Data/Model/Mutant/SE/PunchAttack_2.mp3", punch_attack_se);                   // �p���`�U��
	m_se.LoadSound("Data/Model/Mutant/SE/LightningSwordAttack_1.mp3", sword_attack_se_1);   // ���U���P
	m_se.LoadSound("Data/Model/Mutant/SE/LightningSwordAttack_2.mp3", sword_attack_se_2);   // ���U���Q
	m_se.LoadSound("Data/Model/Mutant/SE/Damage.mp3", damage_se);									 // �_���[�W���󂯂���
	m_se.LoadSound("Data/Model/Mutant/SE/Roar.mp3", roar_se);												  // ���K��
	m_se.LoadSound("Data/Model/Mutant/SE/Run3.mp3", run_se);													 // ����

}

//-----------------------------------------------
// SE�̍X�V����
//-----------------------------------------------
void Monster::SEUpdate(int se_info_num)
{
	// SE���Đ�����Ă�����SE�Đ��\��ԂȂ�
	if (m_se.PlayingSound() == false && m_se.m_playing_flag)
	{
		// SE�̍Đ�
		m_se.PlaySound_(m_se_info[se_info_num].se_num, m_se_info[se_info_num].play_type, m_se_info[se_info_num].loop);
		// SE���Đ����ꂽ�̂ōĐ��\��Ԃ�ύX����
		m_se.m_playing_flag = false;
	}
}

