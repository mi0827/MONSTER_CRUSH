#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"

#include "src/Model/Model.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"


#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"
#include "src/Character/CharacterBase.h"
#include "SamplePlayer.h"



//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
SamplePlayer::SamplePlayer()
{
	// �ŏ��̓A�C�h����Ԃɂ��Ă���
	m_idle_flag = true;
	// �ŏ��̓A�C�h����Ԃɂ��Ă���
	m_player_mode = IDLE;


	// �ړ��̍ۂ̓����蔻��̃T�C�Y�̐ݒ�
	m_move_hit_size = { 3.0f,5.0f,3.0f };
}


//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
SamplePlayer::~SamplePlayer()
{
	// �I������������
	Exit();
}


//-----------------------------------------------
// ����������
//-----------------------------------------------
void SamplePlayer::Init()
{

	// �x�[�X�N���X�ōs���Ă��鏉�������Ă�
	CharacterBase::BaseInit(PLAYER_HIT_R, HP_MAX);

	// �v���C���[�̏������W
	SetCharacterPos({ 170,0,170 });
	// �����̐ݒ�
	SetCharacterRot({ 0,180.0f,0 });
	// ���f���̃X�P�[���̐ݒ�
	m_transform.scale.set(0.1f, 0.1f, 0.1f);


	// ���f���摜�̓ǂݍ���
	m_model.LoadModel("Data/Model/Player/Player.mv1");
	// �A�j���[�V�����̏����ݒ�
	AnimLoadInit();

	// �U���֘A�̃A�j���[�V�����ɂ��Ă̏���ۑ�����
	SetAttackInfo(ATTACK_ANIM_STAR, attack_anim_1, attack_kick_anim_1, COMBO_MAX);
	// �U���A�j���[�V�����̐����̓����蔻��̓��ꕨ���m�ۂ���
	NEWSetAttackHitDamage(ATTACK_ACTION);

	// �����蔻����Ƃ��Ăق����^�C�~���O�̂����Ă�
	SetHitTimeInit();

	// �X�e�[�^�X�o�[�̐ݒ�
	StatusBarInit();
}


//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void SamplePlayer::Update(Vector3* camera_rot)
{


	// HP�̒l�����������ǂ���
	m_hp.Update(m_hp_value);



	// �����Ă邩����ł邩�ŏ�����ς���
	switch (m_life_and_death)
	{
	case alive: // �����Ă鎞�̏���

		// �����Ă鎞�̍X�V����
		LiveUpdate(camera_rot);
		// �v���C���[��HP���O��葽������
		if (m_hp_value > 0)
		{
			// �����Ă�
			m_life_and_death = alive;
		}
		else // ����ȊO�̎�
		{
			// ����
			m_life_and_death = die;
			// �v���C���[�̏�Ԃ���DIE�ύX
			m_player_mode = DIE;
			// �A�j���[�V�����̐؂�ւ��t���O���グ��
			m_animation.m_anim_change_flag = true;

			// ���񂾃A�j���[�V����������
			if (m_animation.ChangeFlag(true))
			{
				// �A�j���[�V�������~�ɕύX����
				m_animation.ChangeAnimation(&m_model, die, true);
			}
		}

		break;

	case die: // ���񂾂Ƃ��̏���
		DieUpdate();
		break;

	default:
		break;
	}


	// �A�j���[�V�����̍Đ�
	m_animation.PlayAnimation(&m_model, m_combo_flag);

	// �����蔻��̍X�V����
	CDUpdate();

	// �t���O�Ǘ��p�֐�
	PlayerMode(m_player_mode);

	// �n�ʂɖ��܂��Ă��܂������̂��߂̏���
	if (m_transform.pos.y <= 0)
	{
		m_transform.pos.y = 0;
	}

}


//-----------------------------------------------
// �����Ă���Ƃ��̍X�V����
//-----------------------------------------------
void SamplePlayer::LiveUpdate(Vector3* camera_rot)
{
	// ���[�����O�͖��G�Ȃ̂Ń��[�����O�̊ԂɍU�����󂯂Ȃ�
	// �v���C���[�̃��[�h���J�E���^�[�łȂ��Ƃ�
	// �v���C���[���U�����󂯂����̃`�F�b�N
	if (!m_rolling_flag || m_player_mode != COUNTER)
	{
		CheckHitDamage();
	}


	// �ҋ@��Ԃ܂��͑���̎�����
	if (m_idle_flag == true || m_run_flag == true)
	{
		// ���[�����O�������ȊO ���� �G����̃_���[�W��H������t���O�������ĂȂ�������
		if (m_rolling_flag == false && m_damage_flag == false && m_counter_flag == false)
		{
			// �ړ�����
			MoveAction(run, PLAYER_MOVE_SPEED, camera_rot);
		}
	}

	// �_���[�W��H������t���O�������Ă��Ȃ�������
	if (m_damage_flag == false && !m_counter_flag)
	{
		// ���[�����O�̐؂�ւ�
		RollingActionStart();
	}


	// �L�����N�^�[�̏�Ԃɂ���čs����ς���
	switch (m_player_mode)
	{
	case IDLE: // �A�C�h���̎�
		if (m_idle_flag)
		{
			//Player_Mode(IDLE);
			// �A�C�h����ԂȂ̂ŃA�C�h���t���O�𗧂Ă�
			m_idle_flag = true;

			// �A�j���[�V�����ύX���\�Ȏ���
			if (m_animation.ChangeFlag(m_idle_flag))
			{
				// �A�j���[�V�������~�ɕύX����
				m_animation.ChangeAnimation(&m_model, idle, true);
			}
		}
		// �ŏ��̍U���𔻒f����
		AttackFirst();
		break;
	case RUN: // �ړ��� 
		// �ҋ@�t���O�𖈉񃊃Z�b�g
		m_idle_flag = false;
		// run_flagf�t���O������������
		if (m_run_flag == false)
		{
			// �ҋ@�t���O���グ��
			m_idle_flag = true;
			// �A�j���[�V�����ύX���s����悤�ɂ���
			m_animation.m_anim_change_flag = true;
			// �v���C���[�̏�Ԃ�IDLE�ɕύX
			m_player_mode = IDLE;
		}

		// �ŏ��̍U���𔻒f����
		AttackFirst();

		break;
	case ROLLING: // ���[�����O�A�N�V���������Ă��鎞
		RollingActionUpdate(rolling, (int)PLAYER_ROLLING_SPEED);

		break;
	case COUNTER:
		CounterAction(counter_anim);

		break;

	case ATTACK: // �U�����J��o���Ă��鎞

		// �R���{�t���O�������Ă��Ȃ���
		// �U���A�j���[�V�����̍Đ����I����Ă�����
		if (m_combo_flag == false && m_animation.m_contexts[0].is_playing == false)
		{
			// �ҋ@���[�h�ɂ��Ă���
			m_player_mode = IDLE;
		}
		// �J�E���^�[�U�����s���Ă���ꍇ
		// �������邱�Ƃɂ���ăJ�E���^�[�U������ɉ�����ł��Ȃ��Ȃ�
		if (m_counter_flag && m_animation.m_contexts[0].is_playing == false)
		{
			// �J�E���^�[�t���O������
			m_counter_flag = false;
			// �ҋ@���[�h�ɂ��Ă���
			m_player_mode = IDLE;
		}

		// �U���p�̊֐�
		AttackUpdate();
		break;

	case HIT_DAMAGE: // �_���[�W���󂯂���
		HitDamageUpdate(hit_damage);
		break;
	}
}

//-----------------------------------------------
// ���񂾁i�������j���̍X�V����
//-----------------------------------------------
void SamplePlayer::DieUpdate()
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
void SamplePlayer::Draw()
{

	//===================
	// �J�v�Z���̕`��i���j�i��ŏ����j
	//===================
	// �U���t���O���������Ă�����
	if (m_attack_flag)
	{

		// �U���̓����蔻��s���Ă����Ƃ�����
		if (AttackHitGoodTiming(m_now_attack))
		{

			// �����蔻���`��
			if (m_now_attack == attack_counter)
			{
				m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
			}
			m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
			
		}
	}
	/*m_body.Draw();
	m_right_hand.Draw();
	m_left_hand.Draw();
	m_right_feet.Draw();
	m_left_feet.Draw();*/

	// ���f���̕`�� (�`�����ɂ��Ȃ��Ɠ����蔻�肪������������)

	m_model.DrawModel(&m_transform);



	//m_move_hit_box.Draw(255, 255);
}


//-----------------------------------------------
// �I������
//-----------------------------------------------
void SamplePlayer::Exit()
{

}


//-----------------------------------------------
// �����蔻��
//-----------------------------------------------
void SamplePlayer::CDUpdate()
{
	// �L�����{�̂̓����蔻��̃J�v�Z���i��ŏ����j
	m_body.CreateNodoCapsule(&m_model, 6, 65, 3.5f);
	// �E��̂����蔻��
	m_right_hand.CreateNodoCapsule(&m_model, 32, 46, 2.5f);
	// ����̓����蔻��
	m_left_hand.CreateNodoCapsule(&m_model, 8, 22, 2.5f);
	// �E���̓����蔻��
	m_right_feet.CreateNodoCapsule(&m_model, 60, 64, 2.5f);
	// �����̓����蔻��
	m_left_feet.CreateNodoCapsule(&m_model, 55, 59, 2.5f);

	// ���̓����蔻��𒲂ׂ邽�߂ɍ��������
	{
		// �J�v�Z���̍��W�P
		Vector3 top_pos;
		top_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 10,
			m_transform.pos.y + 5,
			m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 10);

		// �J�v�Z���̍��W�Q
		Vector3 under_pos;
		under_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 5,
			m_transform.pos.y + 5,
			m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 5);


	}
	// �U�����̓����蓖������̕ۑ��ƃ_���[�W�̐ݒ�
	SetHitDamage(m_left_hand, m_attack_damage[attack_punch_1], (attack_punch_1));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_2], (attack_punch_2));
	SetHitDamage(m_right_hand, m_attack_damage[attack_punch_3], (attack_punch_3));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_1], (attack_kick_1));
	SetHitDamage(m_left_feet, m_attack_damage[attack_kick_2], (attack_kick_2));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_3], (attack_kick_3));
	SetHitDamage(m_right_feet, m_attack_damage[attack_counter], (attack_counter));
}


//-----------------------------------------------
// �X�e�[�^�X�o�[�̐ݒ�p�֐�
//-----------------------------------------------
void SamplePlayer::StatusBarInit()
{
	// HP�̐ݒ�
	m_hp.Set({ 50, SCREEN_H - 50 }, { 500,25 }, &m_hp_value, true);
	m_hp.SetColor(50, 255, 50, &m_hp.m_color);
	m_hp.SetColor(128, 128, 128, &m_hp.m_back_color);
	m_hp.SetColor(0, 0, 0, &m_hp.m_line_color);
	m_hp.SetColor(255, 0, 0, &m_hp.m_character_color);
	m_hp.SetName("HP");
}


//-----------------------------------------------
// �X�e�[�^�X�o�[�`��p�֐�
//-----------------------------------------------
void SamplePlayer::StatusBarDraw()
{

	m_hp.Update(m_hp_value);
	//===================
	// UI�̕`��
	//===================
	m_hp.Draw();
}


//-----------------------------------------------
// �����蔻����s���ė~�����^�C�~���O��ۑ�����֐�
// �S���̍U���ɓ����蔻��ɐݒ肷��(���Ƃł���)
//-----------------------------------------------
void SamplePlayer::SetHitTimeInit()
{
	for (int i = 0; i < attack_max; i++)
	{
		SetHitTiming(attack_frame[i].start_frame, attack_frame[i].end_frame, attack_frame[i].can_hit_stop, i);
	}


}


//-----------------------------------------------
// �A�j���[�V�����̏�������
//-----------------------------------------------
void SamplePlayer::AnimLoadInit()
{
	// �A�j���[�V�����̏����ݒ�
	m_animation.InitAnimation(anim_max, idle);
	// �A�j���[�V�����̓ǂݍ���
	m_animation.LoadAnimation("Data/Model/Player/Animation/Player_Idle.mv1", idle, 1, 1.0f); //!< �A�C�h��
	m_animation.LoadAnimation("Data/Model/Player/Animation/Player_Run.mv1", run, 1, 1.0f);   //!< ����
	m_animation.LoadAnimation("Data/Model/Player/Animation/rolling.mv1", rolling, 1, 2.0f);     //!< ���[�����O
	m_animation.LoadAnimation("Data/Model/player/Animation/hit_damage.mv1", hit_damage, 1, 1.0f); //!< �_���[�W��H�������
	m_animation.LoadAnimation("Data/Model/Player/Animation/die.mv1", die, 1, 1.0f);            //!< ���񂾂Ƃ�
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Punch.mv1", attack_anim_1, 1, 1.0f);  //!< �U���P
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Punch2.mv1", attack_anim_2, 1, 2.0f); //!< �U���Q
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Punch3.mv1", attack_anim_3, 1, 2.0f); //!< �U���R
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick2.mv1", attack_kick_anim_1, 1, 1.5f); //<! �L�b�N�U��
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick3.mv1", attack_kick_anim_2, 1, 1.5f); //<! �L�b�N�U��
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick4.mv1", attack_kick_anim_3, 1, 1.5f); //<! �L�b�N�U��
	m_animation.LoadAnimation("Data/Model/Player/Animation/Attack/Kick4.mv1", counter_anim, 1, 1.5f);       //<! �J�E���^�[�U��
	// �ŏ��̓f�t�H���g�A�j���[�V���������Ă���
	m_animation.InitAttachAnimation(&m_model, idle, true);
}

//-----------------------------------------------
// �G�t�F�N�g�̏�������
//-----------------------------------------------
void SamplePlayer::EffectLoadInit()
{
	
}

//-----------------------------------------------
// �t���O�Ǘ��p�֐�
// ���̊֐����Ȃ��ƐF�X�ȃo�O���o��
//-----------------------------------------------
void SamplePlayer::PlayerMode(int mode)
{
	switch (mode)
	{
	case IDLE:
		m_idle_flag = true;
		m_run_flag = false;
		m_attack_flag = false;
		m_rolling_flag = false;
		m_counter_flag = false;
		break;
	case RUN:
		m_idle_flag = false;
		m_run_flag = true;
		m_attack_flag = false;
		m_rolling_flag = false;
		m_counter_flag = false;
		break;
	case ATTACK:

		m_idle_flag = false;
		m_run_flag = false;
		m_rolling_flag = false;
		m_attack_flag = true;
		m_counter_flag = false;
		break;
	case ROLLING:
		m_idle_flag = false;
		break;
	case HIT_DAMAGE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_rolling_flag = false;
		m_damage_flag = true;

		break;
	case DIE:
		m_idle_flag = false;
		m_run_flag = false;
		m_attack_flag = false;
		m_damage_flag = false;
		m_rolling_flag = false;
		break;
	}
}

