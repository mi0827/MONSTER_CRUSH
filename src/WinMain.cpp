#include "WinMain.h"
#include "GameMain.h"

// EffekseerForDXLib.h���C���N���[�h���܂��B
#include "EffekseerForDXLib.h"


// �L�[���͂Ɋւ���ϐ�
char KeyBuffer[256];
int KeyFrame[256];
// �}�E�X���͂Ɋւ���ϐ�
int MouseLeftFrame;
int MouseRightFrame;
// ���4�̕ϐ��̓L�[�{�[�h�ƃ}�E�X�̉����ꂽ��
// ���擾���邽�߂ɕK�v�Ȋ֕ϐ�

// �}�E�X�̍��E�ړ��Ɋւ���ϐ�
int NowMouseX;
int NowMouseY;
int BeforeMouseX;
int BeforeMouseY;

//! @brief  �}�E�X����ʂ̐^�񒆂ɌŒ肷�邩�ǂ����̃t���O
//! @brief true : �ϓ��\�A false : �Œ肷��
bool mouse_pos_middle_flag = false;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Time; // ���Ԃ��Ƃ邽�߂̕ϐ�

	// �U�FLog.txt���o�͂��Ȃ��悤�ɂ��܂�
	SetOutApplicationLogValidFlag(FALSE);
	// �R�F�E�B���h�E���[�h�ł������܂�
	ChangeWindowMode(TRUE);
	// �V�F�E�B���h�E�^�C�g���̕�����̐ݒ�
	SetMainWindowText("Monsuta");
	// �W�F�w�i�̓h��Ԃ��̐F��ݒ�
	SetBackgroundColor(100, 100, 100);


	// �X�F�E�B���h�E�T�C�Y�̐ݒ�
	// 800 �~ 450 : �E�B���h�E�̑傫��
	// 32 : �F���i32�r�b�g�J���[�j*���ڂĂȐF�̐�����Ȃ�
	SetGraphMode(SCREEN_W, SCREEN_H, 32);

	// ��ʗp�̂y�o�b�t�@�̃r�b�g�[�x��ݒ肷��( 16 or 24 or 32 )
	SetCreateDrawValidGraphZBufferBitDepth(32);
	SetZBufferBitDepth(32);

	// �����F�̐ݒ�(�摜�̒��́@R:255, G:0, B:255)
	SetTransColor(255, 0, 255);

	// �N����ʂ𕡐������グ�邱�Ƃ��ł���悤��
	SetDoubleStartValidFlag(TRUE);
	// �E�B���h�E���O�ʂɂ����Ă����ɂ����Ă�������Ԃɂ��܂�
	SetAlwaysRunFlag(TRUE);

	// 10 :��ʕ`��ݒ�(�`��̈���ɂ��Č��ɏ�������Ŋ����������̂�O�ɂ��ĉ�ʂɏo���j
	SetDrawScreen(DX_SCREEN_BACK);


	//-----------------------------------------------------------
	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	//-----------------------------------------------------------
	// �V�F�[�_�[�t�@�C���͂���������Ȃ��Ɠǂݍ��߂Ȃ�
	//SetUseDirect3DVersion(DX_DIRECT3D_9EX);

   // �c�w���C�u��������������
	if (DxLib_Init() == -1)	return -1;			// �G���[���N�����璼���ɏI��


	// Effekseer������������B
	// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}
	// Effekseer�Ή�
	//-----------------------------------------------------------
	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	//-----------------------------------------------------------


	//srand(GetNowCount() % RAND_MAX);

	for (int i = 0; i < 256; i++)
	{
		KeyFrame[i] = 0;
	}
	MouseLeftFrame = 0;
	MouseRightFrame = 0;

	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));

	// ���̓�̏������Ȃ��ƂRD��Ԃ����Ȃ�
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	ChangeLightTypeDir(VGet(0.8f, -1.2f, 1.0f));

	// �Q�[���p�b�h���g�p����ۂɕK�v�ɂȂ�ݒ�
	InputPadInit();
	// �Q�[�����C���̏�������
	GameInit();

	// �}�E�X�̕\����Ԃ̐ݒ�
	SetMouseDispFlag(FALSE);
	
	// �t�H���g�̕ύX
	ChangeFont("�l�r ����");
	// �t�H���g�����₷�����Ă���
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_4X4);

	// �S�F���C�����[�v
	while (TRUE) {
		// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
		//Effekseer_Sync3DSetting();

		Time = GetNowCount();

		// �P�P�F�`��̈���N���A
		ClearDrawScreen();
		InputPadUpdate();

		GetHitKeyStateAll(KeyBuffer); // �L�[�{�[�h�̂��ׂẴL�[�̉�����Ԃ��擾����

		// �L�[�������ꂽ�����Ƃ鏈��
		for (int i = 0; i < 256; i++)
		{
			if (KeyBuffer[i])	KeyFrame[i]++; // ���������ꂽ��J�E���g�𑝂₷
			else				KeyFrame[i] = 0; // ����������ĂȂ�������J�E���g��0�ɂ���
		}

		// ��̓�̃R�����g�Ɠ��l�̂��Ƃ����Ă���
		if (CheckMouseInput(MOUSE_INPUT_LEFT))	MouseLeftFrame++;
		else									MouseLeftFrame = 0;
		// ��Ɠ��l
		if (CheckMouseInput(MOUSE_INPUT_RIGHT))	MouseRightFrame++;
		else									MouseRightFrame = 0;
		
		NowMouseX = GetMouseX();
		NowMouseY = GetMouseY();

		// �Q�[���X�V����
		GameUpdate();

		// Effekseer�X�V����
		UpdateEffekseer3D();

		// �Q�[���`�揈��
		GameDraw();
	
		// �}�E�X�̍��W���ړ��������W�ɕύX
		BeforeMouseX = NowMouseX;
		BeforeMouseY = NowMouseY;

		if (mouse_pos_middle_flag == false)
		{
			//	�}�E�X���W��ݒ肷��i��ʂ̐^�񒆂ɌŒ肷��j
			SetMousePoint(SCREEN_W / 2, SCREEN_H / 2);
		}
		
		
		// �P�Q�F�`�悪�����������̂���ʂɉf���܂�
		ScreenFlip();

		// ���ꂪ����ƃt���[�����[�g���Œ�ł���
		while (GetNowCount() - Time < 17) {}

		// �T�F�Ԃ�X�{�^�����������烋�[�v�𔲂���i�E�B���h�E�������j
		if (ProcessMessage())
		{
			break;
		}

		// �T�F�E�V�t�g�L�[����������I��
		if (CheckHitKey(KEY_INPUT_RSHIFT))
		{
			break;
		}
	}
	// �Q�[���I������
	GameExit();
	InputPadExit();

	//--------------------------
	// Effekseer���I������B
	Effkseer_End();
	//--------------------------

	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}

//---------------------------------------------------------------------------------
//	�L�[�������ꂽ�u�Ԃ��擾����
//---------------------------------------------------------------------------------
bool PushHitKey(int key)
{
	if (KeyFrame[key] == 1)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------
//	�}�E�X��������Ă��邩���擾����
//---------------------------------------------------------------------------------
bool CheckMouseInput(int button)
{
	if (GetMouseInput() & button)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------
//	�}�E�X�������ꂽ�u�Ԃ��擾����
//---------------------------------------------------------------------------------
bool PushMouseInput(int button)
{
	if (button & MOUSE_INPUT_LEFT)
	{
		if (MouseLeftFrame == 1)
		{
			return true;
		}
	}

	if (button & MOUSE_INPUT_RIGHT)
	{
		if (MouseRightFrame == 1)
		{
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------------
//	�}�E�X��X���W���擾����
//---------------------------------------------------------------------------------
int GetMouseX() // X���W
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_x;
}

//---------------------------------------------------------------------------------
//	�}�E�X��X���W���擾����
//---------------------------------------------------------------------------------
int GetMouseY() // Y���W
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_y;
}

//---------------------------------------------------------------------------------
//	�}�E�X��X���W�̈ړ��ʂ��擾����
//---------------------------------------------------------------------------------
int GetMouseMoveX()
{
	return NowMouseX - BeforeMouseX;
}

//---------------------------------------------------------------------------------
//	�}�E�X��X���W�̈ړ��ʂ��擾����
//---------------------------------------------------------------------------------
int GetMouseMoveY()
{
	return NowMouseY - BeforeMouseY;
}

//---------------------------------------------------------------------------------
//	�Œ肳�ꂽ�}�E�X��X���W�̈ړ��ʂ��擾����
//---------------------------------------------------------------------------------
int GetFixedMouseMoveX()
{
	int move_x;
	// ��ʂ̒��S�����W
	int fixed_x = SCREEN_W / 2;
	move_x = GetMouseX() - fixed_x;
	return move_x;
}

//---------------------------------------------------------------------------------
//	�Œ肳�ꂽ�}�E�X��Y���W�̈ړ��ʂ��擾����
//---------------------------------------------------------------------------------
int GetFixedMouseMoveY()
{
	int move_y;
	// ��ʂ̒��S�����W
	int fixed_y = SCREEN_H / 2;
	move_y = GetMouseY() - fixed_y;
	return move_y;
}

//---------------------------------------------------------------------------------
//	�}�E�X�̈ʒu���Œ肷�邩��ϓ��ł��邩��؂�ւ����
//---------------------------------------------------------------------------------
void ChangeMousePosMove(bool mouse_pos_flag)
{
	mouse_pos_middle_flag = mouse_pos_flag;
}

//---------------------------------------------------------------------------------
//	�x�����W�A���ɕϊ�����֐�
//---------------------------------------------------------------------------------
float TO_RADIAN(float degree)
{
	return degree * 3.14159265f / 180.0f;
}

//---------------------------------------------------------------------------------
//	���W�A����x�ɕϊ�����֐�
//---------------------------------------------------------------------------------
float TO_DEGREE(float radian)
{
	return radian * 180.0f / 3.14159265f;
}