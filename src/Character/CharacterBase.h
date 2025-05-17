#pragma once
//! @file CharacterBase.h
//! @brief �L�����N�^�[�̃x�[�X�N���X
class CharacterBase
{
public:

	//-----------------------------------------------
	//! �֐��̐錾
	//-----------------------------------------------
	//! @brief �R���X�g���N�^
	CharacterBase();

	//! @brief �f�X�g���N�^
	~CharacterBase();

	//! �������z�֐�
	//! @brief ��������
	virtual void Init() = 0;

	//! @brief �X�V����
	//! @param �J�����̌���
	virtual void Update(Vector3* camera_rot) = 0;
	//! @brief �����Ă鎞�̍X�V����
	//! @param �J�����̌���
	virtual void LiveUpdate(Vector3* camera_rot) = 0;

	//! @brief �x�[�X�N���X�ł̏�������
	//! @param �G�̈ړ��̍ۂɎg�����a�̐ݒ�
	//! @param HP�̍ő��
	void BaseInit(float r, int HP);

	//! @brief ���񂾂Ƃ��̍X�V���� 
	virtual void DieUpdate() = 0;

	//! @brief �`�揈��
	virtual void Draw() = 0;

	//! @brief �I������
	virtual void Exit() = 0;

	//! @brief �����蔻��̍X�V����
	virtual void CDUpdate() = 0;

	//! @brief �X�e�[�^�X�o�[�̐ݒ�p�֐�
	virtual void StatusBarInit() = 0;

	//! @brief �X�e�[�^�X�o�[�̕`��֐�
	virtual void StatusBarDraw() = 0;

	//! @brief �����蔻����s���ė~�����^�C�~���O��ۑ������邽�߂̊֐�
	virtual void SetHitTimeInit() = 0;

	//! @brief �A�j���[�V�����̓ǂݍ��݂��܂Ƃ߂�֐�
	virtual void AnimLoadInit() = 0;

	//! @brief �G�t�F�N�g�̓ǂݍ��݂��܂Ƃ߂�֐�
	virtual void EffectLoadInit() = 0;

	//! @brief �G�t�F�N�g�̍X�V����
	//! @param �s�������G�t�F�N�g�ԍ�
	//! @param �s�������G�t�F�N�g�̏��ԍ�
	virtual void EffectUpdate(int effect_num, int effect_info_num) = 0;

	//! @brief SE�̓ǂݍ���
	virtual void SELoadInit() = 0;

	//! @brief SE�̍X�V����
	//! @param �s������SE�̏�񂪕ۊǂ���Ă���ԍ�
	virtual void SEUpdate(int se_info_num) = 0;

	//! @brief �v���C���[�̏��(�t���O)�Ǘ��֐�
	//! @param �v���C���[�̏��
	virtual void PlayerMode(int mode) = 0;

	//! @brief �U�����󂯂����̍X�V����
	virtual void ComeAttackUpdate() = 0;

	//! @brief �v���C���[�̈ړ��p�֐�
	//! @param Run�A�j���[�V�����̕ۑ�����Ă���A�j���[�V�����ԍ�
	//! @param �v���C���[�̈ړ��X�s�[�h
	//! @param �J�����̌���
	void MoveAction(int run_anim_no, const float move_speed,Vector3* camera_rot);

	//! @brief �L�����̕ǎC�蔻��p�̊֐�
	//! @param �L�����̍��W
	//! @param �����蔻�葊��̃{�b�N�X�̏��
	void MoveHitUpdate(BoxCollision* box);

	//! @brief �L�����N�^�[�̍��W��ݒ肷��֐�
	//! @param �ݒ肵�������W
	void SetCharacterPos(Vector3 pos);

	//! @brief �L�����N�^�[�̌�����ݒ肷��֐�
	//! @param �ݒ肵��������
	void SetCharacterRot(Vector3 rot);

	//! @brief �U���Ɋւ�������Z�b�g���邽�߂̊֐�
	//! @param �U���A�j���[�V�������n�܂�A�j���[�V�����ԍ�
	//! @param �ŏ��ɍU���������A�j���[�V�����ԍ�1
	//! @param �ŏ��ɍU���������A�j���[�V�����ԍ�2
	//! @param �R���{�U���̍ő�R���{��
	void SetAttackInfo(const int attack_start_anim, int attack_1, int attack_2, const int combo_max);

	//! @brief �ŏ��̍U�����s�����߂̊֐�
	void AttackFirst();

	//! @brief �U���̍X�V����
	void AttackUpdate();

	//! @brief �����蔻����s���Ăق����^�C�~���O���Z�b�g����֐�
	//! @param �����蔻����Ƃ��Ăق����t���[���̃X�^�[�g
	//! @param �����蔻����I����Ăق����t���[���̏I���
	//! @param �ۑ��������U���ԍ�
	//! @param �q�b�g�X�g�b�v���s���Ă�����
	void SetHitTiming(float attack_frame_start, float attack_frame_end,  bool can_hitstop, int attack_num);

	// �����蔻����Ƃ��Ă悢�^�C�~���O���𔻒f����֐�
	//! @brief �U���ԍ�
	bool AttackHitGoodTiming(int anim_num);

	//! @brief �����蔻��ƃ_���[�W��ۑ�����֐�
	//! @param �����蔻��p�̃J�v�Z��
	//! @param �^����_���[�W
	//! @param �U���ԍ�
	void SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num);

	//! @brief ���[�����O�A�N�V�������J�n����邽�߂̊֐�
	void RollingActionStart();

	//! @brief ���[�����O�A�N�V�����̍X�V����
	//! @param ���[�����O���ۑ�����Ă���A�j���[�V�����ԍ�
	//! @param ���[�����O���̈ړ��X�s�[�h
	void RollingActionUpdate(int rolling_anim_no,const int rolling_speed);

	//! @brief �J�E���^�[�A�N�V����
	//! @param �J�E���^�[�A�j���[�V�������ۑ�����Ă���ԍ�
	void CounterAction(int counter_anim_no);

	//! @brief �R���{�A�N�V�����̍X�V����
	void ComboActionUpdate();

	//! @brief �U�����󂯂������`�F�b�N���邽�߂̊֐�
	void CheckHitDamage();

	//! @biref �U�����󂯂Ď��̃A�j���[�V��������
	//! @param �_���[�W���󂯂��A�j���[�V�������ۑ�����Ă���ԍ�
	void HitDamageUpdate(int hit_damage_anim);


	// �U���̎��̓����蔻��ƃ_���[�W�̍\����
	// �e�q�N���X�Œ�`����
	struct Attack_Hit_Damage
	{
		//!  �U�����Ɏg�����������蔻��
		CapsuleCollision attack_hit;
		//! �U���ɂ������_���[�W
		int attack_damage = 0;

		//! �����蔻����Ƃ��Ăق����^�C�~���O
		//! �X�^�[�g
		int start_time = 0;
		//! �I���
		int end_time = 0;
		//! �q�b�g�X�g�b�v���s���Ă����U�����ǂ���
		bool can_hit_stop;
	};
	std::vector< Attack_Hit_Damage*> m_attack_hit_damage;
	//! @brief �U�����̓����蔻���ݒ肷��p�̊֐�
	//! @param �U���A�j���[�V�����̍ő吔
	void  NEWSetAttackHitDamage(int attack_anim_max);

	//! @brief �x�[�X�N���X�ł̍X�V����
	//! @param �P�F�ړ����������łȂ����̃t���O
	//! @param �Q�F�J�����̌���
	//! @param �S�F�ړ��X�s�[�h
	//! @detail �ړ��Ɋւ�����̂̍X�V����
	void BaseUpdate(bool* m_check_move, Vector3* camera_rot, const float* mov_speed);




	//-----------------------------------------------
	//! �Q�[���p�b�h�̏���
	//-----------------------------------------------
	// �Q�[���p�b�h�p�̕ϐ�
	int m_pad_input = 0;

	PAD_NO m_pad_no = PAD_NO::PAD_NO1;				// ���Ԃ̃p�b�h���g���Ă���̂�
	// �Q�[���p�b�h�̔ԍ����Z�b�g����
	// DX���C�u�����[���ł̃p�b�h�̔ԍ����擾
	void SetPadNo(PAD_NO no, int dx_no)
	{
		m_pad_no = no;
		m_pad_input = dx_no;
	}
	// �Q�[���p�b�h�̔ԍ����Ƃ��Ă���
	PAD_NO GetPadNo() const
	{
		return m_pad_no;
	}

public:


	//-----------------------------------------------
	//! �N���X�̃I�u�W�F�N�g���`
	//-----------------------------------------------
	//! ���W�A�����A�T�C�Y
	Transform m_transform;
	//! �ړ�
	Move m_move;
	//! �ړ��̍ۂ̓����蔻��Ɏg��
	BoxCollision m_move_hit_box;
	//! �T�E���h�N���X�̃I�u�W�F�N�g
	Sound m_se;
	//! ���f���N���X�̃I�u�W�F�N�g
	Model m_model;
	//! �A�j���[�V�����N���X�̃I�u�W�F�N�g
	Animation m_animation;
	//! �G�t�F�N�g�N���X�I�u�W�F�N�g
	Effect m_effect;
	//! �R���{�N���X
	Combo m_combo;
	//! �J�v�Z���R���W����
	//=================
	// �����蔻��
	//=================
	CapsuleCollision m_right_hand; //!< �E��̓����蔻��
	CapsuleCollision m_left_hand;   //!< �E��̓����蔻��
	CapsuleCollision m_body;         //!< �{�̂̓����蔻��
	CapsuleCollision m_left_feet;    //!< �����̓����蔻��
	CapsuleCollision m_right_feet;  //!< �E���̓����蔻��


	//! �ǎC�蔻��̂��߂ɂ���������W��ۑ����Ă����ϐ�
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
	//! �ړ��̍ۂ̓����蔻��p�̃T�C�Y
	Vector3 m_move_hit_size = { 5.0f ,5.0f,5.0f };

	//===============
	// ��ԊǗ��p�t���O
	//=============== 
	 //-----------------------------------------------
	// �񋓑̂ŊǗ�
	//-----------------------------------------------
	//! �v���C���[�̏��
	enum Situation
	{
		alive, // �����Ă�Ƃ�
		die,   // ����ł�
	};
	//! �����Ă邩��ۑ����邽�߂̕ϐ�
	// �ŏ��͐����Ă��Ԃ���n�߂�
	int m_life_and_death = alive;
	//! �v���C���[�̏��
	enum PlayerMode
	{
		IDLE,      // �ҋ@
		RUN,       // ������
		ROLLING, // ���
		COUNTER, // �J�E���^�[
		ATTACK,    // �U��
		HIT_DAMAGE, // �U�����󂯂�
		DIE        // ���S
	};
	//! �v���C���[�̃��[�h���Ǘ�����ϐ�
	int m_player_mode = 0;

	//! �A�C�h����Ԃ��̃t���O
	bool m_idle_flag = false;
	//! �����Ă����ȉ��̃t���O
	bool m_run_flag = false;
	//! ���[�����O�t���O
	bool m_rolling_flag = false;
	//! �U����Ԃ��ǂ����̃t���O
	bool m_attack_flag = false;
	//! �U�����󂯂����̃t���O
	bool m_damage_flag = false;
	//! �����̃J�E���^�[�U���p�̃t���O
	bool m_counter_flag = false;

	//! �U�����󂯂����̃A�j���[�V������ݒ肷�邽�߂̕ϐ�
	//! �Ȃ񂩂����_���[�W���󂯂����̃A�j���[�V�����Z�b�g�ɓ����Ăق����Ȃ��̂ō����
	//! �{���͂����Ƃ������@������Ǝv���������炻����ɕύX
	bool m_damage_anim_flag = false;
	//! �����蔻����Ƃ��Ă������̃t���O
	bool m_can_hit_damage_flag;

	

	//------------------------------------------
	// �A�j���[�V�����A�U���֘A
	//------------------------------------------
	//! ���̃A�j���[�V�����ԍ���ۑ�����p�̕ϐ�
	int m_now_attack_anim = 0;
	//! �U���A�j���[�V�����̎n�܂��ۑ�����ϐ�
	int m_ATTAK_START_ANIM_NO;
	//! ���߂ɂ������U���̃A�j���[�V�����ԍ���ۑ�����ϐ�
	int m_first_attack_anim_1;
	int m_first_attack_anim_2;

	// ����
	int aaa = 0;
	int bbb = 0;


	//! �R���{�̍ő�R���{���p�̕ϐ�
	int m_COMBO_MAX;
	//! �R���{�����񑱂��Ă��邩�̃J�E���g
	int m_combo_count = 0;
protected:
	//------------------------------------------
	// �R���{�֘A
	//------------------------------------------
	//! �R���{�p�t���O
	bool m_combo_flag = false;
	//! �R���{�̍ۃ}�E�X�̂ǂ���������ꂽ��
	int m_mouse_flag = 0; // MOUSE_INPUT_RIGHT : �E�{�^���AMOUSE_INPUT_LEFT : ���{�^�� 
	
	//! ���̃A�j���[�V�����ԍ���ۑ�
	int m_next_anim = -1;
	//! �R���{����߂Ăق������̃t���O
	bool m_stop_combo_flag = false;


	//!  �Q�[���p�b�h�p�ړ��p�x�N�g���p�ϐ�
	Vector3 m_mov = { 0.0f,0.0f,0.0f };

public:
	//! �G�Ƃ̈ړ������蔻��Ɏg��
	float m_hit_r = 0.0f;

	//! �v���C���[�̌��ݍs���Ă���U���ԍ���ۑ�����(�����蔻��Ɏg�p����)
	int m_now_attack = 0;

	//! HP�̎c��
	int m_hp_value = 0;
};