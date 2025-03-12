#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include <fstream>
#include <string>
#include "InfoText.h"
using namespace std;

InfoText::InfoText()
{
}
InfoText::~InfoText()
{
	// �Ō�ɊJ�����Ă���
	lines.clear();
}

//! @brief �e�L�X�g�t�@�C�����當����̓ǂݍ��ݏ���
//! @param �t�@�C���̃p�X
void InfoText::LoadText(const char file_path[256])
{
	// �e�L�X�g�t�@�C���̓ǂݍ���
	// �t�@�C���̃f�[�^���������
	ifstream file(file_path);
	if (file.is_open() == true)
	{
		while (true)
		{
			string line;
			// ��s�擾����
			getline(file, line);

			// ���s�ǂݎ�����̂��𑝂₷
			m_LINES_MAX++;

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

void InfoText::ConversionNumbers(int text_num, float* variable_1, float* variable_2, float* variable_3)
{
	// string�^����char�^�ɕύX
	// const char* text = lines[text_num].c_str();

	// �����ɓn���ꂽ�ϐ��ɂ���ď����������ς�� 
	// �ݒ肳�ꂽ��������̎�
	if (variable_2 == nullptr && variable_3 == nullptr)
	{
		sscanf_s(lines[text_num].c_str(), "%f", variable_1);
	}
	// �ݒ肳�ꂽ��������̎�
	else if (variable_3 == nullptr)
	{
		sscanf_s(lines[text_num].c_str(), "%f %f", variable_1, variable_2);
	}
	// �ݒ肳�ꂽ�������O�̎�
	else
	{
		sscanf_s(lines[text_num].c_str(), "%f %f %f", variable_1, variable_2, variable_3);
	}
}


