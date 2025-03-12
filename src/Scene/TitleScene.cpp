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

#include "src/Camera.h"
#include "src/System/Text.h"
#include "Base/Scene_Base.h"
#include "TitleScene.h"



//------------------------------------------
// ��������
//------------------------------------------
void TitleScene::Init()
{
	// �x�[�X�N���X�ŏ��������Ă����������̂̏�����
	BaseInit();

	// �t�B�[���h�̏�����
	// m_field_1.Init();

	// �J�����̏����ݒ�
	camera.PlayFieldInit();

	m_background_image = LoadGraph("Data/Title/Title.png");
	// ���݂̃V�[���̐ݒ�(�^�C�g���V�[��)
	m_now_scene = Title;

	// SE�̏�����
	m_sound.NewArraySecureSound(SE_MAX);
	m_sound.LoadSound("Data/SE/Title/Title_start.mp3", decision);
}

//------------------------------------------
// �X�V����
//------------------------------------------
void TitleScene::Update()
{

	switch (m_turn)
	{
	case Main:

		// �^�C�g���V�[���̍X�V����
		TitleUpdate();

		break;
	case FadeOut:
		// �t�F�[�h�A�E�g�̏���
		FadeOutSceneChange(Story);
		break;
	}

}

//------------------------------------------
// �`�揈��
//------------------------------------------
void TitleScene::Draw()
{


	//=============================================
	// ���Ń^�C�g����`��
	//=============================================
	// �w�i�̕`��
	DrawExtendGraphF(0, 0, SCREEN_W, SCREEN_H, m_background_image, TRUE);

	// �t�H���g�T�C�Y�̐ݒ�
	SetFontSize(150);
	// �^�C�g���̕`��
	const char* name = "�����X�^�[�N���b�V��";
	// �`�敝�̎擾
	float w = GetDrawStringWidth(name, -1);
	// ������̍����̎擾
	float h = GetFontSize();
	// �`����W
	Vector2 draw_pos = { SCREEN_W / 2 - w / 2, SCREEN_H / 2 - h };
	DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));

	// �ǂ̃{�^���������Ύn�܂邩��`��
	SetFontSize(100);
	if (GetJoypadNum() <= 0)
	{
		name = "SPACE";
	}
	else
	{
		name = " A ";
	}

	w = GetDrawStringWidth(name, -1);
	h = GetFontSize();
	draw_pos = { SCREEN_W / 2 - w / 2, SCREEN_H / 2 + h * 2 };
	// ���̃L�[��`�悷��̂��������x��ύX��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_text_blend_value);
	DrawString(draw_pos.x, draw_pos.y, name, GetColor(255, 128, 50));
	// �Â��̕ύX
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, TEXT_BLEND_MAX);

	// �t�F�[�h�̕`�揈��
	FadeDraw();

}

//------------------------------------------
// �I������
//------------------------------------------
void TitleScene::Exit()
{
	// �Đ�����SE���~�߂�
	m_sound.StopSound();

	//�@�V���h�[�}�b�v�̍폜
	ExitShadowMap();

}

//------------------------------------------
// �t�B�[���h�Ƃ̓����蔻��
//------------------------------------------
void TitleScene::HitField()
{

}

//------------------------------------------
// �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
//------------------------------------------
void TitleScene::OptionValuesReflect(int bgm, int se, int mouse)
{
	// �J�����̊��x�ݒ�
	camera.SetCameraSensi(mouse);

	// �L�����N�^�[�̃T�E���h�̒���
	//m_player->m_se.SetSoundVolume(se);


}

//------------------------------------------
// �^�C�g���̍X�V����
//------------------------------------------
void TitleScene::TitleUpdate()
{
	switch (m_title_mode_num)
	{
	case TITLE:

		// �^�C�g����ʂł̃J�E���g�����炷
		m_count_flame++;
		// �t���[�������ȉ��ɂȂ�����
		if ((m_count_flame % FLAME_MAX == 0) && m_count_flame != 0)
		{
			// ���Ԃ𑝂₷
			m_count_time++;
		}
		// ���Ԃ����ȏ�o�߂�����
		if (m_count_time >= TITLE_TIME_MAX)
		{
			// �J�E���g�����Z�b�g
			m_count_time = 0;
			m_count_flame = 0;
			// ���惂�[�h�Ɉڍs����
			m_title_mode_num = MOVIE;
		}

		// X�L�[�������ꂽ���ɃV�[���̕ύX������i�������̉��j
		if (PushHitKey(KEY_INPUT_SPACE)||IsPadOn(PAD_ID::PAD_A))
		{
			// �t�F�[�h�A�E�g��Ԃ���
			m_turn = FadeOut;
			// SE�̍Đ�
			m_sound.PlaySound_(decision, DX_PLAYTYPE_BACK, true);
		}


		// �Q�[�����i��ł���ԃt���[�����J�E���g��������
		// �X�g�[���[��i�߂�ۂ̃X�y�[�X�L�[�̕������������������邽�߂̂���
		if (m_count_time % 1 == 0)
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
		break;

	case MOVIE:

		// �v���C����̍Đ�
		m_movie = PlayMovie("Data/Movie/03.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);
		//GetMovieStateToGraph(m_movie);
		// �w��̓���̍Đ���Ԃ��擾
		if (GetMovieStateToGraph(m_movie))
		{
			// ��ʂ̏�Ԃ��^�C�g����Ԃɂ���
			m_title_mode_num = TITLE;

		}
		break;
	}
}


