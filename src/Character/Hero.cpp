#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"



#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"
#include "src/Hit/Hit.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/Character/CharacterBase.h"
#include "Hero.h"



//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
Hero::Hero()
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
Hero::~Hero()
{
	// �I������������
	Exit();
}

//-----------------------------------------------
// ����������
//-----------------------------------------------
void Hero::Init()
{

	// �x�[�X�N���X�ōs���Ă��鏉�������Ă�
	CharacterBase::BaseInit(PLAYER_HIT_R, HP_MAX);



	// �v���C���[�̏������W
	SetCharacterPos({ 250,0,250 });
	// �����̐ݒ�
	SetCharacterRot({ 0,180.0f,0 });
	// ���f���̃X�P�[���̐ݒ�
	m_transform.scale.set(0.1f, 0.1f, 0.1f);

	// �L�����N�^�[���f���摜�̓ǂݍ���
	m_model.LoadModel("Data/Model/Hero/Hero.mv1");
	// �L�����N�^�[�̃}�e���A������
	// ���f���̃}�e���A�����m��
	// �}�e���A���̃A���r�G���g��������Ǝ����ɂ������Ă���e�����邭�Ȃ�
	m_model.SetMaterialAmbient({ 1.0f,1.0f,1.0f,0.0f });

	// �A�j���[�V�����̏����ݒ�
	AnimLoadInit();
	// �G�t�F�N�g�̏����ݒ�
	EffectLoadInit();
	// �T�E���h�̏�����
	SELoadInit();
	// �U���֘A�̃A�j���[�V�����ɂ��Ă̏���ۑ�����
	SetAttackInfo(ATTACK_ANIM_STAR, attack_sword_anim_1, attack_sword_anim_4, COMBO_MAX);
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
void Hero::Update(Vector3* camera_rot)
{

	// �ړ��O�̍��W��U�ۑ����Ă���
	m_before_pos = m_transform.pos;
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
void Hero::LiveUpdate(Vector3* camera_rot)
{
	// ���f����Y����]�ɐ����������Ă���
	m_model.RotationYLimits();
	// ���[�����O�͖��G�Ȃ̂Ń��[�����O�̊ԂɍU�����󂯂Ȃ�
	// �v���C���[�̃��[�h���J�E���^�[�łȂ��Ƃ�
	// �v���C���[���U�����󂯂����̃`�F�b�N
	if (!m_rolling_flag || m_player_mode != COUNTER)
	{
		CheckHitDamage();
		if (m_player_mode== HIT_DAMAGE)
		{
			// �Đ�����SE���~�߂�
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}
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
		if (m_player_mode == ROLLING)
		{
			// �Đ�����SE���~�߂�
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}
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
		else
		{
			// �����t���O�������Ă���Ƃ�����
			// SE�̍Đ�
			SEUpdate(run_se_info);
		}
		
		// �ŏ��̍U���𔻒f����
		AttackFirst();
		// �����t���O�����������Ƃ����U���Ɉڍs����^�C�~���O��
		if (m_player_mode == ATTACK || m_run_flag == false)
		{
			// �Đ�����SE���~�߂�
			m_se.StopSound();
			m_se.m_playing_flag = true;
		}

		break;
	case ROLLING: // ���[�����O�A�N�V���������Ă��鎞

		// ���[�����O�̍X�V����
		RollingActionUpdate(rolling, static_cast<int>(PLAYER_ROLLING_SPEED));
		

		break;
	case COUNTER:

		// �J�E���^�[�U�����Z�b�g
		CounterAction(counter_anim);

		m_effect.StopEffect();
		// �G�t�F�N�g�Đ��\��Ԃɂ��Ă���
		m_effect.m_play_effect_flag = true;

		break;

	case ATTACK: // �U�����J��o���Ă��鎞

		// �R���{�t���O�������Ă��Ȃ���
		// �U���A�j���[�V�����̍Đ����I����Ă�����
		if (m_combo_flag == false && m_animation.m_contexts[0].is_playing == false)
		{
			// �ҋ@���[�h�ɂ��Ă���
			m_player_mode = IDLE;
			m_combo.ChangeComboMode(m_combo.COMBO_FINISH);

		}

		// �J�E���^�[�U�����s���Ă���ꍇ
		// �������邱�Ƃɂ���ăJ�E���^�[�U�����ɉ�����ł��Ȃ��Ȃ�
		if (m_counter_flag && m_animation.m_contexts[0].is_playing == false)
		{
			// �J�E���^�[�t���O������
			m_counter_flag = false;
			// �ҋ@���[�h�ɂ��Ă���
			m_player_mode = IDLE;
		}

		// �U���p�̊֐�
		AttackUpdate();


		// �U���̃G�t�F�N�g���L�b�N�ȊO�̎�
		// �U�����̃G�t�F�N�g�͍U���ƃ^�C�~���O�����킹��
		if (m_animation.m_contexts[0].play_time >= m_effect_info[m_now_attack].effect_start_anim_frame &&
			m_effect.m_play_effect_flag == true)
		{
			m_effect.StopEffect();
			if (m_now_attack == attack_kick_1 || m_now_attack == attack_kick_2)
			{
				// �L�b�N�U���̎��̃G�t�F�N�g
				EffectUpdate(attack_kick_effect, m_now_attack);
			}
			else
			{
				// ���ōU������Ƃ��̃G�t�F�N�g
				EffectUpdate(attack_sword_effect, m_now_attack);
			}
		}
		// �U���ɂ������T�E���h���Đ�
		if (m_animation.m_contexts[0].play_time >= m_se_info[m_now_attack].se_start_frame)
		{
			SEUpdate(m_now_attack);
		}
		

		if (m_animation.m_contexts[0].is_playing == false)
		{
			// �U���A�j���[�V�������I���������
			// ���̃G�t�F�N�g���Đ��ł���悤�ɂ���
			m_effect.m_play_effect_flag = true;
			// ����SE���Đ��ł���悤�ɂ���
			m_se.m_playing_flag = true;
			// �Đ�����SE���I��点��
			m_se.StopSound();
		}

		break;

	case HIT_DAMAGE: // �_���[�W���󂯂���
		HitDamageUpdate(hit_damage);
		break;
	}
}

//-----------------------------------------------
// ���񂾁i�������j���̍X�V����
//-----------------------------------------------
void Hero::DieUpdate()
{
	// ���񂾃A�j���[�V�������ǂ�Ȃ��悤��
	// ���̂Ƃ���܂Ői�񂾂�
	if (m_animation.m_contexts[0].play_time >= 140)
	{
		// �����O�ɖ߂�
		m_animation.m_contexts[0].play_time = 140;
	}
}

//-----------------------------------------------
// �`�揈��
//-----------------------------------------------
void Hero::Draw()
{
	//===================
	// �J�v�Z���̕`��i���j�i��ŏ����j
	//===================
	// �U���t���O���������Ă�����
	//if (m_attack_flag)
	//{
	//	// �U���̓����蔻��s���Ă����Ƃ�����
	//	if (AttackHitGoodTiming(m_now_attack))
	//	{
	//		// �����蔻���`��
	//		m_attack_hit_damage[m_now_attack]->attack_hit.Draw();
	//	}
	//}
	// m_body.Draw();
	/*m_right_hand.Draw();
	m_left_hand.Draw();
	m_right_feet.Draw();
	m_left_feet.Draw();*/
	//m_sword.Draw();
	// ���f���̕`�� (�`�����ɂ��Ȃ��Ɠ����蔻�肪������������)
	m_model.DrawModel(&m_transform);
}

//-----------------------------------------------
// �I������
//-----------------------------------------------
void Hero::Exit()
{
	// �Đ�����SE���~�߂�
	m_se.StopSound();
}

//-----------------------------------------------
// �����蔻��
//-----------------------------------------------
void Hero::CDUpdate()
{
	// �L�����{�̂̓����蔻��̃J�v�Z���i��ŏ����j
	//m_body.CreateNodoCapsule(&m_model, 6, 65, 3.5f);
	m_body.CreateCapsule(m_transform.pos, { 0.0f, 20.0f, 0.0f },3.5f);
	// �E��̂����蔻��
	m_right_hand.CreateNodoCapsule(&m_model, 32, 46, 2.5f);
	// ����̓����蔻��
	m_left_hand.CreateNodoCapsule(&m_model, 8, 22, 2.5f);
	// �E���̓����蔻��
	m_right_feet.CreateNodoCapsule(&m_model, 60, 64, 2.5f);
	// �����̓����蔻��
	m_left_feet.CreateNodoCapsule(&m_model, 55, 59, 2.5f);

	// ���̓����蔻��𒲂ׂ邽�߂ɍ��������
	// �J�v�Z���̍��W�P
	Vector3 top_pos;
	top_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 11,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 11);

	// �J�v�Z���̍��W�Q
	Vector3 under_pos;
	under_pos.set(m_transform.pos.x + sinf(TO_RADIAN(m_transform.rot.y)) * 8,
		m_transform.pos.y + 8,
		m_transform.pos.z + cosf(TO_RADIAN(m_transform.rot.y)) * 8);
	// ���p�̓����蔻��̍쐬
	m_sword.CreateCapsuleCoordinatePos(top_pos, under_pos, 10);

	// �U�����̓����蓖������̕ۑ��ƃ_���[�W�̐ݒ�
	SetHitDamage(m_sword, m_attack_damage[attack_sword_1], (attack_sword_1));
	SetHitDamage(m_sword, m_attack_damage[attack_sword_2], (attack_sword_2));
	SetHitDamage(m_sword, m_attack_damage[attack_sword_3], (attack_sword_3));
	SetHitDamage(m_sword, m_attack_damage[attack_sword_4], (attack_sword_4));
	SetHitDamage(m_right_feet, m_attack_damage[attack_kick_1], (attack_kick_1));
	SetHitDamage(m_left_feet, m_attack_damage[attack_kick_2], (attack_kick_2));
	SetHitDamage(m_sword, m_attack_damage[attack_counter], (attack_counter));

}

//-----------------------------------------------
// �X�e�[�^�X�o�[�̐ݒ�p�֐�
//-----------------------------------------------
void Hero::StatusBarInit()
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
// �X�e�[�^�X�`��p
//-----------------------------------------------
void Hero::StatusBarDraw()
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
void Hero::SetHitTimeInit()
{
	for (int i = 0; i < attack_max; i++)
	{
		SetHitTiming(attack_frame[i].start_frame, attack_frame[i].end_frame, attack_frame[i].can_hit_stop, i);
	}

}


//-----------------------------------------------
// �A�j���[�V�����̏�������
//-----------------------------------------------
void Hero::AnimLoadInit()
{
	// �A�j���[�V�����̏����ݒ�
	m_animation.InitAnimation(anim_max, idle);
	// �A�j���[�V�����̓ǂݍ���
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Idle.mv1", idle, 0, 1.0f); //!< �A�C�h��
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Run.mv1", run, 0, 1.0f);   //!< ����
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Rolling.mv1", rolling, 0, 1.5f);   //!< ���[�����O
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Hit_damage.mv1", hit_damage, 0, 1.0f);   //!< �U�������������Ƃ��̃_���[�W
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Die.mv1", die, 0, 1.0f);   //!< ���񂾂Ƃ�
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword1.mv1", attack_sword_anim_1, 0, 2.5f); //!< ���U���P 
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword2.mv1", attack_sword_anim_2, 0, 2.5f); //!< ���U���Q
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword3.mv1", attack_sword_anim_3, 0, 2.5f); //!< ���U���R
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/AttackSword4.mv1", attack_sword_anim_4, 0, 2.3f); //!< ���U���S
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/Kick1.mv1", attack_kick_anim_1, 0, 2.3f); //<! �L�b�N�U���P
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/Kick2.mv1", attack_kick_anim_2, 0, 2.3f); //<! �L�b�N�U���Q
	m_animation.LoadAnimation("Data/Model/Hero/Animation/Attack/counter3.mv1", counter_anim, 0, 3.5f);     //<! �J�E���^�[�U��
	// �ŏ��̓f�t�H���g�A�j���[�V���������Ă���
	m_animation.InitAttachAnimation(&m_model, idle, true);
}

//-----------------------------------------------
// �G�t�F�N�g�̏�������
//-----------------------------------------------
void Hero::EffectLoadInit()
{
	// �G�t�F�N�g������
	// �G�t�F�N�g�̍ő吔��ݒ�
	m_effect.NewArraySecure(effect_max);
	// �G�t�F�N�g�̓ǂݍ���
	m_effect.LoadEffect("Data/Model/Hero/Effect/Simple_Ribbon_Sword.efkefc", attack_sword_effect, 1.0f); // ���U�����̃G�t�F�N�g
	m_effect.LoadEffect("Data/Model/Hero/Effect/kick.efkefc", attack_kick_effect, 1.0f); // �L�b�N�U�����̃G�t�F�N�g
	m_effect.LoadEffect("Data/Model/Hero/Effect/HitDamage.efkefc", damage_effect, 1.0f); // �_���[�W���󂯂����̃G�t�F�N�g
	m_effect.LoadEffect("Data/Model/Hero/Effect/rolling.efkefc", rolling_effect, 1.0f); // ���[�����O���̃G�t�F�N�g
}

//-----------------------------------------------
// �G�t�F�N�g�̍X�V����
//-----------------------------------------------
void Hero::EffectUpdate(int effect_num, int effect_info_num)
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
	m_effect.SetEffectRotPos(m_transform.pos, m_effect_info[effect_info_num].pos, m_transform.rot);
	// �G�t�F�N�g�̃T�C�Y�����킹��
	m_effect.SetEffectSize(m_effect_info[effect_info_num].size);
	// �G�t�F�N�g�̌��������킹��
	// �v���C���[�̌����ɂ����킹��
	m_effect.SetEffectRot(m_effect_info[effect_info_num].rot.x, m_effect_info[effect_info_num].rot.y + m_transform.rot.y, m_effect_info[effect_info_num].rot.z);

}

//-----------------------------------------------
// SE�̏�������
//-----------------------------------------------
void Hero::SELoadInit()
{
	// SE�̏�����
	// SE�̍ő吔��ݒ�
	m_se.NewArraySecureSound(se_max);
	// SE�̓ǂݍ���
	m_se.LoadSound("Data/Model/Hero/SE/SwordAttack_1.mp3", sword_attack_se_1); // ���U���P
	m_se.LoadSound("Data/Model/Hero/SE/SwordAttack_3.mp3", sword_attack_se_2); // ���U���Q
	m_se.LoadSound("Data/Model/Hero/SE/KickAttack_1.mp3", kick_attack_se_1); // �L�b�N�U���P
	m_se.LoadSound("Data/Model/Hero/SE/KickAttack_2.mp3", kick_attack_se_2); // �L�b�N�U���Q
	m_se.LoadSound("Data/Model/Hero/SE/Run_1.mp3", run_se); // ����
	m_se.LoadSound("Data/Model/Hero/SE/Rolling.mp3", rolling_se); // ���[�����O
	m_se.LoadSound("Data/Model/Hero/SE/Damage.mp3", damage_se); // �U�����󂯂����̃T�E���h

}

//-----------------------------------------------
// SE�̍X�V����
//-----------------------------------------------
void Hero::SEUpdate(int se_info_num)
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

//-----------------------------------------------
// �t���O�Ǘ��p�֐�
// ���̊֐����Ȃ��ƐF�X�ȃo�O���o��
//-----------------------------------------------
void Hero::PlayerMode(int mode)
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
		m_attack_flag = false;
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

//-----------------------------------------------
// �U�����󂯂����̍X�V����
//-----------------------------------------------
void Hero::ComeAttackUpdate()
{
	// �G�t�F�N�g�Đ��\��Ԃɂ���
	m_effect.m_play_effect_flag = true;
	// �_���[�W���󂯂����̃G�t�F�N�g
	EffectUpdate(damage_effect, damage_effect_info);

	//�Đ�����SE���I��点��
	//m_se.StopSound();
	// SE�Đ��\�ɂ��Ă���
	//m_se.m_playing_flag = true;
	// �U�����󂯂����p��SE�̍Đ�
	SEUpdate(damage_se_info);

}


