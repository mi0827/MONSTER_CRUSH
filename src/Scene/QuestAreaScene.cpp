#include "src/WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"
#include "src/Hit/HitStop.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"
#include "src/Character/Receptionist.h"
#include "src/Character/Bot.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"
#include "src/Field/Field.h"

#include "src/System/DamageAction.h"
#include "src/Action/Attack.h"

#include "src/Camera.h"
#include "src/System/Text.h"
#include "Base/Scene_Base.h"
#include "QuestAreaScene.h"


//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
QuestAreaScene::QuestAreaScene()
{


}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
QuestAreaScene::~QuestAreaScene()
{

	delete[] m_landmark_text;
}

//------------------------------------------
// ��������
//------------------------------------------
void QuestAreaScene::Init()
{
	m_select_num = 0;

	// �x�[�X�N���X�ŏ��������Ă����������̂̏�����
	BaseInit();

	// �t�B�[���h�̏�����
	m_field_1.Init();

	// �J�����̏����ݒ�
	camera.PlayFieldInit();

	PlayerInit(hero);
	// �v���C���[�̏����ݒ� 
	m_player->Init();
	// �v���C���[�̍��W�̐ݒ�
	m_player->SetCharacterPos({ 0.0f, 0.0f,250.0f });
	// �v���C���[�̌����̐ݒ�
	m_player->SetCharacterRot({ 0.0f,-180.0f,0.0f });

	// �J�����̌����̐ݒ�
	camera.SetCameraRot(m_player->m_transform.rot);

	// ��t��̏����ݒ�
	receptionist.Init();
	// ��t��Ƙb����G���A�̐ݒ�
	InitArea();

	// �{�b�g�̏�����
	m_bot.Init();

	// �V���h�[�}�b�v�̐ݒ�
	ShadowMapInit();
	// ���݂̃V�[���̐ݒ�(�N�G�X�g�󒍃V�[��)
	m_now_scene = QuestArea;

	// �e�L�X�g�̓ǂݍ���
	m_quest_text.LoadText("Data/Text/Quest.txt", quest_max);
	m_quest_area_text.LoadText("Data/Text/QuestAreaStory.txt", palyer_story_max);
	m_reception_text.LoadText("Data/Text/QuestReception.txt", recept_teception_max);
}


//------------------------------------------
// �X�V����
//------------------------------------------
void QuestAreaScene::Update()
{
	switch (m_turn)
	{
	case Main:
		// �J�����̍X�V����
		camera.MouseCamera(&m_player->m_transform.pos);
		// ���C���ōs�����������̃V�[���̏�Ԃɍ��킹�ď����𕪂����s����֐�
		QuestAreaUpdate();


		break;
	case FadeOut:
		// �t�F�[�h�A�E�g�̏���
		FadeOutSceneChange(Battle);
		break;
	}
}

//------------------------------------------
// �`�揈��
//------------------------------------------
void QuestAreaScene::Draw()
{
	// �V���h�[�}�b�v�̃G���A�̃Z�b�g
	SetShadowMapArea(m_shadowMap_handle_1, m_player->m_transform.pos);
	SetShadowMapArea(m_shadowMap_handle_2, receptionist.m_transform.pos);
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
		receptionist.Draw();
	}
	ShadowMap_DrawSetup(m_shadowMap_handle);
	{
		// �t�B�[���h�̕`��
		m_field_1.Draw(camera.m_pos, m_player->m_transform.pos);
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
		// ��t��̕`��
		receptionist.Draw();
		// �{�b�g
		m_bot.Draw();
	}
	SetUseShadowMap(2, m_shadowMap_handle_2);
	{
		receptionist.Draw();
	}
	SetUseShadowMap(0, m_shadowMap_handle);
	{
		// �v���C���[�̕`�揈��
		m_player->Draw();

		// ��t��̕`��
		receptionist.Draw();
		// �V���h�E�}�b�v�փL�����N�^�[���f���̕`��
		m_field_1.Draw(camera.m_pos, m_player->m_transform.pos);
	}
	UseShadowMapSet();


	// ��b���e�ɉ����ď�����ύX
	switch (scene_mode_num)
	{
	case normal:// ��b���Ă��Ȃ����
		ModeNormalDraw();
		break;

	case talk_start:
		break;
	case receptionist_talk: // ��b��
		TalkDraw();
		break;
	case accepting_quest: // �N�G�X�g���󂯂Ă�����
		AcceptingQuestDraw();
		break;
	}

	// �t�F�[�h�̕`�揈��
	FadeDraw();
}

//------------------------------------------
// �I������
//------------------------------------------
void QuestAreaScene::Exit()
{


	//�@�V���h�[�}�b�v�̍폜
	ExitShadowMap();

	// �v���C���[�̏I������
	m_player->Exit();
}

//------------------------------------------
// �t�B�[���h�ƃL�����N�^�[�̓����蔻��
//------------------------------------------
void QuestAreaScene::HitField()
{

	// �t�B�[���h�̒n�ʃ��f���ƃv���C���[�̓����蔻��
	HitGroundCharacter(&m_player->m_transform.pos, &m_field_1.m_field_model);
	// ��t��
	HitGroundCharacter(&receptionist.m_transform.pos, &m_field_1.m_field_model);


	// �؂̃I�u�W�F�N�g�ƃv���C���[�̓����蔻��
	// �Ȃ����Q�{���������蔻�肪�ǂ����Ȃ�
	for (int i = 0; i < m_field_1.TREE_MAX; i++)
	{
		// �����X�^�[�ƃv���C���[�̈ړ��̓����蔻��
		if (CheckCapsuleHit(m_field_1.m_hit_tree[i], m_player->m_body))
		{
			m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &m_field_1.m_hit_tree[i]);
		}
	}

	// �v���C���[�ƃt�B�[���h���͂��Ă�l�p�Ƃ̓����蔻��
	for (int i = 0; i < m_field_1.AROUND_MAX; i++)
	{
		if (CheckBoxHit3D(m_player->m_transform.pos, m_player->m_move_hit_size,
			m_field_1.m_hit_around[i].m_box.hit_pos, m_field_1.m_hit_around[i].m_box.half_size))
		{
			m_player->MoveHitUpdate(&m_field_1.m_hit_around[i]);
		}
	}

}

//------------------------------------------
// �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
//------------------------------------------
void QuestAreaScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// �J�����̊��x�ݒ�
	camera.SetCameraSensi(mouse);
	// �L�����N�^�[�̃T�E���h�̒���
	m_player->m_se.SetSoundVolume(se);

}

//------------------------------------------
// �b����悤�ɂȂ�G���A�̐ݒ�
//------------------------------------------
void QuestAreaScene::InitArea()
{
	// �J�v�Z�����W�͎�t��Ɠ������W�ɂ���
	Vector3 pos = receptionist.m_transform.pos;
	Vector3 pos2 = { 0.0f, 60.0f, 0.0f };
	// ���a�̐ݒ�
	float r = AREA_FIELD_R;
	// �J�v�Z���̍쐬
	m_area.CreateCapsule(pos, pos2, r);
}

//------------------------------------------
// �b����悤�ɂȂ�G���A�̐ݒ�
//------------------------------------------
void QuestAreaScene::LandMarkUpdate()
{
	// �J�����̌������擾����
	m_camera_rot = camera.GetCameraRot();
	

	// ������̕`��̂��߂̐ݒ�
	for (int i = 0; i < landmark_text_max; i++)
	{
		// ���W�ϊ�
		VECTOR pos = DrawStringWorld(receptionist.m_transform.pos, m_landmark_text[i].shift_pos);
		// �`����p�ɕϊ�
		m_landmark_text[i].draw_pos.VSet(pos);

		// �ϊ������X�N���[�����W��Z�̒l��0.0 ~ 1.0 �Ȃ�`�悵�Ă���
		if (i == f_text || i == x_text)
		{
			// �`�悵����������F : �b���̏ꍇ
			// �b���Ă����G���A�ɓ����Ă��Ȃ��ƕ`��ł��Ȃ��悤�ɂ���
			if (m_area_hit)
			{
				// �J�����̉�p���ғ����ɂ���ĕ`�悵�Ȃ�
				// ��p��
				if (pos.z > 0.0f && pos.z < 1.0f)
				{
					// �`�悷��
					m_landmark_text[i].draw_flag = true;
				}
				else // ��ʂ���
				{
					// �`�悵�Ȃ�
					m_landmark_text[i].draw_flag = false;
				}
			}
			else // �͈͂����ł��`��ł��Ȃ�
			{
				m_landmark_text[i].draw_flag = false;
			}
		}
		else // �w��̃e�L�X�g�ȊO��
		{
			// �J�����̉�p���ғ����ɂ���ĕ`�悵�Ȃ�
			if (pos.z > 0.0f && pos.z < 1.0f)
			{
				// �`�悷��
				m_landmark_text[i].draw_flag = true;
			}
			else
			{
				// �`�悵�Ȃ�
				m_landmark_text[i].draw_flag = false;
			}
		}
	}
}

//------------------------------------------
// ���̃V�[���̏�Ԃɍ��킹�ēK�؂ȏ�����������֐�
//------------------------------------------
void QuestAreaScene::QuestAreaUpdate()
{
	// �Q�[�����i��ł���ԃt���[�����J�E���g��������
	// �X�g�[���[��i�߂�ۂ̃X�y�[�X�L�[�̕������������������邽�߂̂���
	if (m_blend_flag)
	{
		m_frame_count++;
		if (m_frame_count >= 10)
		{
			m_frame_count = 0;
			m_blend_flag = false;
		}
	}
	else
	{
		if (m_blend_change_flag)
		{
			m_text_blend_value += 10;
			if (m_text_blend_value >= TEXT_BLEND_MAX)
			{
				m_blend_flag = true;
				m_blend_change_flag = false;
			}
		}
		else
		{
			m_text_blend_value -= 10;
			if (m_text_blend_value <= TEXT_BLEND_MIN)
			{
				m_blend_flag = true;
				m_blend_change_flag = true;
			}
		}
	}




	switch (scene_mode_num)
	{
	case normal:// ��b���Ă��Ȃ����
		ModeNormalUpdate();
		break;
	case talk_start:
		TalkStart();
		break;

	case receptionist_talk: // ��b��
		TalkUpdate();

		break;
	case accepting_quest: // �N�G�X�g���󂯂Ă�����
		AcceptingQuestUpdate();
		break;
	}
}

//------------------------------------------
// ��b���Ă��Ȃ���Ԃ̍X�V����
//------------------------------------------
void QuestAreaScene::ModeNormalUpdate()
{

	// �J�����̌������擾����
	m_camera_rot = camera.GetCameraRot();


	// �q�b�g�X�g�b�v�̃^�C�~���O�ȊO
	if (m_hit_stop.CheckHitStop() == false)
	{
		// �v���C���[�̍X�V����
		m_player->Update(&m_camera_rot);
	}
	// ��t��̂̍X�V����
	receptionist.Update();

	// ��t��ƃv���C���[�̈ړ��̓����蔻��
	if (CheckCapsuleHit(receptionist.m_hit_body, m_player->m_body))
	{
		m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &receptionist.m_hit_body);
	}

	// �{�b�g�̍X�V����
	BotUpdate();

	// �t�B�[���h�ƃL�����N�^�[�̓����蔻��
	HitField();
	// �ڈ���ʂ����̏���
	LandMarkUpdate();

	// �v���C���[����t��Ƙb����͈͂ɓ��������̊m�F
	if (CheckCapsuleHit(m_area, m_player->m_body))
	{
		// X�L�[�������ꂽ���ɃV�[���̕ύX������i�������̉��j
		if (PushHitKey(KEY_INPUT_F) || IsPadOn(PAD_ID::PAD_X))
		{
			// ���̃V�[���̏�Ԃ���b�p�[�g�Ɉړ�����
			scene_mode_num = talk_start;
		}
		// �b����G���A�ɓ���������t���O���グ��
		m_area_hit = true;
	}
	else
	{
		// �͈͊O�ɂ���Ƃ��̓t���O��������
		m_area_hit = false;
	}
}

//------------------------------------------
// ��b�͂��߂̏���
//------------------------------------------
void QuestAreaScene::TalkStart()
{
	// �N�G�X�g�o���m�F�ł�������I�����ēx�b���������Ă���ꍇ	
	if (m_quest_acception_num == quest_confirmation)
	{
		// ��قǂ̉�b�̑�������ɂȂ�悤�Ɏ�t��̃e�L�X�g�̍s��ݒ肷��
		m_reception_text_line = 3;
	}

	// �V�[���̏�Ԃ���b���Ɉړ�
	scene_mode_num = receptionist_talk;

}

//------------------------------------------
// ��b�p�[�g�̍X�V����
//------------------------------------------
void QuestAreaScene::TalkUpdate()
{
	// �}�E�X�̉E�N���b�N���X�y�[�X�L�[�ŉ�b��i�߂�
	if (PushMouseInput(MOUSE_INPUT_LEFT) || PushHitKey(KEY_INPUT_SPACE) || IsPadOn(PAD_ID::PAD_X))
	{
		// �`�悷��e�L�X�g����i�߂�
		m_reception_text_line++;
	}

	// ��t��̉�b���ǂ��܂Ői��ł���̂��œ��e��ύX���邽�߂̂���
	switch (m_quest_acception_num)
	{
	case quest_before_accepting:

		// �e�L�X�g����s�i�񂾂�
		if (m_reception_text_line >= 1)
		{
			// ���̉�b���[�h�Ɉړ������Ă���
			m_quest_acception_num = quest_confirmation;
			// �V�[���̏�Ԃ��N�G�X�g�I����ԂɈڍs����
			scene_mode_num = accepting_quest;
			// �^�C�~���O��
			m_reception_text_line = 3;
		}
		break;
	case quest_confirmation:
		// �I�����̕ύX
		if (PushHitKey(KEY_INPUT_W) || IsPadOn(PAD_ID::PAD_D_UP))
		{
			m_select_num--;
			if (m_select_num < 0)
			{
				m_select_num = 0;
			}

		}
		if (PushHitKey(KEY_INPUT_S) || IsPadOn(PAD_ID::PAD_D_DOWN))
		{
			m_select_num++;
			if (m_select_num > 1)
			{
				m_select_num = 1;
			}
		}

		if (m_reception_text_line >= 4)
		{
			// ���̉�b���[�h�Ɉړ������Ă���
			m_quest_acception_num = quest_after_accepting;
			//m_reception_text_line = 3;
		}
		break;
	case quest_after_accepting:
		// �͂����I�΂ꂽ���̏���
		if (m_select_num == 0)
		{
			// �t�F�[�h�A�E�g��Ԃɂ���
			m_turn = FadeOut;
			// �e�L�X�g�ԍ������������Ă���
			m_reception_text_line = 0;
			break;
		}
		else // ���������I�΂ꂽ���̏����𕪂���
		{
			if (m_reception_text_line >= 6)
			{
				// �}�E�X�̉E�N���b�N���X�y�[�X�L�[�ŉ�b��i�߂�
				if (PushMouseInput(MOUSE_INPUT_LEFT) || PushHitKey(KEY_INPUT_SPACE) || IsPadOn(PAD_ID::PAD_X))
				{
					// ��b�V�[���ɖ߂�
					m_quest_acception_num = quest_confirmation;
					// ��b��������߂��Ă���
					// �v���C���[�Ƃ̉�b����������I���
					scene_mode_num = normal;
				}
			}
		}
		break;
	}
}

//------------------------------------------
// �N�G�X�g���󂯂Ă���Ƃ��̍X�V����
//------------------------------------------
void QuestAreaScene::AcceptingQuestUpdate()
{
	// �I�����̕ύX
	if (PushHitKey(KEY_INPUT_W) || IsPadOn(PAD_ID::PAD_D_UP))
	{
		m_select_num--;
		if (m_select_num < 0)
		{
			m_select_num = 0;
		}
	}
	if (PushHitKey(KEY_INPUT_S) || IsPadOn(PAD_ID::PAD_D_DOWN))
	{
		m_select_num++;
		if (m_select_num > 1)
		{
			m_select_num = 1;
		}
	}

	// �N�G�X�g�I����ʂ̏�ԕʂ̏���
	switch (m_quest_selection_num)
	{
	case quest_selection:
		m_quest_num = m_select_num;
		break;
	case reply_selection:
		m_reply_num = m_select_num;
		break;
	}

	// ���ɐi��
	if (PushMouseInput(MOUSE_INPUT_LEFT) || PushHitKey(KEY_INPUT_SPACE) || IsPadOn(PAD_ID::PAD_X))
	{
		// ���������s���񂸂i�߂�
		m_quest_text_line += 3;

		// �ԓ����
		if (m_quest_selection_num == reply_selection)
		{
			// �ԓ�����������������
			if (m_reply_num == 1)
			{
				// �ԓ����������Ȃ�N�G�X�g�I���ɖ߂�
				m_quest_text_line = 0;
			}
			else
			{
				// �͂���������
				// ���̃V�[���̏�Ԃ���b�p�[�g�̏�Ԃ��Ă���
				scene_mode_num = receptionist_talk;
				// ��t��̉�b�̓r�������蒼��
				m_quest_text_line = 2;

			}
		}
		if (scene_mode_num == accepting_quest)
		{
			if (m_quest_text_line <= 3)
			{
				// �I������ʂ����ɐi�߂�
				m_quest_selection_num = quest_selection;
			}
			if (m_quest_text_line >= 3 && m_quest_text_line < 5)
			{
				// �I������ʂ�ԓ���ʐi�߂�
				m_quest_selection_num = reply_selection;
			}
		}
	}
}

//------------------------------------------
// ��b���Ă��Ȃ���Ԃ̕`�揈��
//------------------------------------------
void QuestAreaScene::ModeNormalDraw()
{
	// �t�H���g�T�C�Y�̐ݒ�
	SetFontSize(FONT_SIZE);

	// ������̍����̎擾
	float h = GetFontSize();
	// �ڈ�̕`��
	for (int i = 0; i < landmark_text_max; i++)
	{
		if (m_landmark_text[i].draw_flag)
		{
			// �`�敝�̎擾
			float w = GetDrawStringWidth(m_landmark_text[i].text, -1);
			// �`����W
			m_landmark_draw_pos = { m_landmark_text[i].draw_pos.x - w / 2,  m_landmark_text[i].draw_pos.y - h };
			// ������̕`��
			if (i != excamation_pos)
			{
				if (GetJoypadNum() >= 1)
				{
					DrawString(m_landmark_draw_pos.x, m_landmark_draw_pos.y, m_landmark_text[1].text, GetColor(255, 128, 50));
				}
				else
				{
					DrawString(m_landmark_draw_pos.x, m_landmark_draw_pos.y, m_landmark_text[0].text, GetColor(255, 128, 50));
				}
			}
			else
			{
				DrawString(m_landmark_draw_pos.x, m_landmark_draw_pos.y, m_landmark_text[i].text, GetColor(255, 128, 50));
			}

		}
	}
	// �v���C���[�̃e�L�X�g�̕`��
	SetFontSize(TEXT_FONT_SIZE);
	h = GetFontSize();
	m_text_draw_pos.set((SCREEN_W / 2 - m_quest_area_text.TITLE_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_quest_area_text.CREVICE_SIZE)));
	m_quest_area_text.TextDraw(m_quest_area_text_line, { m_text_draw_pos.x, (m_text_draw_pos.y + h) }, m_quest_area_text.TITLE_BACK_SIZE);

	// �N���b���Ă��邩�̕`��
	DrawString(m_text_draw_pos.x, m_text_draw_pos.y, "Player", GetColor(255, 128, 50));
}

//------------------------------------------
// ��b�X�^�[�g���̕`�揈��
//------------------------------------------
void QuestAreaScene::TalkStartDraw()
{

}


//------------------------------------------
// ��b�p�[�g�̕`�揈��
//------------------------------------------
void QuestAreaScene::TalkDraw()
{
	// ������̍����̎擾
	float h = GetFontSize();

	h = GetFontSize();
	// �O�g�̕`����W
	Vector2 box_pos;
	// �N�G�X�g�o���m�F�������`�悷��
	if (m_quest_acception_num == quest_confirmation)
	{
		// �ǂ����I�����Ă��邩�̊O�g
		if (m_select_num == 0)
		{
			box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30);
		}
		else
		{
			box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30 + (h * 2 + m_quest_text.CREVICE_SIZE));
		}

		DrawBox(box_pos.x, box_pos.y, box_pos.x + m_quest_text.QUEST_BACK_SIZE + 70, box_pos.y + h * 2 + 40, GetColor(255, 255, 0), TRUE);
		// YES
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2));
		m_quest_text.TextDraw(3, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// NO
		// Y���W�͂���Ăق������𑫂�
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2 + (h * 2 + m_quest_text.CREVICE_SIZE)));
		m_quest_text.TextDraw(4, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);
	}

	// ��b���e�̕`��
	m_text_draw_pos.set((SCREEN_W / 2 - m_reception_text.QUEST_STORY_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_reception_text.CREVICE_SIZE)));
	m_reception_text.TextDraw(m_reception_text_line, { m_text_draw_pos.x, (m_text_draw_pos.y + h) }, m_reception_text.QUEST_STORY_BACK_SIZE);

	// �X�g�[���[��i�s�L�[�̕`��
	const char* text = "SPACE";
	// �Q�[���p�b�h�̐ڑ����Ă��邩�ɂ���ĕ`�悷����̂�ύX����
	if (GetJoypadNum() <= 0)
	{
		text = "SPACE";
	}
	else
	{
		text = " X ";
	}

	float x = GetDrawStringWidth(text, -1);
	// �e�L�X�g�o�[�̒[��������e�L�X�g���������ʒu�ɐݒ�
	int draw_pos_x = m_text_draw_pos.x + m_reception_text.QUEST_STORY_BACK_SIZE - x;
	// ���̃L�[��`�悷��̂��������x��ύX��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_text_blend_value);

	DrawString(draw_pos_x, m_text_draw_pos.y + h, text, GetColor(255, 128, 50));
	// �Â��̕ύX
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TEXT_BLEND_MAX);

	// �N���b���Ă��邩�̕`��
	DrawString(m_text_draw_pos.x, m_text_draw_pos.y, "��̏�", GetColor(255, 128, 50));
}

//------------------------------------------
// �N�G�X�g���󂯂Ă���Ƃ��̕`�揈��
//------------------------------------------
void QuestAreaScene::AcceptingQuestDraw()
{
	// ������̍����̎擾
	float h = GetFontSize();

	h = GetFontSize();

	Vector2 box_pos;
	if (m_select_num == 0)
	{
		box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30);
	}
	else
	{
		box_pos.set(SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE - 30, SCREEN_H / 2 - 30 + (h * 2 + m_quest_text.CREVICE_SIZE));
	}
	// �ǂ����I��ł��邩���킩��p�̃{�b�N�X
	DrawBox(box_pos.x, box_pos.y, box_pos.x + m_quest_text.QUEST_BACK_SIZE + 70, box_pos.y + h * 2 + 40, GetColor(255, 255, 0), TRUE);

	if (m_quest_selection_num == quest_selection)
	{
		m_text_draw_pos.set(m_quest_draw_pos[0].x, m_quest_draw_pos[0].y);
		// �N�G�X�g�P
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2));
		m_quest_text.TextDraw(0, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// �N�G�X�g�Q 
		// Y���W�͂���Ăق������𑫂�
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2 + (h * 2 + m_quest_text.CREVICE_SIZE)));
		m_quest_text.TextDraw(1, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// �m�F����1
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_STORY_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_quest_text.CREVICE_SIZE) - 100));
		m_quest_text.TextDraw(2, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_STORY_BACK_SIZE);
	}


	if (m_quest_selection_num == reply_selection)
	{

		// YES
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2));
		m_quest_text.TextDraw(3, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// NO
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_BACK_SIZE_HALF_SIZE), (SCREEN_H / 2 + (h * 2 + m_quest_text.CREVICE_SIZE)));
		m_quest_text.TextDraw(4, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_BACK_SIZE);

		// �m�F����2
		m_text_draw_pos.set((SCREEN_W / 2 - m_quest_text.QUEST_STORY_BACK_HALF_SIZE), (SCREEN_H - (h * 2 + m_quest_text.CREVICE_SIZE) - 100));
		m_quest_text.TextDraw(5, { m_text_draw_pos.x, m_text_draw_pos.y }, m_quest_text.QUEST_STORY_BACK_SIZE);
	}

	// �X�g�[���[��i�s�L�[�̕`��
	const char* text = "SPACE";
	// �Q�[���p�b�h�̐ڑ����Ă��邩�ɂ���ĕ`�悷����̂�ύX����
	if (GetJoypadNum() <= 0)
	{
		text = "SPACE";
	}
	else
	{
		text = " X ";
	}
	float x = GetDrawStringWidth(text, -1);
	// �e�L�X�g�o�[�̒[��������e�L�X�g���������ʒu�ɐݒ�
	int draw_pos_x = m_text_draw_pos.x + m_reception_text.QUEST_STORY_BACK_SIZE - x;
	// ���̃L�[��`�悷��̂��������x��ύX��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_text_blend_value);
	DrawString(draw_pos_x, m_text_draw_pos.y, text, GetColor(255, 128, 50));
	// �Â��̕ύX
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TEXT_BLEND_MAX);

}

//------------------------------------------
// �{�b�g�֘A�̍X�V����
//------------------------------------------
void QuestAreaScene::BotUpdate()
{
	// �{�b�g�ƃv���C���[�̈ړ��̓����蔻��
	if (CheckCapsuleHit(m_bot.m_body, m_player->m_body))
	{
		m_player->m_move.MoveHitCapsule(&m_player->m_transform.pos, m_player->m_body.m_capsule.radius, &m_bot.m_body);
	}

	// �v���C���[���U�����Ă�����
	if (m_player->m_attack_flag)
	{
		// player�̍U���̎��Ɏ�肽�������蔻��ƃ����X�^�[�̑̂Ƃ̓����蔻����Ƃ�
		int num = m_player->m_now_attack;
		// �U���̓����蔻��s���Ă����Ƃ�����(�U���A�j���[�V�����̎w��̃t���[���Ԃ���)
		if (m_player->AttackHitGoodTiming(num))
		{
			// �����X�^�[�̃{�f�B�[�Ƃ̓����蔻����Ƃ�
			if (HitAttack(m_bot.m_body, m_player->m_attack_hit_damage[num]->attack_hit) == true)
			{
				// �����蔻�肪���������񂾂����̏�����ʂ�悤�ɂ���
				// �_���[�W�������s���Ă����t���O���オ���Ă�����
				if (m_player->m_can_hit_damage_flag)
				{
					// ��񂾂��ʂ��Ăق�������t���O��������
					m_player->m_can_hit_damage_flag = false;

					// �{�b�g���U���󂯂����̏���
					m_bot.Update();

					// �q�b�g�X�g�b�v���s���Ă������^�C�~���O�����s��
					if (m_player->m_attack_hit_damage[num]->can_hit_stop)
					{
						// �_���[�W���������^�C�~���O�Ńq�b�g�X�g�b�v�̃J�E���g�����Z�b�g����
						m_hit_stop.StopCountReset();
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

}





