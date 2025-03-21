#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/System/Spotlight.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

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
	m_transform.pos.set(50.0f, 0.0f, 50.0f);
	// ���f���̃X�P�[���̐ݒ�
	m_transform.scale.set(0.2f, 0.2f, 0.2f);
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
	// �A�j���[�V�����̏����ݒ�
	Anima_Load_Init();

	// �X�e�[�^�X�o�[�̐ݒ�
	StatusBarInit();

	// �����X�^�[�̃X�e�[�^�X�����̐ݒ�
	MonsterBase::BaseInit(HP_MAX);
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void Monster::Update(Transform* traget_pos, float target_r,CapsuleCollision body, Camera* camera)
{
	clsDx();
	BaseSetTarget(traget_pos, target_r,body);

	// �ҋ@��Ԃ܂��͑���̎�������
	// �ړ�����
	if (m_idle_flag == true || m_run_flag == true /*&& m_monster_mode == IDLE*/)
	{
		// �����X�^�[�̉�]���Ă悢�悤�ɂ���
		m_move.SetCanRotate(true);
		// �ړ�����
		Move_Update();
	}

	switch (m_monster_mode)
	{
	case IDLE: // ��~��� 
		if (m_idle_flag)
		{
			//Player_Mode(IDLE);
			// �A�j���[�V�����ύX���\�Ȏ���
			if (m_animation.ChangeFlag(m_idle_flag))
			{
				// �A�j���[�V�������~�ɕύX����
				m_animation.ChangeAnimation(&m_model, idle, true);
			}

			// �ړ����~�܂��Ă�����
			if (!m_move.m_hit)
			{
				// �ŏ��̍U�����s��
				// �U���t���O���グ��
				m_attack_flag = true;
				Attack_First();
			}
		}
		break;
	case RUN:
		// �ҋ@�t���O�𖈉񃊃Z�b�g
		m_idle_flag = false;
		// run_flagf�t���O������������
		if (m_run_flag == false)
		{
			// �ҋ@�t���O���グ��
			m_idle_flag = true;
			// �A�j���[�V�����ύX���s����悤�ɂ���
			m_animation.m_anim_change_flag = true;
			m_monster_mode = IDLE;
		}

		// �����Ă���ԂɈ��ȏ�̋������󂢂���
		// �W�����v�U��������
		Attack_Jump();

		break;
	case ATTACK:

		// �����Ăق����Ȃ��̂Ńt���O��
		m_idle_flag = false;
		m_run_flag = false;

		// �W�����v�U�����̏���
		if (m_now_attack_anim == jump)
		{
			Jump_Update();
		}
		// �U����(�A�j���[�V������)�͉�]���Ăق����Ȃ�
		m_move.SetCanRotate(false);
		// �����Ă����͈͂����v���C���[�̌����Ƃ����Ă����邩�𒲂ׂ�
		m_move.m_hit = m_move.TargetHit();
		
		// �A�j���[�V�����̍Đ����I������Ƃ�
		if (m_animation.m_contexts[0].play_time >= m_animation.m_contexts[0].animation_total_time)
		{
			// �ړ����Ă�����Ԃ�������
			if (m_move.m_hit)
			{
				// �ړ��t���O�𗧂Ă�
				m_run_flag = true;
			}
		}
		// �U���p�̊֐�
		Attack_Update();

		break;
	}

	// �A�j���[�V�����̍Đ�
	m_animation.PlayAnimation(&m_model, m_combo_flag);
	// �����蔻��̍X�V����
	CDUpdate();
}

//-----------------------------------------------
// �`�揈��
//-----------------------------------------------
void Monster::Draw()
{
	// �J�v�Z���̕`��(�����蔻��)
	m_body.Draw();
	m_left_hand.Draw();
	m_right_hand.Draw();
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
// �����蔻��
//-----------------------------------------------
void Monster::CDUpdate()
{
	// �L�����{�̂̓����蔻��̃J�v�Z���i��ŏ����j


	// ���̍��W�����f���̃m�[�h���łƂ��Ă���Ƃ�������
	// m_body.CreateCapsule(m_transform.pos);
	// m_body.SetSize({ 0.0f,25.0f, 0.0f }, 12);

	// ����̂����蔻��
	m_left_hand.CreateNodoCapsule(&m_model, 12, 22, 3.0f);
	

	// �E��̓����蔻��
	m_right_hand.CreateNodoCapsule(&m_model, 36, 46, 3.0f);
	
}

//-----------------------------------------------
// �X�e�[�^�X�o�[�̐ݒ�p�֐�
//-----------------------------------------------
void Monster::StatusBarInit()
{
	// HP�̐ݒ�
	m_hp.Set({ 25,25 }, { 1200, 25 }, &m_hp_value, true);
	m_hp.SetColor(255, 100, 50, &m_hp.m_color);
	m_hp.SetColor(128, 128, 128, &m_hp.m_back_color);
	m_hp.SetColor(0, 0, 0, &m_hp.m_line_color);
	m_hp.SetColor(255, 255, 255, &m_hp.m_character_color);
	m_hp.SetName("HP");
}

//-----------------------------------------------
// �X�e�[�^�X�o�[�`��p�֐�
//-----------------------------------------------
void Monster::StatusBarDraw()
{
	// HP�̒l�����������ǂ���
	m_hp.Update(m_hp_value);
	//===================
	// UI�̕`��
	//===================
	m_hp.Draw();
}

//-----------------------------------------------
// �A�j���[�V�����̏�������
//-----------------------------------------------
void Monster::Anima_Load_Init()
{
	// �A�j���[�V�����̏����ݒ�
	m_animation.InitAnimation(anim_max, idle);
	// �A�j���[�V�����̓ǂݍ���
	m_animation.LoadAnimation("Data/Model/Monster/Animation/idle.mv1", idle, 0, 1.0f); //!< �A�C�h��
	m_animation.LoadAnimation("Data/Model/Monster/Animation/Run.mv1", run, 0, 1.0f); //!< ����

	// �����ƃ����X�^�[���ۂ��U����T���Ă���

	m_animation.LoadAnimation("Data/Model/Monster/Animation/Punch_1.mv1", attack_1, 0, 1.0f); //!< �U���P
	m_animation.LoadAnimation("Data/Model/Monster/Animation/rolling.mv1", rolling, 0, 1.0f); //!< ���[�����O
	m_animation.LoadAnimation("Data/Model/Monster/Animation/jump.mv1", jump, 0, 1.0f); //!< �W�����v
	// �ŏ��̓f�t�H���g�A�j���[�V���������Ă���
	m_animation.InitAttachAnimation(&m_model, idle, true);
}

//-----------------------------------------------
// �v���C���[�̈ړ��p�֐�
//-----------------------------------------------
void Monster::Move_Update()
{
	// ���񃊃Z�b�g
	m_run_flag = false;

	// �ړ��O�̍��W��U�ۑ����Ă���
	m_before_pos = m_transform.pos;

	// �x�[�X�N���X�̍X�V����
	// �ړ��̏��������ɓ����Ă���
	MoveUpdate(&m_run_flag);

	// run_flag ���オ���Ă�Ƃ�����
	// �v���C���[���[�h��RUN�ȊO�̎�
	if (m_run_flag && m_monster_mode != RUN)
	{
		// �A�j���[�V�����̐؂�ւ��t���O���グ��
		m_animation.m_anim_change_flag = true;
	}

	// �A�j���[�V�����ύX���\�Ȏ���
	if (m_animation.ChangeFlag(m_run_flag)) {
		// ����A�j���[�V�����ɕύX
		m_animation.ChangeAnimation(&m_model, run, true);
		// �A�j���[�V�������ς��������
		// �v���C���[���[�h�̐؂�ւ�������
		m_monster_mode = RUN;
	}
}

//-----------------------------------------------
// �ŏ��̍U���𔻒f����
//-----------------------------------------------
void Monster::Attack_First()
{

	// attack_flag ���オ���Ă�Ƃ�����
	// �v���C���[���[�h��ATTACK�ȊO�̎�
	if (m_attack_flag && m_monster_mode != ATTACK)
	{
		// �A�j���[�V�����̐؂�ւ��t���O���グ��
		m_animation.m_anim_change_flag = true;
	}

	// �U�����[�h�ɂ��Ă���
	m_monster_mode = ATTACK;
	m_animation.ChangeAnimation(&m_model, attack_1, false);

	// �U���A�j���[�V�����ԍ��̕ۑ�
	m_now_attack_anim = attack_1;
	// ���݂̍U���ԍ���ۑ�����
	m_now_attack = m_now_attack_anim - ATTACK_ANIM_START;

	m_stop_combo_flag = true;
}

//-----------------------------------------------
// �U���Ɋւ���X�V����
//-----------------------------------------------
void Monster::Attack_Update()
{
	// �R���{�����Ă����t���O�������Ă��鎞����
	if (m_stop_combo_flag)
	{
		// �R���{�֐����Ă�
		Combo_Update();
	}
}

//-----------------------------------------------
// �W�����v�U���Ɋւ��鏈��
//-----------------------------------------------
void Monster::Attack_Jump()
{
	// �^�[�Q�b�g�Ƃ̋���
	float distance = m_move.GetTargetDistance();
	// �^�[�Q�b�g�Ƃ̋��������ȏ�ɂȂ�����
	if (TARGET_DISTANCE <= distance)
	{
		// �W�����v�U�������Ăق����̂Ń����t���O������
		m_run_flag = false;

		// attack_flag ���オ���Ă�Ƃ�����
	   // �v���C���[���[�h��ATTACK�ȊO�̎�
		if (m_attack_flag && m_monster_mode != ATTACK)
		{
			// �A�j���[�V�����̐؂�ւ��t���O���グ��
			m_animation.m_anim_change_flag = true;
		}
		// �U�����[�h�ɂ��Ă���
		m_monster_mode = ATTACK;

		m_animation.ChangeAnimation(&m_model, jump, false);
		// �U���A�j���[�V�����ԍ��̕ۑ�
		m_now_attack_anim = jump;
		m_stop_combo_flag = true;
		// �W�����v������
		jump_num = STANDBY;
	}
}

//-----------------------------------------------
// �W�����v�U�����̏���
//-----------------------------------------------
void Monster::Jump_Update()
{
	if (m_animation.m_contexts[0].play_time >= 110.0f)
	{
		// �W�����v�t���O��������
		m_jump_flag = false; // �����鏈����
	}
}

//-----------------------------------------------
// �R���{�̔��f������֐�
//-----------------------------------------------
void Monster::Combo_Update()
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
		m_next_anim = Set_Rand_Attack();

		// �R���{�p�̃A�j���[�V����������
		m_animation.ActionComboChangeAnimation(&m_model, m_next_anim, false, &m_combo_flag);

		if (!m_combo_flag)
		{
			// ���݂̍U���A�j���[�V������ۑ�
			m_now_attack_anim = m_next_anim;
		}
	}
}


//-----------------------------------------------
// �s�������A�j���[�V�����������_���őI�Ԃ��߂̊֐�
//-----------------------------------------------
int Monster::Set_Rand_Attack()
{
	// ���ɍs���Ăق����A�j���[�V����������ϐ�
	int next_anim = 0;

	// �A�j���[�V���������܂閼�Ŗ������[�v
	while (true)
	{
		// ���̃A�j���[�V�����������_���œ����
		// �U���A�j���[�V�����X�^�[�g����U���A�j���[�V�����̍ő�܂ł�
		next_anim = GetRand(ATTACK_ANIM_MAX) + ATTACK_ANIM_START;
		// ���ɍs�������A�j���[�V�����ƍ��̃A�j���[�V���������Ԃ�����
		if (next_anim == m_now_attack_anim)
		{
			// �܂������_���œ���Ȃ���
			next_anim = GetRand(ATTACK_ANIM_MAX) + ATTACK_ANIM_START;
		}
		break;
	}

	// ���ɍs���Ăق����U���A�j���[�V������Ԃ�
	return next_anim;
}
