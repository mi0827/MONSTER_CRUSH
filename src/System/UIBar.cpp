#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"
#include "src/Hit/Hit.h"
#include "UIBar.h"

//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
UIBra::UIBra()
{
}

//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
UIBra::~UIBra()
{
}

//-----------------------------------------------
// �����ݒ�
//-----------------------------------------------
void UIBra::Set(Vector2 pos, Vector2 size, int* value, bool line)
{
	// �ݒ�̕ۑ�
	m_pos1 = pos;                // ���W�P
	m_size = size;                 // �T�C�Y
	m_pos2 = pos + size;	      // ���W�Q
	m_value = *value;			  // ���l
	m_value_max = *value;   // ���l�̍ő吔��ۑ�
	m_line_judgment = line;	  // �O�g����邩�ǂ���

	// �o�b�N�o�[�̐ݒ�
	m_back_pos1 = m_pos1;
	m_back_pos2 = m_pos2;

	// �O�g�̃��C���������ꍇ
	if (m_line_judgment)
	{
		/*m_line_size.x = m_size.x + LINE_SIZE_X;
		m_line_size.y = m_size.y + LINE_SIZE_Y;*/
		// �O�g�̃��C���̐ݒ�
		m_line_pos1.x = m_pos1.x - LINE_SIZE_X;
		m_line_pos1.y = m_pos1.y - LINE_SIZE_Y;

		m_line_pos2.x = m_pos2.x + LINE_SIZE_X;
		m_line_pos2.y = m_pos2.y + LINE_SIZE_Y;
		SetColor(255, 255, 255, &m_line_color);
	}
	// �e�J���[�̐ݒ�
	SetColor(128, 128, 128, &m_color);
	SetColor(0, 0, 0, &m_back_color);
	SetColor(255, 255, 0, &m_character_color);

	// value�̒l���P���邽�тɌ���ʂ̐ݒ�
	value_decrease = m_size.x / m_value;


}

//-----------------------------------------------
// �J���[�̃Z�b�g�p�֐�
//-----------------------------------------------
void UIBra::SetColor(int red, int green, int blue, int* color)
{
	// �F�̐ݒ�
	*color = GetColor(red, green, blue);
}

//-----------------------------------------------
// ���O�̐ݒ�
//-----------------------------------------------
void UIBra::SetName(const char name[256])
{
	for (int i = 0; i < 256; i++)
	{
		m_name[i] = name[i];
	}
	// ���O�̐ݒ�������̂Ŗ��O�̃t���O��ݒ肷��
	m_name_judge = true;
}

//-----------------------------------------------
// �X�V����
//-----------------------------------------------
void UIBra::Update(int value)
{
	if (value <= 0)
	{
			value = 0;
	}
	m_value = value;
	// m_value �̐��l���O�����ɂȂ�����o�[������ȏ�Ђ���Ăق����Ȃ�

		// value���ǂꂾ�������Ă���̂��𒲂ׂ�
	int value_difference = m_value_max - m_value;

	// �����Ă��镪�̃o�[�����炵����
	new_value = value_decrease * value_difference;
}

//-----------------------------------------------
// �`�揈��
//-----------------------------------------------
void UIBra::Draw()
{
	// �O�g�̃��C���������ꍇ
	if (m_line_judgment)
	{
		DrawBox((int)m_line_pos1.x, (int)m_line_pos1.y, (int)m_line_pos2.x, (int)m_line_pos2.y, m_line_color, TRUE);
	}

	// �o�b�N�o�[�̕`��
	DrawBox((int)m_back_pos1.x, (int)m_back_pos1.y, (int)m_back_pos2.x, (int)m_back_pos2.y, m_back_color, TRUE);

	DrawBox((int)m_pos1.x, (int)m_pos1.y, (int)(m_pos2.x - new_value), (int)m_pos2.y, m_color, TRUE);

	if (m_name_judge)
	{
		// ���݂̃t�H���g�̃T�C�Y��ۑ����Ă��u��
		int font_size = GetFontSize();
		// �t�H���g�̃T�C�Y��ݒ�i�o�[�̃T�C�Y�ɍ��킹��j
		SetFontSize((int)m_size.y);
		// ���O�̕`��
		DrawString((int)(m_back_pos1.x + 5), (int)(m_back_pos1.y), m_name, m_character_color);
		// �t�H���g�̃T�C�Y�����Ƃ̃T�C�Y�ɖ߂�
		SetFontSize(font_size);
	}
}

//-----------------------------------------------
// �o�[�Ɠ_�Ƃ̓����蔻���Ԃ�
//-----------------------------------------------
bool UIBra::HitPointBra(float pos_x, float pos_y)
{
	// �|�C���g�̍��W
	Vector2 point_pos = { pos_x,pos_y };
	if (CheckPointBoxHit(point_pos, m_pos1, m_size))
	{
		return true;
	}
	else
	{
		return false;
	}
}
