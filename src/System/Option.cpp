#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"
#include "UIBar.h"
#include "src/Sound/Sound.h"
#include "Option.h"

// �I�v�V�������j���[�ł�邱��
// BGM,SE,�̃{�����[������
// �}�E�X�̊��x����
// �������
// 

// 
// �I�v�V�����w�i�͔�������
// ���ʂ͂P�O�����̃^�C�v�ɂ���
// �}�E�X���x�̂P�O����
// ��������͌��

Option::Option()
{
	m_menu_count = 0; // �O����X�^�[�g

	m_option_flag = false; // �ŏ��̓I�v�V�������j���[�͕��Ă���
	m_bgm_Volume = 5;
	m_se_Volume = 5;
	m_select = 0; // BGM����X�^�[�g
}

Option::~Option()
{
}

//----------------------------------------------
// ��������
//----------------------------------------------
void Option::Init()
{
	// �摜�ނ̓ǂݍ���
	m_image_box = LoadGraph("Data/Option/OptionMenu.png"); // �w�i
	m_operation_instructions_image = LoadGraph("Data/Option/Operationinstructions.jpg"); // ���������
	m_gamePad_operation_explanation_image = LoadGraph("Data/Option/GamePadOperationExplanation.jpg"); // ���������
	// �e�o�[�̐ݒ�
	SetOptionMenu();
	// �e�I�v�V�����̒l�̐ݒ�
	for (int i = 0; i < MENU_MAX; i++)
	{
		option_menu[i].m_value = option_menu[i].m_value / 2;
	}

	// ������������������ŏ���
	//m_op.Set({ BRA_X_STRAT ,m_option_box_pos.y + BRA_Y_DIVISION * 4 }, { 200,30 }, 0, true);
	// SE�̏�����
	m_se.NewArraySecureSound(se_max);
	m_se.LoadSound("Data/SE/menu_open.mp3", open_menu_se); // �I�v�V�������j���[���J������
	m_se.LoadSound("Data/SE/menu_close.mp3", close_menu_se); // �I�v�V�������j���[�����Ƃ�
	m_se.LoadSound("Data/SE/menu_selection.mp3", selection_menu_se); // �e���j���[��I�����Ă���Ƃ�
	m_se.LoadSound("Data/SE/menu_change_value.mp3", change_value_se); // �e���l��ύX���Ă���Ƃ�
}

//----------------------------------------------
// �I�v�V�����̍X�V����
//----------------------------------------------
void Option::Update()
{
	// �I�v�V�������j���[����������SE�̃{�����[���𒲐�����
	m_se.SetSoundVolume(option_menu[SE].m_value);

	// �I�v�V�������j���[���J���Ƃ�
	// �{�^���������ꂽ��
	if (m_option_flag == false)
	{
		// ���j���[���J���Ƃ��ɃQ�[���p�b�h�������Ă�����
		if (IsPadOn(PAD_ID::PAD_START))
		{
			m_operateType = GamePad;
		}
		// �L�[�{�[�h�̎�
		else if (PushHitKey(KEY_INPUT_TAB))
		{
			m_operateType = KeyboardMouse;
		}

		if (PushHitKey(KEY_INPUT_TAB) || IsPadOn(PAD_ID::PAD_START))
		{
			// �}�E�X�̕\����Ԃ̐ݒ�
			SetMouseDispFlag(TRUE);
			// �}�E�X�̌Œ����������
			ChangeMousePosMove(TRUE);
			// �I�v�V�������j���[���J��
			m_option_flag = true;
			// SE�Đ��\�ɂ��Ă���
			m_se.m_playing_flag = true;
			// ���ݍĐ�����SE���~�߂�
			m_se.StopSound();
		}
	}


	// �I�v�V�������j���[���J���Ă���Ƃ�
	if (m_option_flag)
	{
		// SE�̍Đ�
		if (m_se.m_playing_flag)
		{
			SoundPlay(open_menu_se);
		}
		// �I�v�V�����̍X�V����
		OpenMenuUpdate();
	}

	// �J�E���g�����ȏ�ɂȂ�����
	if (m_menu_count >= MENU_COUNT)
	{
		// �I�v�V�������j���[���J���Ă���
		// �Ȃ��{�^���������ꂽ��
		if (PushHitKey(KEY_INPUT_TAB) || IsPadOn(PAD_ID::PAD_START))
		{
			// �I�v�V�������j���[�����
			m_option_flag = false;
			// �J�E���g�����Z�b�g
			m_menu_count = 0;
			// SE�Đ��\�ɂ��Ă���
			m_se.m_playing_flag = true;
			// ���ݍĐ�����SE���~�߂�
			m_se.StopSound();
			// SE�̍Đ�
			SoundPlay(close_menu_se);

			// �}�E�X�̕\����Ԃ̐ݒ�
			SetMouseDispFlag(FALSE);
			// �}�E�X�̌Œ����������
			ChangeMousePosMove(FALSE);
		}
	}
	// �o�[�̍X�V����
	for (int i = 0; i < MENU_MAX; i++)
	{
		m_bra[i].Update(option_menu[i].m_value);
	}
}

//----------------------------------------------
// �I�v�V�����̐ݒ�
//----------------------------------------------
void Option::SetOptionMenu()
{
	// �e�o�[�̐ݒ�
	for (int i = 0; i < MENU_MAX; i++)
	{
		// �e�o�[�̍쐬
		m_bra[i].Set(option_menu[i].m_pos, m_bra_size, &option_menu[i].m_value, true);

		// ���O�̐ݒ�
		m_bra[i].SetName(option_menu[i].m_name);

		// �F�̐ݒ�
		// �o�[�{�̂̐F
		m_bra[i].SetColor(255, 255, 0, &m_bra[i].m_color);
		// �o�[�̃o�b�N�̐F
		m_bra[i].SetColor(128, 128, 0, &m_bra[i].m_back_color);
		// �O�g�̐F
		m_bra[i].SetColor(0, 0, 0, &m_bra[i].m_line_color);
		// �����̐F
		m_bra[i].SetColor(0, 0, 0, &m_bra[i].m_character_color);
	}
}

//----------------------------------------------
// �ǂ̃��j���[�𑀍삷�邩�̑I������
//----------------------------------------------
void Option::MenuSelect()
{
	// W�L�[�A�Q�[���p�b�h�̏�����{�^���������ꂽ��
	if (PushHitKey(KEY_INPUT_W) || IsPadOn(PAD_ID::PAD_D_UP))
	{
		// SE�̍Đ�
		SoundPlay(selection_menu_se);
		m_selection_menu--;
		// ���ȏ��ɍs���Ɖ��Ɉړ�����
		if (m_selection_menu < BGM)
		{
			m_selection_menu = MOUSE;
		}
	}
	// S�L�[�A�Q�[���p�b�h�̉������{�^���������ꂽ��
	if (PushHitKey(KEY_INPUT_S) || IsPadOn(PAD_ID::PAD_D_DOWN))
	{
		// SE�̍Đ�
		SoundPlay(selection_menu_se);
		m_selection_menu++;
		// ���ȏ㉺�ɍs���Ə�Ɉړ�����
		if (m_selection_menu > MOUSE)
		{
			m_selection_menu = BGM;
		}
	}
	// �I�����Ă��郁�j���[�����O�g�̐F��ς���
	for (int i = 0; i < MENU_MAX; i++)
	{
		if (i == m_selection_menu)
		{
			// �O�g�̐F
			m_bra[m_selection_menu].SetColor(255, 255, 255, &m_bra[m_selection_menu].m_line_color);
		}
		else
		{
			// �O�g�̐F
			m_bra[i].SetColor(0, 0, 0, &m_bra[i].m_line_color);
		}
	}

	// �}�E�X�ł����j���[��I���ł���悤�ɂ���
	if (CheckMouseInput(MOUSE_INPUT_LEFT))
	{
		// �}�E�X�̍��W���擾
		float mouse_pos_x = float(GetMouseX());
		float mouse_pos_y = float(GetMouseY());

		// �ǂ̃��j���[�Ɠ������Ă��邩�𒲂ׂ�
		for (int i = 0; i < MENU_MAX; i++)
		{
			// �������Ă������̑I����Ԃɂ���
			if (m_bra[i].HitPointBra(mouse_pos_x, mouse_pos_y))
			{
				m_selection_menu = i;
				// SE�̍Đ�
				SoundPlay(selection_menu_se);
			}
		}
	}


}

//----------------------------------------------
// �I�v�V�������J���Ă���Ƃ��̍X�V����
//----------------------------------------------
void Option::OpenMenuUpdate()
{
	// �ǂ̃��j���[�𑀍삷�邩�̑I��
	MenuSelect();

	m_menu_count++; // �J�E���g�𑝂₷
	// �I�����ꂽ���j���[�o�[�̒l�𑝂₷
	// D�L�[�A�Q�[���p�b�h�̉E�����{�^���������ꂽ�Ƃ�
	if (PushHitKey(KEY_INPUT_D) || IsPadOn(PAD_ID::PAD_D_RIGHT))
	{
		// SE���Đ����łȂ����
		if (m_se.PlayingSound())
		{
			// SE�Đ��\�ɂ��Ă���
			m_se.m_playing_flag = true;
			// ���ݍĐ�����SE���~�߂�
			m_se.StopSound();
		}

		// SE�̍Đ�
		SoundPlay(change_value_se);
		// �\�����ł���p�ɒl��ݒ肵�Ēl�ɑ���
		// �}�E�X�̕ύX�ł�������
		if (m_selection_menu == MOUSE)
		{
			option_menu[m_selection_menu].m_value += MOUSE_SENSI_VALUE;
			if (option_menu[m_selection_menu].m_value >= MOUSE_SENSI_MAX)
			{
				// �ő�̒l�𒴂��Ȃ��悤�ɂ���
				option_menu[m_selection_menu].m_value = MOUSE_SENSI_MAX;
			}
		}
		else
		{
			option_menu[m_selection_menu].m_value += VOLUME_CONSTANT_VALUE;
			if (option_menu[m_selection_menu].m_value >= VOLUME_MAX)
			{
				// �ő�̒l�𒴂��Ȃ��悤�ɂ���
				option_menu[m_selection_menu].m_value = VOLUME_MAX;
			}
		}
	}
	// �I�����ꂽ���j���[�o�[�̒l�����炷
	// A�L�[�A�Q�[���p�b�h�̍������{�^���������ꂽ�Ƃ�
	if (PushHitKey(KEY_INPUT_A) || IsPadOn(PAD_ID::PAD_D_LEFT))
	{
		// SE�̍Đ�
		SoundPlay(change_value_se);
		if (m_selection_menu == MOUSE)
		{
			option_menu[m_selection_menu].m_value -= MOUSE_SENSI_VALUE;
			if (option_menu[m_selection_menu].m_value <= MOUSE_SENSI_MIN)
			{
				// �ŏ��̒l�𒴂��Ȃ��悤�ɂ���
				option_menu[m_selection_menu].m_value = MOUSE_SENSI_MIN;
			}
		}
		else
		{
			// �\�����ł���p�ɒl��ݒ肵�Ēl�ɑ���
			option_menu[m_selection_menu].m_value -= VOLUME_CONSTANT_VALUE;
			if (option_menu[m_selection_menu].m_value <= VOLUME_MIN)
			{
				// �ŏ��̒l�𒴂��Ȃ��悤�ɂ���
				option_menu[m_selection_menu].m_value = VOLUME_MIN;
			}
		}

	}
}

//----------------------------------------------
// �T�E���h�̍Đ�
//----------------------------------------------
void Option::SoundPlay(int se_num)
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
	m_se.PlaySound_(se_num, m_se_info[se_num].play_type, m_se_info[se_num].loop);
	m_se.m_playing_flag = false;
}

//----------------------------------------------
// �I�v�V�����̕`�揈��
//----------------------------------------------
void Option::Draw()
{
	// ChangeFont("�l�r ����");
	// �I�v�V�������j���[���J���Ă��鎞����
	// �`�悷��
	if (m_option_flag)
	{
		// �w�i�摜
		DrawExtendGraphF(m_option_box_pos.x, m_option_box_pos.y, m_option_box_pos.x + BOX_SIZE_X, m_option_box_pos.y + BOX_SIZE_Y, m_image_box, TRUE);  // �I�v�V�������j���[�̔w�i

		// �f�t�H���g�̕����̑傫����ۑ����Ă���
		int original_font_size = GetFontSize();
		SetFontSize(28); // �t�H���g�T�C�Y�̕ύX
		// �e�o�[�̕`�揈��
		for (int i = 0; i < MENU_MAX; i++)
		{
			m_bra[i].Draw();
		}
		SetFontSize(original_font_size); // �t�H���g�T�C�Y��߂�

		// ���j���[��ʂ��Q�[���p�b�h�A�L�[�{�[�h�J�����ق��ɍ��킹�ĕ`�悷��
		if (m_operateType == GamePad)
		{
			
				// �������(���ŕ`��)
				DrawExtendGraphF(m_operation_instructions_pos.x, m_operation_instructions_pos.y,
					SCREEN_W - MARGIN, SCREEN_H - MARGIN, m_gamePad_operation_explanation_image, TRUE);
		}
		else
		{
			// �������(���ŕ`��)
			DrawExtendGraphF(m_operation_instructions_pos.x, m_operation_instructions_pos.y,
				SCREEN_W - MARGIN, SCREEN_H - MARGIN, m_operation_instructions_image, TRUE);
		}

	}

}
