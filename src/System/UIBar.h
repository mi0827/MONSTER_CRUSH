#pragma once
//! @file UIBar.h
//! @brief �o�[���UI�̃N���X
class UIBra
{
public:
	//! �R���X�g���N�^
	UIBra();
	//! �f�X�g���N�^
	~UIBra();

	//! @brief �o�[�̏�����
	//! @param �o�[�̍��W
	//! @param �o�[�̃T�C�Y
	//! @param �ݒ肵�������l(�ő�l)
	//! @param �g���쐬���邩�ǂ���
	void Set(Vector2 pos, Vector2 size, int* value, bool line);

	//! @brief �o�[�̐F�̐ݒ�
	//! @param �Ԃ̒l
	//! @param �΂̒l
	//! @param �̒l
	//! @param UIBra�N���X�̊e�J���[�ϐ��̂ǂꂩ������(�ǂ̐F��ς����ϐ�������)
	void SetColor(int red,int green, int blue, int* color);
	
	//! @brief ���O�̐ݒ�֐�
	//! @param ���O
	void SetName(const char name[256]);

	//! @brief �X�V����
	void Update(int value);

	//! @brief �`��p�֐�
	void Draw();

	//! @brief �o�[�Ɠ_�Ƃ̓����蔻���Ԃ�
	//! @param x���W
	//! @param y���W
	//! @return true : �q�b�g���Ă���Afalse : �q�b�g���Ă��Ȃ�
	bool HitPointBra(float pos_x, float pos_y);

public:
	//! �ݒ肵�������l��ۑ�����ϐ�
	int m_value = 0;
	//! �{�̂��J���[
	int m_color = 0;
	//! �o�b�N�J���[
	int m_back_color = 0;
	//! �O�g�̃J���[
	int m_line_color = 0;
	//! �����̃J���[
	int m_character_color = 0;

private:
	//! �o�[�̍��W
	Vector2 m_pos1{0,0};
	Vector2 m_pos2{ 0,0 };
	//! �o�[�̑傫��
	Vector2 m_size{0,0};

	//! �o�b�N�o�[�̍��W
	Vector2 m_back_pos1{ 0,0 };
	Vector2 m_back_pos2{ 0,0 };

	//! �o�[���C���̃��C�����������p�̍��W
	Vector2 m_line_pos1{0,0};
	Vector2 m_line_pos2{ 0,0 };
	//! �o�[�̃��C�����������p�̑傫��
	Vector2 m_line_size{0,0};
	//! ���C�������₷���Ȃ�悤�ɏ������߂ɂ���
	static constexpr float LINE_SIZE_X = 5.0f;
	static constexpr float LINE_SIZE_Y = 5.0f;

	//! �O�g�̃��C�����������Ђ��Ȃ����𔻒f����ϐ�
	bool m_line_judgment = false;

	//! value�̍ő�l��ۑ����Ă����ϐ�
	int m_value_max = 0;

	//! value�̒l���P���邽�тɃo�[�̌����
	float value_decrease = 0;

	// ���ǂꂾ���������Ăǂ�قڎc���Ă��邩
	float new_value = 0;

	//! ���O��ۑ����邽�߂̕ϐ�
	 char m_name[256] ;
	 // ���O��`�悷��̕ϐ�(�ŏ��͖��O�Ȃ���Ԃ��炷��)
	 bool m_name_judge = false;
};

