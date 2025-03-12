#pragma once
// !@file InfoText.h
//! @�e�L�X�g�t�@�C��������̓ǂݎ��A�������ݗp�̃N���X
class InfoText
{
public:

	//! @brief �R���X�g���N�^
	InfoText();
	//! @brief �f�X�g���N�^
	~InfoText();

	//void Init();

	//! @brief �e�L�X�g�t�@�C�����當����̓ǂݍ��ݏ���
	//! @param �t�@�C���̃p�X
	//! @param �e�L�X�g�t�@�C���̒��g�̍s��
	void LoadText(const char file_path[256]);

	//! @brief �ǂݍ��񂾃e�L�X�g�̕����𐔎��ɕϊ�����悤�̊֐�
	//! @param �ǂݍ��񂾃e�L�X�g�̉��s�ڂ�ϊ���������
	//! @param �ϊ�����������ۑ����邽�߂̕ϐ��P
	//! @param �ϊ�����������ۑ����邽�߂̕ϐ��Q(�Ȃ���ΐݒ肳��Ȃ�)
	//! @param �ϊ�����������ۑ����邽�߂̕ϐ��R(�Ȃ���ΐݒ肳��Ȃ�)
	void ConversionNumbers(int text_num , float* variable_1, float* variable_2 = nullptr, float* variable_3 = nullptr);
	//! @brief �e�L�X�g�ɏ������ނ悤�ɕϐ�
	void UpText();


public:
	// �������ۑ�����z��̍ő吔
	static constexpr int CHAE_MAX = 256;

	// ���s�ǂݎ�����̂���ۑ����邽�߂̂���
	int m_LINES_MAX = 0;
	// �e�L�X�g�t�@�C�����當�����ǂݎ�������̂�ۑ��������
	// �����������
	std::vector<std::string> lines;



};