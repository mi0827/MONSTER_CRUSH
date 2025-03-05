#pragma once
// Windows �v���O�������g���̂�
#include <Windows.h>

// DX���C�u�������g���̂�
#include <DxLib.h>
#include <time.h> // ���Ԃ��g���̂ɂ���
#include <math.h> // �T�C���A�R�T�C���╽�����ȂǁA���w�I�Ȋ֐��ł��B

#include <vector>

#pragma warning (disable : 4819)

#include <d3d9.h>
//#include <d3dx9.h>

// ��ʃT�C�Y
#define SCREEN_W	1920//800//1280//1920
#define SCREEN_H	1080//450//720 //1080



#include "System/InputPad.h" // �Q�[���p�b�h

// EffekseerForDXLib.h���C���N���[�h���܂��B
#include "EffekseerForDXLib.h"




//------------------------
// �֐��̒�`
bool PushHitKey(int key); // �L�[�{�[�h��������Ă��邩������
// �}�E�X�������ꂽ�����镨
bool CheckMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
// �}�E�X��������Ă��邩���������
bool PushMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
// �}�E�X�̍��W���擾����
int GetMouseX(); // X���W
int GetMouseY(); // Y���W
// �}�E�X�̈ړ��ʂ��擾
int GetMouseMoveX(); // X���W
int GetMouseMoveY(); // Y���W
// �Œ肳�ꂽ�}�E�X�̈ړ��ʂ��擾
int GetFixedMouseMoveX();// X���W
int GetFixedMouseMoveY();// Y���W

//! @brief  �}�E�X�̈ʒu���Œ肷�邩��ϓ��ł��邩��؂�ւ����
//! @param true : �ϓ��\�Afalse : �Œ艻
void ChangeMousePosMove(bool mouse_pos_flag);

// �x�����W�A���ɕϊ�����֐�
float TO_RADIAN(float degree);
// ���W�A����x�ɕϊ�����֐�
float TO_DEGREE(float radian);

#include "src/System/Vector3.h"
void DrawCircle3D_XZ(Vector3 center, float radius, int color, bool fill = false);

const int BLACK = 0x000000;

