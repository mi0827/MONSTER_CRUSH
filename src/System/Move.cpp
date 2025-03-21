#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/Collision/BoxCollision.h"
#include "src/Model/Model.h"
#include "src/Collision/CapsuleCollision.h"
#include "Move.h"


Move::Move()
{
}


Move::~Move()
{
}


//---------------------------------------------------------------------------
// �ړ��Ɋւ���X�V�����p�֐�
//---------------------------------------------------------------------------
void Move::Update(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, const float* mov_speed, Vector3* player_pos, PAD_NO m_pad_no, Vector3 m_mov)
{	
	// �U���������
	LookingUpdate(player_rot, camera_rot);
	// �L�����ƃJ�����̌����̉�]�̍�
	//m_difference_rot = player_rot->y - camera_rot->y;
	
	// ���f����Y����]�ɐ�����������
	
	// �ړ����̓_�b�V������
    // �Q�[���p�b�h�̓��͏�Ԃ��Ƃ�
	//	�Q�[���p�b�h�̍��X�e�B�b�N�̒l���g���č��W�i m_pos �j�̒l��ύX
	// ���X�e�b�N�Ńv���C���[�̌�������W�̍X�V
	// �Q�[���p�b�h�̏����擾�iXINPUT �̏��j
	XINPUT_STATE input = { {'\0'}, '\0','\0',0,0,0,0 };
	// �Q�[���p�b�h�̏����ۂ��Ǝ擾
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	
	// ���X�e�B�b�N�̒l��ݒ�
	m_mov.x = input.ThumbLX;
	m_mov.z = input.ThumbLY;
	// -32768 �` 32767 ��-1.0f�@�`�@1.0f�ɂ��܂�
	m_mov /= 32768.0f;
	if (m_mov.GetLength() > 0.5f)
	{
		MoveGamePad(m_check_move, &m_mov, camera_rot, player_rot, player_pos, mov_speed);
	}
	
	// WASD�L�[�Ńv���C���[�̈ړ�
	// �E��ړ�
	if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_W))
	{
		MoveDiagonallyUpRight(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
	}
	else //����ړ�
		if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_W))
		{
			MoveDiagonallyUpLeft(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
		}
		else // �E���ړ�
			if (CheckHitKey(KEY_INPUT_D) && CheckHitKey(KEY_INPUT_S))
			{
				MoveObliqueLowerRight(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
			}
			else // �����ړ�
				if (CheckHitKey(KEY_INPUT_A) && CheckHitKey(KEY_INPUT_S))
				{
					MoveObliqueLowerLeft(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
				}
				else // ��ړ�
					if (CheckHitKey(KEY_INPUT_W))
					{
						MoveFront(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
					}
					else // ���ړ�
						if (CheckHitKey(KEY_INPUT_S))
						{
							MoveDhindo(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
						}
						else // ���ړ�
							if (CheckHitKey(KEY_INPUT_A))
							{
								MoveLeft(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
							}
							else // �E�ړ�
								if (CheckHitKey(KEY_INPUT_D))
								{
									MoveRight(m_check_move, camera_rot, player_rot, player_pos, mov_speed);
								}
}

//---------------------------------------------------------------------------
// �L�����N�^�[�̕ǎC��p�֐�(�{�b�N�X)
//---------------------------------------------------------------------------
void Move::MoveHit(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
{
	if (before_pos->x + hit_size->x >= box->m_box.hit_pos.x - box->m_box.half_size.x && before_pos->x - hit_size->x <= box->m_box.hit_pos.x + box->m_box.half_size.x)
	{
		// �c���������߂�
		player_pos->z = before_pos->z;
	}
	if (before_pos->z + hit_size->z >= box->m_box.hit_pos.z - box->m_box.half_size.z && before_pos->z - hit_size->z <= box->m_box.hit_pos.z + box->m_box.half_size.z)
	{
		// �c���������߂�
		player_pos->x = before_pos->x;
	}
}


//---------------------------------------------------------------------------
// �L�����N�^�[�̕ǎC��p�֐�(�~)
//---------------------------------------------------------------------------
void Move::MoveHitCapsule(Vector3* player_pos, float r, CapsuleCollision* capsule)
{
	// ���ꂼ��̍X�V�������I������̂Ńv���C���[��NPC�̈ʒu�֌W�����苗���߂Â��Ȃ��悤�ɂ��܂�
	// �P�F�v���C���[��NPC�̋��������߂�
	float distance = GetVector3Distance(*player_pos, capsule->m_capsule.pos1);
	// �Q�F��̋��������߂�i���ꂼ��̔��a�j
	float radius = r + capsule->m_capsule.radius;

	// �R�F�Q�L�����̋�������̒��������Z��������
	if (distance < radius) {
		// 4�F�ǂꂭ�炢���ɓ����Ă��邩�i���̒����������~���d�Ȃ��Ă���
		float in_lengef = radius - distance;
		// �T�F�ǂ̂��炷��
		Vector3 dir = *player_pos - capsule->m_capsule.pos1;

		// �U�F���̃x�N�g���̒����𒆂ɓ����Ă����钷���ɔ����ɂ��܂�
		dir.SetLength(in_lengef);
		// 7�F���̕������v���C���[���W���ړ�������
		*player_pos += dir;

	}
}

//---------------------------------------------------------------------------
// �Q�[���p�b�h�̈ړ��p�֐�(�Q�[���p�b�h�p)
//---------------------------------------------------------------------------
void Move::MoveGamePad(bool* m_check_move, Vector3* mov, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{

	*m_check_move = true; // �����Ă���
	// �����Ă�������ɍ��W�ړ�
	// �����蔻�肪���鎞�͍��W�ړ������Ȃ�
	//	���̃x�N�g������]�����邽�߂̍s����쐬���܂��B
	MATRIX mat_y = MGetRotY(TO_RADIAN(camera_rot->y));
	//	���̍s����g���ăx�N�g������]�����܂��B
	*mov = GetVector3VTransform(*mov, mat_y);
	// �ړ��x�N�g���̂����̂�����������߂܂�
	player_rot->y = TO_DEGREE(atan2f(mov->x, mov->z));
	// �ړ��x�N�g���̑傫���� PLAYER_MOV_SPEED �̂��������ɂ��܂�
	mov->SetLength(*mov_speed);
	// ���̈ړ��x�N�g�������W�ړ�
	*player_pos += *mov;

}

//---------------------------------------------------------------------------
// �O�ړ�
//---------------------------------------------------------------------------
void Move::MoveFront(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = 0.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;
	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// ���ړ�
//---------------------------------------------------------------------------
void Move::MoveDhindo(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = 180.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;

	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// ���ړ�
//---------------------------------------------------------------------------
void Move::MoveLeft(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = -90.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;
	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// �E�ړ�
//---------------------------------------------------------------------------
void Move::MoveRight(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = 90.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;
	
	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�	
	Vector3 move;
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// �E�΂ߏ�ւ̈ړ�
//---------------------------------------------------------------------------
void Move::MoveDiagonallyUpRight(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = 45.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;
	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�		
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// ���΂ߏ�ւ̈ړ�
//---------------------------------------------------------------------------
void Move::MoveDiagonallyUpLeft(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = -45.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;
	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�		
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// �E�΂߉��ւ̈ړ�
//---------------------------------------------------------------------------
void Move::MoveObliqueLowerRight(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = 135.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;
	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// ���΂߉��ւ̈ړ�
//---------------------------------------------------------------------------
void Move::MoveObliqueLowerLeft(bool* m_check_move, Vector3* camera_rot, Vector3* player_rot, Vector3* player_pos, const float* mov_speed)
{
	// ���Ăق��������Z�b�g
	m_face_num = 225.0f;
	// �⊮�t���O���グ��
	m_complementation_flag = true;
	// �����Ă���
	*m_check_move = true;
	// �����Ă�������ɍ��W�ړ�
	player_pos->x += *mov_speed * sinf(TO_RADIAN(player_rot->y));
	player_pos->z += *mov_speed * cosf(TO_RADIAN(player_rot->y));
}

//---------------------------------------------------------------------------
// �L�����N�^�[�̐U���������
//---------------------------------------------------------------------------
void Move::LookingUpdate(Vector3* chara_rot, Vector3* camera_rot)
{
	// �U������t���O���オ���Ă���Ƃ�
	if (m_complementation_flag)
	{
		// �L������Y����]�s��
		m_character_mat = MGetRotY(TO_RADIAN(chara_rot->y));
		// �v���C���[�̑O�����̃x�N�g��
		m_character_dir = VGet(m_character_mat.m[2][0], m_character_mat.m[2][1], m_character_mat.m[2][2]);
		// ����������������Y����]�s��
		m_face_mat = MGetRotY(TO_RADIAN(camera_rot->y + m_face_num)); // �J�������猩�����̉E����
		// ���������������x�N�g��
		m_face_dir = VGet(m_face_mat.m[2][0], m_face_mat.m[2][1], m_face_mat.m[2][2]);
		// �L�����̑O���x�N�g���ƌ����Ăق��������x�N�g���̓�̃x�N�g���̓��ς����߂�
		inner_product = GetVector3Dot(m_character_dir, m_face_dir);
		// �L�����O�������x�N�g���ƌ��������������̓�̃x�N�g���̊O�ς����߂�
		cross_product = GetVector3Cross(m_character_dir, m_face_dir);
		// �O�ςɂ���ĕ�����ς���
		if (cross_product.y > 0)
		{
			if (inner_product <= 0.99)
			{
				chara_rot->y += 10.0f;
			}
			else
			{
				// �U�������������߂�
				m_complementation_flag = false;
			}
		}
		else
		{
			if (inner_product <= 0.99)
			{
				chara_rot->y -= 10.0f;
			}
			else
			{
				// �U�������������߂�
				m_complementation_flag = false;
			}
		}
	}
	
}
