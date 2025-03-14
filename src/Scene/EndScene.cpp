#include "src/WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Animation/Animation.h"
#include "src/Effect/Effect.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"
#include "src/Field/Field.h"

#include "src/Sound/BGM.h"
#include "src/Sound/SE.h"
#include "src/Camera.h"
#include "src/System/Text.h"
#include "Base/Scene_Base.h"
#include "EndScene.h"


//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
EndScene::EndScene()
{
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
EndScene::~EndScene()
{
}

//------------------------------------------
// ��������
//------------------------------------------
void EndScene::Init()
{
	// �x�[�X�N���X�ŏ��������Ă����������̂̏�����
	BaseInit();


	// �t�B�[���h�̏�����
	m_field_2.Init();


	// �J�����̏����ݒ�
	camera.PlayFieldInit();

	PlayerInit(hero);
	// �v���C���[�̏����ݒ� 
	m_player->Init();

	// �V���h�[�}�b�v�̐ݒ�
	ShadowMapInit();

	// ���݂̃V�[���̐ݒ�(�G���h�V�[��)
	m_now_scene = End;


	// �G���h�V�[���Ŏg���p�̃e�L�X�g�̓ǂݍ���
	m_text.LoadText("Data/Text/End.txt", text_max);



	// SE�̏�����
	m_se.NewArraySecureSound(se_max);
	m_se.LoadSound("Data/SE/menu_selection.mp3", se_1); // �e���j���[��I�����Ă���Ƃ�



}


//------------------------------------------
// �X�V����
//------------------------------------------
void EndScene::Update()
{
	// �}�E�X�̕\����Ԃ̐ݒ�
	SetMouseDispFlag(TRUE);
	// �}�E�X�̌Œ����������
	ChangeMousePosMove(TRUE);
	// �v���C���[�̏�����ɃJ������z�u���邽�߂̕ϐ�
	Vector3 target_pos;

	// �}�E�X�̍��W���擾
	Vector2 mouse_pos = { float(GetMouseX()),float(GetMouseY()) };

	// �ǂ̃��j���[�Ɠ������Ă��邩�𒲂ׂ�
	Vector2 pos2;

	switch (m_turn)
	{
	case Main:
		// �o�߃t���[����i�߂�
		m_frame_count++;
		// �w��̃t���[���ɒB������
		if (m_frame_count >= END_SCENE_FRAME_MAX)
		{
			// �t���[�����Z�b�g
			m_frame_count = 0;
			// �^�C�g���Ɉړ�����悤�ɂ���
			m_change_scene = Title;
			m_turn = FadeOut;
			m_start_flag = false;
		}


		// �J�����̌������擾����
		m_camera_rot = camera.GetCameraRot();
		// �v���C���[�̍X�V����
		m_player->Update(&m_camera_rot);
		// �J�����̍X�V����
		target_pos = { m_player->m_transform.pos.x,m_player->m_transform.pos.y + 20,m_player->m_transform.pos.z };
		camera.MoveCamera(&target_pos, 10, true);


		// �Q�[���p�b�h�̂ł̑I��
		if (IsPadOn(PAD_ID::PAD_D_LEFT))
		{
			m_select_num = text1;
			// �������Ă���t���O�𗧂Ă�
			m_hit_select_flag = true;
		}
		else if (IsPadOn(PAD_ID::PAD_D_RIGHT))
		{
			m_select_num = text2;
			// �������Ă���t���O�𗧂Ă�
			m_hit_select_flag = true;
		}

		// �ǂ��ɖ߂邩��I���ł���悤�ɂ��Ă���
		// �Q�[���p�b�h�̐ڑ����Ȃ����
		if (GetJoypadNum() <= 0)
		{
			for (int i = 0; i < text_max; i++)
			{
				pos2 = { m_text_draw_pos[i].x + m_text.END_BACK_SIZE,  m_text_draw_pos[i].y + GetFontSize() };
				// �������Ă������̑I����Ԃɂ���
				if (CheckPointBoxHit(mouse_pos, m_text_draw_pos[i], pos2))
				{
					// �����蔻��̂������ق���I����Ԃ���
					m_select_num = i;
					// �������Ă���t���O�𗧂Ă�
					m_hit_select_flag = true;
					break;
				}
				else
				{
					m_hit_select_flag = false;
				}
			}
		}
		


		// �}�E�X���I���}�X�ɂ������Ă���Ƃ�
		if (m_hit_select_flag)
		{
			if (CheckMouseInput(MOUSE_INPUT_LEFT) || IsPadOn(PAD_ID::PAD_X))
			{
				// SE�̍Đ�
				SoundPlay(se_1);
				// �I�������t���O���グ��
				m_start_flag = true;
			}
		}
		// �I��������
		if (m_start_flag)
		{
			switch (m_select_num)
			{
			case text1: // �^�C�g���ɖ߂�
				m_change_scene = Title;
				m_turn = FadeOut;
				m_start_flag = false;
				break;
			case text2: // �N�G�X�g�I����ʂɂ��߂�
				m_change_scene = QuestArea;
				m_turn = FadeOut;
				m_start_flag = false;
				break;
			}
		}
		break;
	case FadeOut:

		// �t�F�[�h�A�E�g�̏���
		FadeOutSceneChange(m_change_scene);
		break;
	}

	// �t�B�[���h�ƃL�����N�^�[�̂���������
	HitField();
}

//------------------------------------------
// �`�揈��
//------------------------------------------
void EndScene::Draw()
{
	// �v���C���[�̃V���h�[�}�b�v�̃G���A�̃Z�b�g
	SetShadowMapArea(m_shadowMap_handle_1, m_player->m_transform.pos);

	//-------------------------------------------------------------
	// �V���h�E�}�b�v�̍쐬�i�����Ŋe�I�u�W�F�N�g�̃V���h�[�}�b�v�̐ݒ�j
	//-------------------------------------------------------------
	// �V���h�E�}�b�v�ւ̕`��̏���
	ShadowMap_DrawSetup(m_shadowMap_handle_1);
	{
		// �v���C���[�̕`�揈��
		m_player->Draw();

	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{

		// �V���h�E�}�b�v�փL�����N�^�[���f���̕`��
		m_field_2.Draw(camera.m_pos,m_player->m_transform.pos);
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

		// �v���C���[�̕`�揈��
		m_player->Draw();

	}
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// �v���C���[�̕`�揈��
		m_player->Draw();
		// �V���h�E�}�b�v�փL�����N�^�[���f���̕`��
		m_field_2.Draw(camera.m_pos, m_player->m_transform.pos);

	}
	UseShadowMapSet();


	// �t�H���g�T�C�Y�̐ݒ�
	SetFontSize(60);

	// ������̍����̎擾
	float h = GetFontSize();
	// �`����W
	Vector2 box_pos;
	// �ǂ����I�����Ă��邩�̊O�g
	box_pos.set(m_text_draw_pos[m_select_num].x - 30, m_text_draw_pos[m_select_num].y - 30);
	if (m_hit_select_flag)
	{
		DrawBox(box_pos.x, box_pos.y, box_pos.x + m_text.END_BACK_SIZE + 70, box_pos.y + h + 70, GetColor(255, 255, 0), TRUE);
	}

	// �e�L�X�g�̕`��
	for (int i = 0; i < text_max; i++)
	{
		m_text.TextDraw(i, m_text_draw_pos[i], m_text.END_BACK_SIZE);
	}

	// �t�F�[�h�̕`��
	FadeDraw();

}

//------------------------------------------
// �I������
//------------------------------------------
void EndScene::Exit()
{

	//�@�V���h�[�}�b�v�̍폜
	ExitShadowMap();

	m_player->Exit();
	// �v���C���[�N���X�̓|�C���^�Ȃ̂ōŌ�ɊJ������
	//delete m_player;

	// �}�E�X�̕\����Ԃ̐ݒ�
	SetMouseDispFlag(FALSE);
	// �}�E�X�̌Œ肷��
	ChangeMousePosMove(FALSE);
}

//------------------------------------------
// �t�B�[���h�ƃL�����N�^�[�̓����蔻��
//------------------------------------------
void EndScene::HitField()
{
	// �t�B�[���h�̒n�ʃ��f���ƃL�����N�^�[�̓����蔻��
	HitGroundCharacter(&m_player->m_transform.pos, &m_field_2.m_field_model);

	// �؂̃I�u�W�F�N�g�ƃv���C���[�̓����蔻��
	//for (int i = 0; i < m_field_2.TREE_MAX; i++)
	//{
	//	// �����X�^�[�ƃv���C���[�̈ړ��̓����蔻��
	//	if (CheckCapsuleHit(m_field_2.m_hit_tree[i], m_player->m_body))
	//	{
	//		m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &m_field_2.m_hit_tree[i]);
	//	}
	//}

	//// �t�F���X�ƃL�����N�^�[�̓����蔻��
	//for (int i = 0; i < m_field_2.FENCE_MAX; i++)
	//{
	//	if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
	//		m_field_2.m_hit_fence[i].m_box.hit_pos, m_field_2.m_hit_fence[i].m_box.half_size))
	//	{
	//		m_player->MoveHitUpdate(&m_field_2.m_hit_fence[i]);
	//	}
	//}


	//// �΂ƃL�����N�^�[�̓����蔻��
	//for (int i = 0; i < m_field_2.STONE_MAX; i++)
	//{
	//	if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
	//		m_field_2.m_hit_stone[i].m_box.hit_pos, m_field_2.m_hit_stone[i].m_box.half_size))
	//	{
	//		m_player->MoveHitUpdate(&m_field_2.m_hit_stone[i]);
	//	}
	//}
}

//------------------------------------------
// �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
//------------------------------------------
void EndScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// �J�����̊��x�ݒ�
	camera.SetCameraSensi(mouse);
	// �L�����N�^�[�̃T�E���h�̒���
	m_player->m_se.SetSoundVolume(se);
}

//----------------------------------------------
// �T�E���h�̍Đ�
//----------------------------------------------
void EndScene::SoundPlay(int se_num)
{
	// ���j���[��ʂł̓{�^���A�ł����邽��
	// �Đ����ɂ���Ȃ�Ȃ����߂ɍĐ����̂��̂������Ă���t���̂�炷
	// SE���Đ����łȂ����
	if (m_se.PlayingSound())
	{
		// SE�Đ��\�ɂ��Ă���
		m_se.m_playing_flag = true;
		// ���ݍĐ�����SE���~�߂�
		m_se.StopSound();
	}
	// SE�̍Đ�
	m_se.PlaySound_(se_num, DX_PLAYTYPE_BACK, true);
	m_se.m_playing_flag = false;
}

