#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include <fstream>
#include <string>
#include "Text.h"
using namespace std;

// ���̃\�[�X���Ȃ��ƌx�������o�ăG���[�ɂȂ�
#pragma warning(disable : 4996)

Text::Text()
{
}

Text::~Text()
{
	// �Ō�ɊJ�����Ă���
	lines.clear();
}

//---------------------------------------------------------------------------
// �e�L�X�g�t�@�C���̓ǂݍ��ݏ���
//---------------------------------------------------------------------------
void Text::LoadText(const char file_path[256], const int line_max)
{

	// �e�L�X�g�t�@�C���̓ǂݍ���
	// �t�@�C���̃f�[�^���������
	ifstream file(file_path);
	if (file.is_open() == true)
	{
		while (true)
		{
			string line;
			getline(file, line);
			// �f�[�^�̈�ԍŌ�ɍs������
			if (file.eof())
			{
				break;
			}
			// vector�̔z��ɒǉ�
			lines.emplace_back(line);
		}
		// �t�@�C�������
		file.close();
	}
}

//---------------------------------------------------------------------------
// �`�揈��
//---------------------------------------------------------------------------
void Text::TextDraw(int line_num, Vector2 draw_pos , int back_size)
{
	// ���e�L�X�g����s�Â�C�ɏo�Ă��邩��
	// �ꕶ���ꕶ���o�Ă���悤�ɂ���

	// �t�H���g�T�C�Y���Ƃ��ĕ�����̔w�i�o�[�̑傫�������߂�
	int font_size = GetFontSize();
	Vector2 box_size;
	box_size.set(draw_pos.x + back_size+ MARGIN* 2, draw_pos.y + font_size+ MARGIN*2);

	// �����̌���`��ӂ�
	DrawBox((int)(draw_pos.x - MARGIN*2), (int)(draw_pos.y - MARGIN*2), (int)(box_size.x + MARGIN), (int)(box_size.y+ MARGIN), GetColor(m_r, m_b, m_g), TRUE);
	// �����̌��̂Ƃ���𔼓����ɂ���
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	// �����̌���`��
	DrawBox((int)(draw_pos.x - MARGIN), (int)(draw_pos.y - MARGIN), (int)box_size.x, (int)box_size.y, GetColor(255, 255, 255), TRUE);
	// �����x�����Ƃɂ��ǂ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);


	// �e�L�X�g�f�[�^����������ۑ�����
	const char* text = 0;
	text = lines[line_num].data();
	// �`�揈��
	DrawString((int)draw_pos.x, (int)(draw_pos.y + MARGIN), text, GetColor(0, 0, 0));
}


//---------------------------------------------------------------------------
// �e�L�X�g�̘g�̐F�̂��Z�b�g����֐�
//---------------------------------------------------------------------------
void Text::SetFrameColor(int r, int g, int b)
{
	// �ԐF
	m_r = r;
	// ��
	m_b = g;
	// ��
	m_g = b;
}
