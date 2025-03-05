#pragma once
//! @file Option.h
//! @brief �I�v�V������ʗp�N���X
class Option
{
public:
	// �R���X�g���N�^
	Option();
	// �f�X�g���N�^
	~Option();
	// ��������
	void Init();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	//! @brief �e�I�v�V�������j���[�̃o�[�̐ݒ�
	void SetOptionMenu();

	//! @brief ���j���[�Z���N�g
	void MenuSelect();

	//! @brief ���j���[��ʂ��J���Ă���Ƃ��̍X�V����
	void OpenMenuUpdate();
private:
	//! @brief �T�E���h�̍Đ�
	//! @param �Đ�������SE�ԍ�
	void SoundPlay(int se_num);

private:
	// �͈�
	static constexpr int VOLUME_MAX = 250;  // �ő�
	static constexpr int VOLUME_MIN = 0; // �ŏ�
	static constexpr int VOLUME_CONSTANT_VALUE = 25; // VOLUME���P�ŕω�����l
	static constexpr int MOUSE_SENSI_MAX = 50; // �}�E�X���x�̍ő�l
	static constexpr int MOUSE_SENSI_MIN = 5; // �}�E�X���x�̍Œ�l
	static constexpr int MOUSE_SENSI_VALUE = 5; // �}�E�X���x����ς��ƕς��l
	static constexpr int MARGIN = 50;
	// ���j���[�{�^����������Ă������܂łɂ�����Œ᎞��
	static constexpr int MENU_COUNT = 5;

	static constexpr int BOX_SIZE_X = SCREEN_W / 2 - 100;         // �l�p�̔w�iX�����̃T�C�Y
	static constexpr int BOX_SIZE_Y = SCREEN_H / 2;        // �l�p�̔w�iY�����̃T�C�Y
	static constexpr int BOX_SIZE_HARF_X = BOX_SIZE_X / 2; // �w�iX�����̔����̃T�C�Y
	static constexpr int BOX_SIZE_HARF_Y = BOX_SIZE_Y / 2; // �w�iY�����̔����̃T�C�Y
	// �e�o�[�̐ݒ�Ŏg��
	static constexpr int BRA_X_DIVISION = BOX_SIZE_X / 4 + 30;  // �w�i�摜�pX���W�w��̕����ŕ��������̑傫��
	static constexpr int BRA_Y_DIVISION = BOX_SIZE_Y / 4 - 20;  // �w�i�摜�pY���W�w��̕����ŕ��������̑傫��


	static constexpr int BAR_X_SIZE = 550;                 // �o�[��X���W�̃T�C�Y
	static constexpr int BAR_Y_SIZE = 50;                  // �o�[�̍��W�̃T�C�Y
	static constexpr int BAR_SPACE_X = BOX_SIZE_HARF_X / 2; // ���S����ǂꂭ�炢���Ԃ��J���邩�̒l(X���W)
	static constexpr int BAR_Y = VOLUME_MAX / 2;            // ���S����ǂꂭ�炢���Ԃ��J���邩�̒l(Y���W)


public:
	int m_bgm_Volume = 0; // BGM�̉���
	int m_se_Volume = 0;  // SE�̉���
	int m_mouse_sensi = 0; // �}�E�X���x
	bool m_option_flag = false;    // �I�v�V�������j���[���J�����߂̂���



	// �I�v�V�������j���[���
	enum Menu
	{
		BGM,     // ����
		SE,        // ����
		MOUSE, //�}�E�X�̊��x

		MAX
	};

	// ����������͂Ԃ������j���[�̍ő吔
	static constexpr int MENU_MAX = MAX;

	//! �I�v�V�������j���[��ʂ̔w�i�̒��S���W
	Vector2 m_option_box_pos = { MARGIN , 0 + BOX_SIZE_HARF_Y };
	
	// ����������̕`����W
	Vector2 m_operation_instructions_pos = { SCREEN_W / 2 + MARGIN,  +MARGIN };

	// �e�o�[��X���W
	static constexpr float BRA_X_STRAT = 0 + BOX_SIZE_HARF_X - BRA_X_DIVISION;

	// �e�X�e�[�^�X�Ŏg������
	struct Information
	{
		Vector2 m_pos;
		// �l
		int m_value = 0;
		// ������p�̓��ꕨ
		const char* m_name;
		// �o�[�̐F
		int m_color;
	};
	// �I�v�V�������j���[�̂R����p��
	Information option_menu[MENU_MAX]
	{
		//! BGM�p
		{{ BRA_X_STRAT ,m_option_box_pos.y + BRA_Y_DIVISION},  VOLUME_MAX , "BGM"},
		//! SE�p
		{{ BRA_X_STRAT ,m_option_box_pos.y + BRA_Y_DIVISION * 2}, VOLUME_MAX , "SE"},
		//! �}�E�X���x�p
		{{ BRA_X_STRAT ,m_option_box_pos.y + BRA_Y_DIVISION * 3},  MOUSE_SENSI_MAX, "�}�E�X���x"},
		
	};
private:
	// ���ꂼ��ŕ`�悷��o�[
	UIBra m_bra[MENU_MAX];
	UIBra m_op;

	// �e�o�[�̃T�C�Y�̐ݒ�
	Vector2 m_bra_size = { BAR_X_SIZE, BAR_Y_SIZE };

	// �I�v�V�������j���[�ł�SE
	Sound m_se;
	// SE�̎�ނ̗񋓑�
	enum SE
	{
		open_menu_se, // �I�v�V�������j���[���J������	
		close_menu_se, // �I�v�V�������j���[�����Ƃ�
		selection_menu_se,// �e���j���[��I�����Ă���Ƃ�
		change_value_se, // �e���l��ύX���Ă���Ƃ�
		se_max
	};

	// SE
	struct SEInfo
	{
		// �Đ�����^�C�v
		int play_type;
		// ���[�v���邩�ǂ���
		bool loop;
	}; 
	SEInfo m_se_info[se_max]
	{
		{DX_PLAYTYPE_BACK, true},
		{DX_PLAYTYPE_BACK, true},
		{DX_PLAYTYPE_BACK, true},
		{DX_PLAYTYPE_BACK, true}
	};


	// ���݂ǂ̃��j���[���I�������Ă��邩��ۑ�����
	int m_selection_menu = BGM;
	// �w�i�摜�p�̓��ꕨ
	int m_image_box = 0;
	// ��������̉摜�p�̓��ꕨ
	int m_operation_instructions_image = 0;
	// ���j���[�{�^����������Ă������܂łɂ�����Œ᎞�Ԃ��J�E���g���邽�߂̕�
	int m_menu_count = 0; 
	// SE��BGM����󂷂邽�߂̂���
	int m_select = 0; 


	// �Q�[���p�b�h�p�̕ϐ�
	int pad_input;
	// �P�Ԃ̃Q�[���p�b�h���Z�b�g
	PAD_NO pad_no = PAD_NO::PAD_NO1;

	// �Q�[���p�b�h�̔ԍ����Z�b�g����
	// DX���C�u�����[���ł̃p�b�h�̔ԍ����擾
	void SetPadNo(PAD_NO no, int dx_no)
	{
		pad_no = no;
		pad_input = dx_no;
	}

};