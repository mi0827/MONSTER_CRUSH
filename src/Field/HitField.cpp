#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/InfoText.h"
#include "src/Model/Model.h"
#include "HitField.h"

//---------------------------------------------------------------------------
// �n�ʃ��f���̓����蔻��̐ݒ�
//---------------------------------------------------------------------------
void SetHitGround(Model* ground_model)
{
	// ��������̍쐬
	MV1SetupCollInfo(ground_model->m_model);
}


//---------------------------------------------------------------------------
// �n�ʃ��f���ƃL�����N�^�\���f���̃��f�����m�̓����蔻��
//---------------------------------------------------------------------------
void HitGroundCharacter(Vector3* character_pos, Model* ground_model)
{
	// ��������̍X�V
	MV1RefreshCollInfo(ground_model->m_model);

	// ���̊J�n���W(�L�����N�^�[�̏�̂ق��̍��W)
	Vector3 line_start = *character_pos + Vector3(0.0f, 100.0f, 0.0f);
	// ���̃S�[�����W(�v���C���[�̉��̂ق��̍��W)
	Vector3 line_goal = *character_pos + Vector3(0.0f, -100.0f, 0.0f);
	// �n�ʃ��f���Ɛ��̓����蔻��
	MV1_COLL_RESULT_POLY character_hit = MV1CollCheck_Line(ground_model->m_model, -1, line_start.VGet(), line_goal.VGet());

	// �������Ă�����̔���
	if (character_hit.HitFlag)
	{
		// �L�����N�^�\���W��n�ʂ̍����ɍ��킹��
		character_pos->y = character_hit.HitPosition.y;
	}
}

//---------------------------------------------------------------------------
// �n�ʃ��f���ƃL�����N�^�[���f���̃��f�����m�̓����蔻��(�W�����v���s���ꍇ)
//---------------------------------------------------------------------------
void HitGroundCharacterJump(Vector3* character_pos, Vector3* jump_mov, bool* jump_flag, float up_speed, float down_speed, Model* ground_model)
{

	// �W�����v�̏����̒��g��������ƒ��n�����Ƃ��Ƀo�E���h����
	// ���z�ł͉�ʓ��Ńv���C���[�̂ق��Ɍ����Ĕ��ł���悤�ɂ�����


	// ��������̍X�V
	MV1RefreshCollInfo(ground_model->m_model);

	// ���̊J�n���W(�L�����N�^�[�̏�̂ق��̍��W)
	Vector3 line_start = *character_pos + Vector3(0.0f, 50.0f, 0.0f);
	// ���̃S�[�����W(�v���C���[�̉��̂ق��̍��W)
	Vector3 line_goal = *character_pos + Vector3(0.0f, -50.0f, 0.0f);
	// �n�ʃ��f���Ɛ��̓����蔻��
	MV1_COLL_RESULT_POLY character_hit = MV1CollCheck_Line(ground_model->m_model, -1, line_start.VGet(), line_goal.VGet());

	
	// �������Ă�����̔���
	if (character_pos->y <= character_hit.HitPosition.y)
	{
		static bool now_jump;
		// ��ւ̈ړ����x�̃��Z�b�g
		*jump_mov = { 0.0f,0.0f,0.0f };
		// �L�����N�^�\���W��n�ʂ̍����ɍ��킹��
		character_pos->y = character_hit.HitPosition.y;
	
		if (character_pos->y == character_hit.HitPosition.y&& now_jump == true)
		{
			*jump_flag = false;
			now_jump = false;
		}
		if (*jump_flag == true)
		{
			now_jump = true;
			*jump_mov += character_hit.Normal * 2.0 * up_speed;
		}
	}

	// ��ɉ��Ɉړ�
	jump_mov->y -= 0.1 * down_speed;

	// ��Ɉړ��ʂ𑫂�
	*character_pos += *jump_mov;
}

//VECTOR GetGroundPos(Vector3* character_pos, Model* ground_model)
//{
//	// ���̊J�n���W(�L�����N�^�[�̏�̂ق��̍��W)
//	Vector3 line_start = *character_pos + Vector3(0.0f, 100.0f, 0.0f);
//	// ���̃S�[�����W(�v���C���[�̉��̂ق��̍��W)
//	Vector3 line_goal = *character_pos + Vector3(0.0f, -100.0f, 0.0f);
//	// �n�ʃ��f���Ɛ��̓����蔻��
//	MV1_COLL_RESULT_POLY character_hit = MV1CollCheck_Line(ground_model->m_model, -1, line_start.VGet(), line_goal.VGet());
//	return  character_hit.HitPosition;
//}
