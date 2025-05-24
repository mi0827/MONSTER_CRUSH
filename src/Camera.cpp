#include "WinMain.h"
#include "src/System/Random.h"
#include "System/Vector3.h"
#include "System/Vector2.h"

#include "src/System/Transform.h"
#include "Camera.h"

//	���̍��W�Ƀ}�E�X���Œ肵�悤�Ǝv���܂�
#define FIXED_X		(SCREEN_W/2)	//	�w���W
#define FIXED_Y		(SCREEN_H/2)	//	�x���W

//---------------------------------------
// �R���X�g���N�^(������)
//---------------------------------------
Camera::Camera()
{
	//! �J�������W�̏����ݒ�
	m_pos.set(0.0f, 0.0f, -20.0f);
	//! �J�����̌����͑S���O�x�ŊJ�n
	m_rot.set(0.0f, 0.0f, 0.0f);

	//! �J������������W
	m_look.set(0.0f, 0.0f, 0.0f); // ���ׂĂO�D�O���ŏ�����

	//! �}�E�X�̈ړ��ʂ̏������ŏ���0.0f����
	m_mouse_move_x = 0.0f;
	m_mouse_move_y = 0.0f;
	

	// �J�����̌���ʒu�Ƌ����̐ݒ�
	SetCamera();
}

// ��������
void Camera::Init()
{
}

//---------------------------------------------------------------------------------
//	// �v���C��ʂ̏����ݒ�
//---------------------------------------------------------------------------------
void Camera::PlayFieldInit()
{
	//m_field_size.set(SCREEN_W , SCREEN_H);                                              // �`�悷���ʂ̃T�C�Y�̐ݒ�
	//m_screen_field = MakeScreen((int)m_field_size.x, (int)m_field_size.y);              // �`���ʂ��������ނ��߂̏����ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 2.0f, -50.0f), VGet(0.0f, 0.0f, 1.0f));  // �J�����̐ݒ�
}



//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void Camera::MouseCamera(Vector3* target_pos)
{
	// �}�E�X�z�C�[���ŃJ�����̋����̕ύX
	ChangeDistance();

	//m_before_pos.set(m_pos); //< �ړ��O�̍��W�̐ݒ�
	// �v���C���[�̌��ɕt���ē���
	m_look.set(target_pos->x, target_pos->y + m_look_height, target_pos->z);


	// �J�������n�ʂɖ��܂�Ȃ��悤�ɂ��Ă���
	if (m_rot.x <= LOWER_ANGLE)
	{
		m_rot.x = LOWER_ANGLE;
	}

	// �J�������^��ɂ����Ȃ��悤�ɂ��Ă���
	if (m_rot.x >= UP_ANGLE_MAX)
	{
		m_rot.x = UP_ANGLE_MAX;
	}

	//==========================================
	// �Q�[���p�b�h�p�̏���
	//==========================================
	//	�Q�[���p�b�h�̉E�X�e�B�b�N�̒l���g���Č����ϐ��i m_rot �j�̒l��ύX
	// ���X�e�b�N�Ńv���C���[�̌�������W�̍X�V
	// �Q�[���p�b�h�̏����擾�iXINPUT �̏��j
	XINPUT_STATE input;
	// �Q�[���p�b�h�̏����ۂ��Ǝ擾
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	// �ړ��p�x�N�g���p�ϐ�
	Vector3 rot;
	// ���X�e�B�b�N�̒l��ݒ�
	rot.y = input.ThumbRX;
	rot.x = input.ThumbRY;
	// -32768 �` 32767 ��-1.0f�@�`�@1.0f�ɂ��܂�
	rot /= 32768.0f;
	// ���̈ړ��p�x�N�g���̑傫����������x�傫���������ړ������悤�Ǝv���܂�
	if (rot.GetLength() > 0.5f)
	{
		m_rot.y += rot.y * PAD_CAMERA_ROT_SPEED;
		m_rot.x -= rot.x * PAD_CAMERA_ROT_SPEED;
		// m_rot += rot * PAD_CAMERA_ROT_SPEED;  // ���̈ړ��x�N�g�������W�ړ�
	}

	//	�}�E�X�̈ړ���
	Vector2 move;

	//	�Œ肳�ꂽ�}�E�X�̂���ړ��p���Q�b�g
	move.x = (float)GetFixedMouseMoveX();
	move.y = (float)GetFixedMouseMoveY();

	// ��]�͂��̂܂܂��Ƒ傫������̂ŏ�����
	//�ړ��ʂ����������܂�
	move *= m_camera_mouse_sensi;
	m_rot.y += move.x;
	m_rot.x += move.y;


	// �܂��͉�]�O�̃x�N�g����p�ӂ��܂�
	// �J����������v���C���[�����̃x�N�g�����쐬���܂�
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// �s���p�ӂ��܂�
	// X����]�s��
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	// Y����]�s��
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));
	// X����]��Y����]�����������̂łQ�̍s����P�ɂ܂Ƃ߂܂�
	MATRIX mat = MMult(mat_x, mat_y);
	// ���̃x�N�g�����w����]�Ƃx����]�����܂�
	// �ȒP�Ɍ���������̋����̖_������Ă���
	VECTOR change_dir = VTransform(base_dir, mat);

	// �J�����̈ʒu�����Ă�����W������̈ʒu�ɍĐݒ�
	m_pos = m_look + change_dir;
}

//---------------------------------------------------------------------------------
//	�^�[�Q�b�g�J�����̍X�V����
//---------------------------------------------------------------------------------
void Camera::TargetCamera(Vector3* target_pos1, Vector3* target_pos2)
{
	// �}�E�X�z�C�[���ŃJ�����̋����̕ύX
	ChangeDistance();

	// �������^�[�Q�b�g�̐ݒ�i����͉��̃^�[�Q�b�g������j
	m_look.set(target_pos1->x, target_pos1->y + m_look_height, target_pos1->z);

	// �J������Y����]�s��
	MATRIX  camera_mat = MGetRotY(TO_RADIAN(m_rot.y));
	// �J�����̑O�������x�N�g��
	Vector3 camera_dir = VGet(camera_mat.m[2][0], camera_mat.m[2][1], camera_mat.m[2][2]);
	camera_dir.y = 0.0f;
	camera_dir.normalize();

	// ���������������x�N�g��
	// �J�������猩���^�[�Q�b�g���ǂ����̙��K�ɂ���̂��̃x�N�g��
	//Vector3 camera_pos =  m_pos.x,m_pos.z };
	//Vector3 target_pos = { target_pos2->x, target_pos2->z };
	Vector3  target_dir = *target_pos2 - m_pos;
	target_dir.y = 0.0f;
	target_dir.normalize();



	// �J�����̑O�������x�N�g���ƌ����Ăق��������x�N�g���̓�̃x�N�g���̓��ς����߂�
	inner_product = GetVector3Dot(camera_dir, target_dir);
	float inner_angle = TO_DEGREE( acosf(std::clamp(inner_product, -1.0f, 1.0f)) );

	// �J�����̑O�������x�N�g���ƌ������������K�̓�̃x�N�g���̓��ς����߂�
	cross_product = GetVector3Cross(camera_dir, target_dir);

	// �O�ςɂ���ĕ�����ς���
	if (cross_product.y > 0)
	{
			m_rot.y += std::min(inner_angle, TARGET_ROT_SPEED);	
	}
	else
	{
			m_rot.y -= std::min(inner_angle, TARGET_ROT_SPEED);
	}


	// �܂��͉�]�O�̃x�N�g����p�ӂ��܂�
	// �J����������v���C���[�����̃x�N�g�����쐬���܂�
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// �s���p�ӂ��܂�
	// X����]�s��
	// MATRIX mat_x = MGetRotX(TO_RADIAN(target1->rot.y));
	// Y����]�s��
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));

	// X����]��Y����]�����������̂łQ�̍s����P�ɂ܂Ƃ߂܂�
	//	MATRIX mat = MMult(mat_x, mat_y);
	// ���̃x�N�g�����w����]�Ƃx����]�����܂�
	// �ȒP�Ɍ���������̋����̖_������Ă���
	VECTOR change_dir = VTransform(base_dir, mat_y);

	// �J�����̈ʒu�����Ă�����W������̈ʒu�ɍĐݒ�
	m_pos = m_look + change_dir;
}

//---------------------------------------------------------------------------------
// �J�������ǂɖ��܂�Ȃ��悤�ɂ���(���܂������Ȃ�)
//---------------------------------------------------------------------------------
void Camera::UseCameraUpdate(bool change_camera, Vector3* target_pos1, Vector3* target_pos2)
{
	if (change_camera)
	{
		MouseCamera(target_pos1);
	}
	else
	{
		TargetCamera(target_pos1, target_pos2);
	}
}

//---------------------------------------------------------------------------------
// �J�������ǂɖ��܂�Ȃ��悤�ɂ���(���܂������Ȃ�)
//---------------------------------------------------------------------------------
void Camera::HitObject(Vector3* obj_pos, Vector3* obj_size)
{
	if (m_before_pos.x + m_hit_box_size.x >= obj_pos->x - obj_size->x && m_before_pos.x - m_hit_box_size.x <= obj_pos->x + obj_size->x)
	{
		m_pos.z = m_before_pos.z;
		m_pos.x = m_before_pos.x;
	}
	if (m_before_pos.z + m_hit_box_size.z >= obj_pos->z - obj_size->z && m_before_pos.z - m_hit_box_size.z <= obj_pos->z + obj_size->z)
	{
		m_pos.z = m_before_pos.z;
		m_pos.x = m_before_pos.x;
	}
}

//---------------------------------------------------------------------------------
//	�`��O�̃J�����̃Z�b�g
//---------------------------------------------------------------------------------
// �߂�ǂ��������Ǌ撣���čH������
void Camera::DrawSet()
{

}

//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void Camera::Draw()
{
	//	�J�����̐ݒ�
	SetCameraNearFar(0.1f, 3000.0f);
	
	SetupCamera_Perspective(TO_RADIAN(CAMERA_ANGLE_DEFAULT + m_change_angle_value));
	// �J�������W�ƌ�����W��n���ăJ�����̐ݒ�
	SetCameraPositionAndTarget_UpVecY(m_pos.VGet(), m_look.VGet());

	// Effekseer��3D�\���̐ݒ��DX���C�u������3D�\���̐ݒ�ɓ���������B
	Effekseer_Sync3DSetting();
}

//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void Camera::Exit()
{
}

//---------------------------------------------------------------------------------
//	�J�����ړ��̍ۂ̃}�E�X�̊��x��ݒ�
//---------------------------------------------------------------------------------
void Camera::SetCameraSensi(float mouse_sensi)
{
	// �}�E�X���x�ɂӂ��킵���l�ɏC�����Ă���ۑ�����
	m_camera_mouse_sensi = mouse_sensi * MOUSE_SENSI_CORRECT;
}

//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void Camera::SetCamera(float height, float length)
{
	// �J�����̌���Y���W�̃v���X���̐ݒ�
	m_look_height = height;
	// �J��������ڕW�܂ł̋����̐ݒ�
	m_length = length;
	// �J�����̉�p�̐ݒ�

	SetupCamera_Perspective(TO_RADIAN(CAMERA_ANGLE_DEFAULT));
}


//---------------------------------------------------------------------------------
//	�ڂ������ڕW���܂��悤�ɃJ�����̈ړ�����
//---------------------------------------------------------------------------------
void Camera::MoveCamera(Vector3* target_pos, int direction, float speed)
{
	// �v���C���[�̌��ɕt���ē���
	m_look.set(target_pos->x, target_pos->y + m_look_height, target_pos->z);

	if (direction == 0)
	{
		m_rot.y -= speed;
	}
	else // ������
	{
		m_rot.y += speed;
	}

	// �܂��͉�]�O�̃x�N�g����p�ӂ��܂�
	// �J����������v���C���[�����̃x�N�g�����쐬���܂�
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// �s���p�ӂ��܂�
	// X����]�s��
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	// Y����]�s��
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));

	// X����]��Y����]�����������̂łQ�̍s����P�ɂ܂Ƃ߂܂�
	MATRIX mat = MMult(mat_x, mat_y);
	// ���̃x�N�g�����w����]�Ƃx����]�����܂�
	// �ȒP�Ɍ���������̋����̖_������Ă���
	VECTOR change_dir = VTransform(base_dir, mat);

	// �J�����̈ʒu�����Ă�����W������̈ʒu�ɍĐݒ�
	m_pos = m_look + change_dir;
}

//---------------------------------------------------------------------------------
// �J�����V�F�C�N���s��
//---------------------------------------------------------------------------------
void Camera::CameraShakeLimited(float power, float time)
{
	// �U���̃p���[��0�̎�
	if (m_shake_time == 0.0f)
	{
		// �U���p���[��ݒ�
		m_power = power;
		m_shake_time = time;
		m_shake_tiam_count = m_shake_time * 60;
	}

	// �h��̋��������X�Ɏキ����
	// m_power -= m_shake_time_value;
	
	// 0��艺���Ȃ��悤�ɂ���
	m_power = std::max(0.0f, m_power);

	m_shake_tiam_count--;
	if(m_shake_tiam_count % 60 == 0)
	{
		m_shake_time--;
	}

	// �h�ꂪ����Ƃ�
	if (m_shake_time > 0.0f)
	{
		// ��p��ύX����l
		int value = GetRandomF(-m_power, m_power);

		if (m_shake_tiam_count % 2 == 0)
		{
			// �J�����̉�p�̕ύX�l���f�t�H���g�������łȂ����Œl��ς���
			if (m_change_angle_value == CAMERA_ANGLE_DEFAULT)
			{
				m_change_angle_value = CAMERA_ANGLE_DEFAULT + (value * 3);
			}
			else
			{
				m_change_angle_value = CAMERA_ANGLE_DEFAULT;
			}
		}
	}
	else
	{
		// ��������Ȃ��Ƃ��͗h��Ȃ�
		m_shake_pos.clear();
		// �f�t�H���g�̉�p�ɖ߂�
		SetupCamera_Perspective(TO_RADIAN(CAMERA_ANGLE_DEFAULT));
		// ��p�ύX�l�����f�t�H���g�ɖ߂��Ă���
		m_change_angle_value = 0;
		m_power = 0;
		m_shake_time = 0;
	}
}

//---------------------------------------------------------------------------------
// �J�����V�F�C�N�������I�ɏI��������֐�
//---------------------------------------------------------------------------------
void Camera::CameraShakeReset()
{
	// �f�t�H���g�̉�p�ɖ߂�
	SetupCamera_Perspective(TO_RADIAN(CAMERA_ANGLE_DEFAULT));
	// ��p�ύX�l�����f�t�H���g�ɖ߂��Ă���
	m_change_angle_value = 0;
	m_power = 0;
	m_shake_time = 0;
}


//---------------------------------------------------------------------------------
// �����̐ݒ�p�֐�
//---------------------------------------------------------------------------------
void Camera::SetCameraRot(Vector3 rot)
{
	m_rot.y = rot.y;
	// �܂��͉�]�O�̃x�N�g����p�ӂ��܂�
	// �J����������v���C���[�����̃x�N�g�����쐬���܂�
	VECTOR base_dir = VGet(0.0f, 0.0f, -m_length);

	// �s���p�ӂ��܂�
	// X����]�s��
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));
	// Y����]�s��
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));

	// X����]��Y����]�����������̂łQ�̍s����P�ɂ܂Ƃ߂܂�
	MATRIX mat = MMult(mat_x, mat_y);
	// ���̃x�N�g�����w����]�Ƃx����]�����܂�
	// �ȒP�Ɍ���������̋����̖_������Ă���
	VECTOR change_dir = VTransform(base_dir, mat);

	// �J�����̈ʒu�����Ă�����W������̈ʒu�ɍĐݒ�
	m_pos = m_look + change_dir;
}

//-------------------------------------------------------------------------------- -
// �J�����̌������擾����֐�
//---------------------------------------------------------------------------------
Vector3 Camera::GetCameraRot()
{
	return m_rot;
}

//-------------------------------------------------------------------------------- -
// �J�����ƃ^�[�Q�b�g�Ƃ̋������}�E�X�z�C�[���ŕύX����
//---------------------------------------------------------------------------------
void Camera::ChangeDistance()
{
	// �}�E�X�̃z�C�[���ł̕ύX
	m_mouse_wheel = (float)GetMouseWheelRotVol();
	m_length += m_mouse_wheel * 1.5f;

	if (GetJoypadNum() >= 1)
	{
		// �Q�[���p�b�g�ł̕ύX
		if (IsPadRepeat(PAD_ID::PAD_L))
		{
			m_length -= 1.5f;
		}
		if (IsPadRepeat(PAD_ID::PAD_R))
		{
			m_length += 1.5f;
		}
	}


	// ���
	if (m_length >= CAMERA_LENGTH_MAX)
	{
		m_length = CAMERA_LENGTH_MAX;
	};
	// ����
	if (m_length <= CAMERA_LENGTH_MIN)
	{
		m_length = CAMERA_LENGTH_MIN;
	};

}
