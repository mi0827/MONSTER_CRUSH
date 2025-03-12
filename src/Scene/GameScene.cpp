#include "src/WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/System/InputPad.h"

#include "src/System/Spotlight.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"
#include "src/Hit/HitStop.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"
#include "src/Camera.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/System/TargetMove.h"

#include "src/Character/CharacterBase.h"
#include "src/Character/MonsterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"
#include "src/Character/Monster.h"
#include "src/Character/Mutant.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"
#include "src/Field/Field.h"


#include "src/System/DamageAction.h"
#include "src/Action/Attack.h"


#include "src/System/Text.h"

#include "src/Scene/Base/Scene_Base.h"
#include "src/Scene/GameScene.h"


//---------------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------------
GameScene::GameScene()
{
}

//---------------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------------
GameScene::~GameScene()
{
}

//---------------------------------------------------------------------------
// �L�����N�^�[�̐ݒ�
//---------------------------------------------------------------------------
void GameScene::GameSceneInit()
{
	// �L�����N�^�[�̐ݒ�
	//if (m_player_num == SAMPLEPLAYER)
	//{
	//	m_player = new Hero;
	//}
	if (m_monster_num == MUTANT)
	{
		monster = new Mutant;
	}
	/*if (m_monster_num == MONSTER)
	{
		monster = new Monster;
	}*/

}


//---------------------------------------------------------------------------
// ��������
//---------------------------------------------------------------------------
void GameScene::Init()
{
	// �x�[�X�N���X�ŏ��������Ă����������̂̏�����
	BaseInit();


	GameSceneInit();

	// �J�����̏����ݒ�
	camera.PlayFieldInit();

	// �L�����N�^�[�̏�������
	CharacterInit();

	// �t�B�[���h�̏����� 
	m_field_2.Init();

	// �V���h�[�}�b�v�̐ݒ�
	ShadowMapInit();
	// ���݂̃V�[���̐ݒ�(�o�g���V�[��)
	m_now_scene = Battle;

	// �J�����̐ݒ�
	camera.SetCamera(camera.CAMERA_HEIGHT_MONSTER, camera.CAMERA_LENGTH_MAX);
}

//---------------------------------------------------------------------------
// �X�V����
//---------------------------------------------------------------------------
void GameScene::Update()
{

	

	// �Q�[���V�[���̒��łǂ̏�ʂ��ɂ���ď�����ς���
	switch (m_what_scene)
	{
	case entry: // �����X�^�[�̓o�ꉉ�o

		EntryUpdate();
		break;
	case battle: // �o�g���V�[��
		GameUpdate();
		break;
	case power_up: // �����X�^�[�̃p���[�A�b�v���o
		break;
	case result: // �o�g���V�[���̌�
		EndUpdate();
		break;
	}
	// �t�B�[���h�Ƃ̓����蔻��
	// ��U�����蔻���؂��Ă���
	HitField();

	// X�L�[�������ꂽ���ɃV�[���̕ύX������i�������̉��j
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		// ���ɍs���Ăق����V�[���̐ݒ������
		SetNextScene(End);

		m_scene_change_judge = true;
	}

}

//---------------------------------------------------------------------------
// �����X�^�[�̓o�ꉉ�o
//---------------------------------------------------------------------------
void GameScene::EntryUpdate()
{
	// �J�����̍X�V����
	camera.MoveCamera(&monster->m_transform.pos, CAMERA_DIRECTIN_FLET, CAMERA_ROT_SPEED);
	monster->EntryUpdate();
	// �J�����V�F�C�N���s��(�J�����̍X�V�����̌�łȂ��Ƃł��Ȃ�)
	camera.CameraShakeLimited(4.0f, (float)CHANGE_TIME);

	// �Q�[���p�b�h���ڑ�����Ă���Ƃ��̓Q�[���p�b�h��U����������
	if (GetJoypadNum() >= 1)
	{
		PadVidation(DX_INPUT_PAD1, 1000, CHANGE_TIME, -1);
	}
	

	// �t���[���̃J�E���g�𑝂₷
	m_count_flame++;
	// �t���[�����w��̒l�܂ő�������
	if (m_count_flame >= ONE_SECOND_FLAME)
	{
		// �^�C�}�[��i�߂�
		m_count_time++;
		// �J�E���g�����Z�b�g
		m_count_flame = 0;
	}
	// �^�C�}�[����莞�Ԃ�������(�T�b)
	if (m_count_time > CHANGE_TIME)
	{
		// �o�g�����X�^�[�Ƃ���
		m_what_scene = battle;
		// �^�C�}�[�����Z�b�g����
		m_count_time = 0;
		// �o�g���̂��߂ɃJ�����̌����̐ݒ�
		camera.SetCameraRot(m_player->m_transform.rot);
		// �����X�^�[��SE�Đ��\�t���O�𗧂Ă�
		// �{���Ȃ烂���X�^�[���ŏ����������ق����ǂ�
		monster->m_se.m_playing_flag = true;

	}
}

//---------------------------------------------------------------------------
// �o�g���V�[���ł̏���
//---------------------------------------------------------------------------
void GameScene::GameUpdate()
{
	// �J�����̍X�V����
	camera.UseCameraUpdate(m_camera_change, &m_player->m_transform.pos, &monster->m_transform.pos);

	
	// �q�b�g�X�g�b�v���N�����Ăق����Ƃ�������
	if (hit_stop.CheckHitStop() == false)
	{
		// �L�����N�^�[�̍X�V����
		CharacterUpdate();
	}

	// �����X�^�[�����K�U�������Ă����Ƃ��Ƀv���C���[���������邽�߂̏���
	CharacterKeepAway();

	// T�L�[�������ꂽ��J������ύX����
	if (PushHitKey(KEY_INPUT_T)|| IsPadOn(PAD_ID::PAD_R_PUSH))
	{
		if (m_camera_change)
		{
			m_camera_change = false;
		}
		else
		{
			m_camera_change = true;
		}
	}

	// �v���C���[��HP���O�ɂȂ�����
	if (m_player->m_hp_value <= 0)
	{
		// �v���C���[�̎��S��m�点��
		m_who_died = player_die;
		// �o�g�����I����������m�点��
		m_what_scene = result;
	}
	// �����X�^�[��HP���[���ɂȂ�����
	if (monster->m_hp_value <= 0)
	{
		// �����X�^�[�̎��S��m�点��
		m_who_died = monster_die;
		// �o�g�����I����������m�点��
		m_what_scene = result;
	}
}

//---------------------------------------------------------------------------
// �o�g�����I������Ƃ��̏���
//---------------------------------------------------------------------------
void GameScene::EndUpdate()
{
	// �t���[���̃J�E���g�𑝂₷
	m_count_flame++;
	// �t���[�����w��̒l�܂ő�������
	if (m_count_flame >= ONE_SECOND_FLAME)
	{
		// �^�C�}�[��i�߂�
		m_count_time++;
		// �J�E���g�����Z�b�g
		m_count_flame = 0;
	}
	// �^�C�}�[����莞�Ԃ�������(�T�b)
	if (m_count_time > CHANGE_TIME)
	{
		m_turn = FadeOut;
		// �^�C�}�[�����Z�b�g����
		m_count_time = 0;

	}
	// �L�����N�^�[�̍X�V����
	CharacterUpdate();
	if (m_turn == FadeOut)
	{
		FadeOutSceneChange(End);
	}


	// �ǂ���̃����X�^�[�����񂾂��ɂ���ď�����ς���
	switch (m_who_died)
	{
	case player_die: // �v���C���[�����񂾂Ƃ�
		// �v���C���[�𒆐S��
		// �E��]
		camera.MoveCamera(&m_player->m_transform.pos, CAMERA_DIRECTIN_RIGHT, CAMERA_ROT_SPEED);
		break;

	case monster_die: // �����X�^�[�����񂾂Ƃ�
		// �����X�^�[�𒆐S��
		// ����]
		camera.MoveCamera(&monster->m_transform.pos, CAMERA_DIRECTIN_FLET, CAMERA_ROT_SPEED);
		break;
	}

}

//---------------------------------------------------------------------------
// �`�揈��
//---------------------------------------------------------------------------
void GameScene::Draw()
{
	// �w��̃V���h�[�}�b�v�̃G���A��ݒ�
	SetShadowMapArea(m_shadowMap_handle_1, m_player->m_transform.pos);
	SetShadowMapArea(m_shadowMap_handle_2, monster->m_transform.pos);

	//-------------------------------------------------------------
		// �V���h�E�}�b�v�̍쐬�i�����Ŋe�I�u�W�F�N�g�̃V���h�[�}�b�v�̐ݒ�j
		//-------------------------------------------------------------
		// �V���h�E�}�b�v�ւ̕`��̏���
	ShadowMap_DrawSetup(m_shadowMap_handle_1);
	{
		// �v���C���[�̕`�揈��
		m_player->Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle_2);
	{
		// �v���C���[�̕`�揈��
		monster->Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{
		// �t�B�[���h�̕`��
		m_field_2.Draw();

	}
	// �V���h�E�}�b�v�ւ̕`����I��
	ShadowMap_DrawEnd();

	//-------------------------------------------------------------
	// �e���f���̕`��
	//-------------------------------------------------------------

	// �o�b�N�o�b�t�@�ɕ`�悷��
	SetDrawScreen(DX_SCREEN_BACK);

	// �J�����̕`�揈��
	camera.Draw();
	// ��̕`��
	SkyDraw();

	// �`��Ɏg�p����V���h�E�}�b�v��ݒ�
	SetUseShadowMap(1, m_shadowMap_handle_1);
	{
		m_player->Draw();
	}
	SetUseShadowMap(2, m_shadowMap_handle_2);
	{
		monster->Draw();
	}
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// �V���h�E�}�b�v�փL�����N�^�[���f���̕`��
		m_field_2.Draw();
		m_player->Draw();
		// �����X�^�[�̕`��
		monster->Draw();
	}

	UseShadowMapSet();

	// �X�e�[�^�X�o�[�̕`��
	StatusDraw();

	// ���U���g�̎������`�悷��
	if (m_what_scene == result)
	{
		// �o�g����̃��b�Z�[�W�̕`��
		VDMessage();
	}

	// �t�F�[�h�̕`�揈��
	FadeDraw();
	SetFontSize(50);
	static constexpr int color = 255;
	DrawFormatString(16, 400, color, "���� : %f", camera.inner_product);
	DrawFormatString(16, 350, color, "�O�� : %f", camera.cross_product.y);
	DrawFormatString(16, 450, color, "�O�� : %f", camera.m_mouse_wheel);
	//SetFontSize(50);
	/*static constexpr int color = 255;
	DrawStringF(16, 250, "player_flag", color, 0);
	DrawFormatString(16, 300, color, "Idle : %d", m_player->m_idle_flag);
	DrawFormatString(16, 350, color, "Run : %d", m_player->m_run_flag);
	DrawFormatString(16, 400, color, "Attack : %d", m_player->m_attack_flag);
	DrawFormatString(16, 450, color, "Damage : %d", m_player->m_damage_flag);
	DrawFormatString(16, 500, color, "Rolling : %d", m_player->m_rolling_flag);
	DrawFormatString(16, 550, color, "Counter : %d", m_player->m_counter_flag);

	DrawStringF(1500, 250, "monster_flag", color, 0);
	DrawFormatString(1500, 300, color, "Idle : %d", monster->m_idle_flag);
	DrawFormatString(1500, 350, color, "Run : %d", monster->m_run_flag);
	DrawFormatString(1500, 400, color, "Attack : %d", monster->m_attack_flag);
	DrawFormatString(1500, 450, color, "Stun : %d", monster->m_stun_flag);
	DrawFormatString(1500, 500, color, "StunNum : %3d", monster->m_stun_value);
	DrawFormatString(1500, 550, color, "Now : %d", monster->m_now_attack);*/

}

//---------------------------------------------------------------------------
// �I������
//---------------------------------------------------------------------------
void GameScene::Exit()
{

	//�@�V���h�[�}�b�v�̍폜
	ExitShadowMap();
	// �v���C���[�̏I������
	m_player->Exit();
	// �����X�^�[�̏I������
	monster->Exit();
}

//---------------------------------------------------------------------------
// �X�e�[�^�X�o�[�̕`��
//---------------------------------------------------------------------------
void GameScene::StatusDraw()
{
	m_player->StatusBarDraw();
	monster->StatusBarDraw();
	monster->StatusBarDraw();
}

//---------------------------------------------------------------------------
// ���C�g�̏����ݒ�
//---------------------------------------------------------------------------
void GameScene::LightInit()
{
}

//---------------------------------------------------------------------------
// �t�B�[���h�̓����蔻���
//---------------------------------------------------------------------------
void GameScene::HitField()
{

	// �t�B�[���h�̒n�ʃ��f���ƃL�����N�^�[�̓����蔻��
	HitGroundCharacter(&m_player->m_transform.pos, &m_field_2.m_field_model);

	// �����X�^�[�ƒn�ʂ̓����蔻��i�W�����v�����ł���j
	HitGroundCharacter(&monster->m_transform.pos, &m_field_2.m_field_model);

	//Vector3 pos = GetGroundPos(&monster->m_transform.pos, &field.m_field_model);
	//monster->SetJumpPos(pos);

	// �؂̃I�u�W�F�N�g�ƃv���C���[�̓����蔻��
	for (int i = 0; i < m_field_2.TREE_MAX; i++)
	{
		// �����X�^�[�ƃv���C���[�̈ړ��̓����蔻��
		if (CheckCapsuleHit(m_field_2.m_hit_tree[i], m_player->m_body))
		{
			m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &m_field_2.m_hit_tree[i]);
		}
	}


	// �t�F���X�ƃL�����N�^�[�̓����蔻��
	for (int i = 0; i < m_field_2.FENCE_MAX; i++)
	{
		if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
			m_field_2.m_hit_fence[i].m_box.hit_pos, m_field_2.m_hit_fence[i].m_box.half_size))
		{
			m_player->MoveHitUpdate(&m_field_2.m_hit_fence[i]);
		}
	}


	// �΂ƃL�����N�^�[�̓����蔻��
	for (int i = 0; i < m_field_2.STONE_MAX; i++)
	{
		if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
			m_field_2.m_hit_stone[i].m_box.hit_pos, m_field_2.m_hit_stone[i].m_box.half_size))
		{
			m_player->MoveHitUpdate(&m_field_2.m_hit_stone[i]);
		}
	}
}

//------------------------------------------
// �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
//------------------------------------------
void GameScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// �J�����̊��x�ݒ�
	camera.SetCameraSensi(mouse);
	// �L�����N�^�[�̃T�E���h�̒���
	m_player->m_se.SetSoundVolume(se);
	// �����X�^�[�̃T�E���h�̒���
	monster->m_se.SetSoundVolume(se);
	
}

//---------------------------------------------------------------------------
// �L�����N�^�[�̏�������
//---------------------------------------------------------------------------
void GameScene::CharacterInit()
{
	PlayerInit(hero);
	// �v���C���[�̏�������
	m_player->Init();
	// �����X�^�[�̏�������
	monster->Init();
}

//---------------------------------------------------------------------------
// �L�����N�^�\�̍X�V����
//---------------------------------------------------------------------------
void GameScene::CharacterUpdate()
{
	// �J�����̌������擾����
	m_camera_rot = camera.GetCameraRot();

	// �v���C���[�̍X�V����
	m_player->Update(&m_camera_rot);

	// �����X�^�[�̍X�V����
	monster->Update(&m_player->m_transform, m_player->m_hit_r, m_player->m_body, &camera);
	

	// �����X�^�[�ƃv���C���[�̈ړ��̓����蔻��
	if (CheckCapsuleHit(monster->m_body, m_player->m_body))
	{
		m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &monster->m_body);
	}

	// �U���֌W�̃A�b�v�f�[�g
	AttackUpdate();
}

//---------------------------------------------------------------------------
// �L�����N�^�\�̍U���Ɋւ���X�V����
//---------------------------------------------------------------------------
void GameScene::AttackUpdate()
{
	// �v���C���[�̍U��
	if (m_player->m_attack_flag)
	{
		// player�̍U���̎��Ɏ�肽�������蔻��ƃ����X�^�[�̑̂Ƃ̓����蔻����Ƃ�
		int num = m_player->m_now_attack;
		// �U���̓����蔻��s���Ă����Ƃ�����(�U���A�j���[�V�����̎w��̃t���[���Ԃ���)
		if (m_player->AttackHitGoodTiming(num))
		{
			// �����X�^�[�̃{�f�B�[�Ƃ̓����蔻����Ƃ�
			if (HitAttack(monster->m_body, m_player->m_attack_hit_damage[num]->attack_hit) == true)
			{
				// �����蔻�肪���������񂾂����̏�����ʂ�悤�ɂ���
				// �_���[�W�������s���Ă����t���O���オ���Ă�����
				if (m_player->m_can_hit_damage_flag)
				{
					// ��񂾂��ʂ��Ăق�������t���O��������
					m_player->m_can_hit_damage_flag = false;

					//--------------------------------------------
					// �����X�^�[�̒��g��ύX����
					// �_���[�W������͍̂U���A�j���[�V�����̊ԂɈ�񂾂�
					DamageCount(m_player->m_attack_hit_damage[num]->attack_damage, 5, &monster->m_hp_value);
					// �X�^���l���U�����󂯂��^�C�~���O�Ō��炷
					DamageCount(m_player->m_attack_hit_damage[num]->attack_damage, 5, &monster->m_stun_value);
					// �����X�^�[���U���󂯂����̏���
					monster->ComeAttackUpdate();
					//--------------------------------------------

					// �q�b�g�X�g�b�v���s���Ă������^�C�~���O�����s��
					if (m_player->m_attack_hit_damage[num]->can_hit_stop)
					{
						// �_���[�W���������^�C�~���O�Ńq�b�g�X�g�b�v�̃J�E���g�����Z�b�g����
						hit_stop.StopCountReset();
						// �q�b�g�X�g�b�v���󂯂�U�����󂯂��Ƃ��Ƀ����X�^�[�ə��K�������ăv���C���[���������邽�߂̃t���O���K�v
						//????

					}
				}
			}
		}
		else
		{
			// �����蔻����Ƃ�Ȃ��Ƃ��Ƀt���O��߂�
			m_player->m_can_hit_damage_flag = true;
		}
	}

	//�����X�^�[�̍U��
	if (monster->m_monster_mode == monster->ATTACK && monster->m_attack_flag)
	{
		// �����X�^�[�̍U�����Ɏg�����������蔻���player�̑̂Ƃ̓����蔻��
		int num = monster->m_now_attack;

		// �U���̓����蔻��s���Ă����Ƃ�����(�U���A�j���[�V�����̎w��̃t���[���Ԃ���)
		if (monster->AttackHitGoodTiming(num))
		{
			// �v���C���[�̃{�f�B�[�Ƃ̓����蔻����Ƃ�
			if (HitAttack(m_player->m_body, monster->m_attack_hit_damage[num]->attack_hit) == true)
			{
			
				// �����蔻�肪���������񂾂����̏�����ʂ�悤�ɂ���
				// �_���[�W�������s���Ă����t���O���オ���Ă�����
				if (monster->m_can_hit_damage_flag)
				{
					// ��񂾂��ʂ��Ăق�������t���O��������
					monster->m_can_hit_damage_flag = false;

					//--------------------------------------------
					// �v���C���[�̒��g��ύX����
					// �v���C���[�̍U���󂯂��t���O���������Ă���Ƃ�
					if (m_player->m_damage_flag == false)
					{
						// �v���C���[�̍U���󂯂��t���O���グ��
						m_player->m_damage_flag = true;
					}
					// �_���[�W������͍̂U���A�j���[�V�����̊ԂɈ�񂾂�
					// �����X�^�[�̓����蔻��ƃ_���[�W�̐ݒ�̓A�j���[�V�����������Ƃ����̂�����������
					DamageCount(monster->m_attack_hit_damage[num]->attack_damage, 5, &m_player->m_hp_value);
					// �v���C���[���U���󂯂����̏���
					m_player->ComeAttackUpdate();
					//--------------------------------------------
					// �Q�[���p�b�h���ڑ�����Ă���Ƃ��̓Q�[���p�b�h��U����������
					

					// �_���[�W���������^�C�~���O�Ńq�b�g�X�g�b�v�̃J�E���g�����Z�b�g����
					hit_stop.StopCountReset();
				}
			}
		}
		else
		{
			// �����蔻����Ƃ�Ȃ��Ƃ��Ƀt���O��߂�
			monster->m_can_hit_damage_flag = true;
		}
	}
}


//---------------------------------------------------------------------------
// �L�����N�^�\�̍U���Ɋւ���X�V����
// Victory(����)
// Defeat(�s�k)
//---------------------------------------------------------------------------
void GameScene::VDMessage()
{

	// �t�H���g�T�C�Y�̐ݒ�
	// �t�H���g�̃f�t�H���g�T�C�Y�̕ۑ�
	int default_font_size = GetFontSize();
	// �t�H���g�T�C�Y�̐ݒ�
	SetFontSize(80);
	// �`�敝�̎擾
	float w = GetDrawStringWidth(m_massage[m_who_died].message, -1);
	// ������̍����̎擾
	float h = GetFontSize();
	// �`����W
	m_massage_pos = { SCREEN_W / 2 - w / 2, SCREEN_H / 2 - h };
	DrawString(m_massage_pos.x, m_massage_pos.y, m_massage[m_who_died].message, GetColor(255, 128, 50));

	// �t�H���g�̃T�C�Y���f�t�H���g�T�C�Y�ɖ߂�
	SetFontSize(default_font_size);
}

//---------------------------------------------------------------------------
// �����X�^�[�̙��K�U���̍ۂɃv���C���[���������邽�߂̏���
//---------------------------------------------------------------------------
void GameScene::CharacterKeepAway()
{
	// �v���C���[�̈ړ����x��菭���������炢
	// �{���v���C���[�̈ړ��X�s�[�h�ɑ΂����������X�s�[�h�Ɏg�p�Ǝv���Ă���
	// float keepaway_speed = m_player.MOVE + 1;
	float keep_away_speed = 2.5f;

	// �����X�^�[�ƃ^�[�Q�b�g�i�v���C���[�Ƃ̋����j
	float distance = monster->m_move.GetTargetDistance();


	// �����X�^�[�����K���Ɉ��̃X�s�[�h�ŃL�����̍��W����������
	if (monster->m_roar_flag)
	{
		// ���̋������̊Ԃ�����ނ�����
		if (distance <= 120.0f)
		{
 			m_player->m_transform.pos.x += keep_away_speed * sinf(TO_RADIAN(m_player->m_transform.rot.y + 180));
			m_player->m_transform.pos.z += keep_away_speed * cosf(TO_RADIAN(m_player->m_transform.rot.y + 180));
		}
	}
}
