


//! @brief �����X�^�[�̃N���X
//! @brief �����X�^�[�x�[�X�N���X���p��
#pragma once

//! @file Monster.h
//! @brief �����X�^�[�̃N���X
//! @brief �����X�^�[�x�[�X�N���X���p��
class Monster :public MonsterBase
{
public:

	//=========================================================================
	// �֐��̐錾
	//=========================================================================
	//! @brief �R���X�g���N�^
	Monster();
	//! @brief �f�X�g���N�^
	~Monster();

	//! �p���悩��̊֐��̃I�[�o�[���C�h
	//! @brief ��������
	void Init()override;
	//! @brief �X�V����
	//! @param �ړ��̎��̃^�[�Q�b�g�̍��W
	//! @param �^�[�Q�b�g�̔��a
	//! @param �^�[�Q�b�g��body�̃J�v�Z���i�����蔻��j
	//! @param �J�����I�u�W�F�N�g
	void Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera) override;

	//! @brief �����Ă鎞�̍X�V����
	//! @param �^�[�Q�b�g�̍��W
	//! @param �^�[�Q�b�g�̔��a
	//! @param �J�����I�u�W�F�N�g
	void LiveUpdate(Transform* target_pos, float target_r, Camera* camera) override;

	//! @brief ���񂾂Ƃ��̍X�V���� 
	void DieUpdate() override;
	//! @brief �`�揈��
	void Draw() override;
	//! @brief �I������
	void Exit() override;

	//! @brief �o�ꉉ�o�p�̍X�V����
	void EntryUpdate() override;

	//! @brief HP�����܂Ō������Ƃ��̃��x���A�b�v����
	void ReinforceUpdate() override;

	//! @brief �U�����󂯂����̍X�V����
	void ComeAttackUpdate() override;

	//! @brief �����蔻��̍X�V����
	void CDUpdate() override;

	//! @brief �X�e�[�^�X�o�[�̐ݒ�p�֐�
	void StatusBarInit() override;
	//! @brief �X�e�[�^�X�o�[�̕`��֐�
	void StatusBarDraw() override;

	//! @brief �����蔻����s���ė~�����^�C�~���O��ۑ�����֐�
	void SetAttackInfo() override;

	//! @brief �����X�^�[�̏��(�t���O)�Ǘ��֐�
	//! @param �����X�^�[�̏��
	void MonsterMode(int mode) override;

	//! @brief �A�j���[�V�����ǂݍ��݊֐�
	void AnimLoadInit() override;

	//! @brief �G�t�F�N�g�̓ǂݍ��݂��܂Ƃ߂�֐�
	void EffectLoadInit() override;
	//! @brief �G�t�F�N�g�̍X�V����
	//! @param �s�������G�t�F�N�g�ԍ�
	//! @param �s�������G�t�F�N�g�̏��ԍ�
	void EffectUpdate(int effect_num, int effect_info_num) override;
	//! @brief SE�̓ǂݍ���
	void SELoadInit() override;
	//! @brief SE�̍X�V����
	//! @param �s������SE�̏�񂪕ۊǂ���Ă���ԍ�
	void SEUpdate(int se_num) override;

	//=========================================================================
	// �萔�̐錾
	//=========================================================================
	static constexpr float MONSTER_MOVE_SPEED = 1.5f;        //! �ړ��X�s�[�h
	static constexpr float MONSTER_ROT_SPEED = 0.5f;         //! ��]�X�s�[�h

	static constexpr float CHANGE_JUMP_RUNNIG_FRAME = 150;   //! Run����W�����v,���[�����O�Ɉڂ�̂ɉ��t���[�������邩

	static constexpr float JUMP_TARGET_DISTANCE = 25.0f;     //! �W�����v����Ƃ�����Ɨ���ĂĂق����Œ���̋���
	static constexpr float JUMP_MOV_SPEED = 1.0f;            //! �W�����v���̈ړ��X�s�[�h
	static constexpr float JUMP_DEVIATION_POS = 26.0f;       //! �W�����v�U���������Ƃ��ɃY���𒼂��l
	static constexpr float JUMP_STRAT_FRAME = 34.0f;         //! �W�����v�̈ړ��J�n�̃t���[��
	static constexpr float JUMP_END_FRAME = 90.0f;           //! �W�����v�̈ړ��I���̃t���[��

	static constexpr int ROLLING_SPEED = 1;             //! ���[�����O�X�s�[�h
	static constexpr float ROLLING_STRAT_FRAME = 0.0f;       //! ���[�����O�̈ړ��J�n�t���[��
	static constexpr float ROLLING_END_FRAME = 70.0f;        //! ���[�����O�̈ړ��I���t���[��
	static constexpr float ROLLING_TARGET_DISTANCE = 15.0f;  //! ���[�����O����Ƃ�����Ɨ���ĂĂق����Œ���̋���

	static constexpr int HP_VALUE_MAX = 500;                 //! HP�̍ő�l
	static constexpr int STUN_VALUE_MAX = 150;               //! �X�^���l�̍ő�

	//=========================================================================
	// �񋓑̂̐錾
	//=========================================================================
	//! �A�j���[�V�����̎�ޗp�̗񋓑�
	//! @detail �U���A�j���[�V�����̓R���{�̑����������Ԃɐݒ肷��
	enum Animation_Max
	{
		idle_anim, //!< �ҋ@
		run_anim,  //!< ����
		die_anim,   //!< ���S
		shout_anim, //!< ����
		hit_damage_anim, //!< �U�����󂯂���
		stun_down_anim, //!< �X�^���œ|��鎞
		stun_up_anim,  //!< �X�^���ŋN���オ��Ƃ�

		punch_attack_1_anim, //!< �p���`�U���P
		sword_attack_1_anim, //!< �\�[�h�U���P
		sword_attack_2_anim, //!< �\�[�h�U���Q
		sword_attack_3_anim, //!< �\�[�h�U���R
		sword_attack_4_anim, //!< �\�[�h�U���S
		sword_attack_5_anim, //!< �\�[�h�U���T
		rolling_anim,  //!< ���[�����O
		jump_anim,     //!< �W�����v�A�N�V����

		anim_max //!< �A�j���[�V�����̍ő吔
	};

	//! �U���A�j���[�V�����̈�ԍŏ�
	static constexpr int ATTACK_ANIM_START = punch_attack_1_anim;
	//! �U���A�j���[�V�����ő�l�ijump�𔲂������W�����v�𔲂��Ă����Ȃ��ƃR���{�U���̎��ȃo�O��j
	static constexpr int ATTACK_ANIM_MAX = rolling_anim - ATTACK_ANIM_START;

	//! �U���A�N�V�����̐�
	static constexpr int ATTACK_ACTION_MAX = anim_max - punch_attack_1_anim;

	// �U���ԍ��̍Đݒ�
	enum AttackAnim
	{
		attack_end = -1, //< �R���{�U���̏I���
		attack_punch_1 = punch_attack_1_anim - ATTACK_ANIM_START,  //< �U���P
		attack_sword_1 = sword_attack_1_anim - ATTACK_ANIM_START,  //< �U���Q
		attack_sword_2 = sword_attack_2_anim - ATTACK_ANIM_START,  //< �U���R
		attack_sword_3 = sword_attack_3_anim - ATTACK_ANIM_START,  //< �U���S
		attack_sword_4 = sword_attack_4_anim - ATTACK_ANIM_START,  //< �U���T
		attack_sword_5 = sword_attack_5_anim - ATTACK_ANIM_START,  //< �U���U
		attack_rolling = rolling_anim - ATTACK_ANIM_START,   //< ���[�����O�U��
		attack_jump = jump_anim - ATTACK_ANIM_START,         //< �W�����v�U��

		attack_max
	};

	//! �R���{�p�^�[���̐�
	static const int M_COMBO_PATTERN_MAX = 6;
	//! �e�p�^�[���̃R���{�̍ő吔
	static const int M_COMBO_NUM_MAX = 4;
	//! �e�R���{�p�^�[���ɃR���{�Ɏg�p����U���ԍ���ۑ����R���{���쐬����
	//! �R���{�̍Ō�ɂ�attack_end�����邱��
	int m_combo_pattern[M_COMBO_PATTERN_MAX][M_COMBO_NUM_MAX]
	{
	  {attack_sword_3,attack_sword_1,attack_end,attack_end},
	  {attack_punch_1,attack_sword_5,attack_end,attack_end},
	  {attack_sword_3,attack_sword_2,attack_end,attack_end},
	  {attack_sword_4,attack_sword_1,attack_punch_1,attack_end},
	  {attack_sword_5,attack_punch_1,attack_sword_3,attack_end},
	  {attack_punch_1,attack_sword_3,attack_sword_2,attack_end},
	};
	// �e�R���{�̌㌄
	int m_combo_rear_crevice_frame[M_COMBO_PATTERN_MAX]
	{
		{30},{30},{20},{20},{20},{15}
	};

	// �����蔻����Ƃ��Ăق����t���[���̍\����
	struct AttackFrame
	{
		// �n�܂�̃t���[��
		float start_frame;
		// �I���̃t���[��
		float end_frame;
	};

	// �����蔻����s���Ăق����^�C�~���O�̐ݒ�̕ϐ�
	// �܂��܂�������
	// ��i�W�܂łɊ���������
	AttackFrame attack_frame[attack_max] =
	{
		// �p���`�P
		{ 13.0f, 30.0f, },
		// �\�[�h�P
		{ 40.0f, 90.0f, },
		// �\�[�h�Q
		{ 30.0f, 90.0f, },
		// �\�[�h�R
		{ 25.0f, 80.0f, },
		// �\�[�h�S
		{10.0f,30.0f},
		// �\�[�h�T
		{45.0f,75.0f},
		// ���[�����O
		{ 10.0f, 50.0f, },
		// �W�����v
		{ 15.0f, 105.0f, },

	};

	//! �e�U���̃_���[�W
	int m_attack_damage[attack_max]
	{
		20,70,50,40,20,60,50,50,
	};

	//! �G�t�F�N�g�̎�ޗp�̗񋓑�
	enum Effect
	{
		sword_attack_effect, // ���ł̍U�����̃G�t�F�N�g
		punch_attack_effect, // �p���`�U�����̃G�t�F�N�g
		damage_effect, // �_���[�W���󂯂����̃G�t�F�N�g
		roar_effect,      // ���K���̃G�t�F�N�g
		effect_max
	};

	// �G�t�F�N�g���������A�j���[�V�����̎�ނ�񋓑̂ŊǗ�
	enum EffectInfoNum
	{
		// �U���ԍ��ƍ��킹��������U���ɍ����悤�ɍU������ݒ�
		attack_punch_1_effect_info, // �p���`�U���P
		attack_sword_1_effect_info, // �\�[�h�U���P
		attack_sowrd_2_effect_info, // �\�[�h�U���Q
		attack_sowrd_3_effect_info, // �\�[�h�U���R
		attack_sowrd_4_effect_info, // �\�[�h�U���S
		attack_sowrd_5_effect_info, // �\�[�h�U���T

		// ��������͍U���Ƃ͕ʂ̃G�t�F�N�g
		damage_effect_info,     // �U�����󂯂����̃G�t�F�N�g
		roar_effect_info,       // ���K���̃G�t�F�N�g  

		effect_info_max
	};
	struct EffectInfo
	{
		// �G�t�F�N�g�̃X�P�[��
		Vector3 size;
		// �G�t�F�N�g�̍��W(�L�����̍��W���炸�炷���̍��W)
		Vector3 pos;
		// �G�t�F�N�g�̌���
		Vector3 rot;

		// ���̓�͂Ȃ��Ă���������
		// �G�t�F�N�g�̍Đ����J�n���������Ƃ��̃A�j���[�V�����t���[��
		float effect_start_anim_frame;
	};
	EffectInfo m_effect_info[effect_info_max]
	{
		// �p���`�U���P
		{ {4.0f,4.0f,4.0f},{15.0f,14.0f,15.0f},{90.0f,180.0f,0.0f}, 5},
		// �\�[�h�U���P
		{ {6.0f,6.0f,6.0f},{20.0f,18.0f,20.0f},{35.0f,-90.0f,0.0f}, 60},
		// �\�[�h�U���Q
		{ {6.0f,6.0f,6.0f},{20.0f,25.0f,20.0f},{155.0f,-90.0f,0.0f}, 40},
		// �\�[�h�U���R
		{ {6.0f,6.0f,6.0f},{15.0f,20.0f,15.0f},{35.0f,-90.0f,0.0f}, 30},
		// �\�[�h�U���S
		{ {5.0f,5.0f,5.0f},{15.0f,24.0f,15.0f},{85.0f,-90.0f,0.0f}, 1},
		// �\�[�h�U���T
		{ {6.0f,6.0f,6.0f},{15.0f,20.0f,15.0f},{90.0f,-90.0f,10.0f}, 30},
		// �_���[�W���󂯂����̃G�t�F�N�g
		{ {1.0f,1.0f,1.0f},{0.0f,15.0f,0.0f},{0.0f,0.0f,0.0f}, 1},
		// ���K���̃G�t�F�N�g  
		{ {1.0f,1.0f,1.0f},{5.0f,12.0f,5.0f},{0.0f,0.0f,0.0f}, 1},

	};


	// SE�̎�ޗp�̗񋓑�
	enum SE
	{
		punch_attack_se,    // �p���`�U��
		sword_attack_se_1, // ���U���P
		sword_attack_se_2, // ���U���Q
		damage_se,            // �_���[�W���󂯂���
		roar_se,                  // ���K��
		run_se,                   // ����
		se_max
	};

	// SE�������������̂̏��̗񋓑�
	enum SEInfoNum
	{
		// �U���ԍ��ƍ��킹��������U���ɍ����悤�ɍU������ݒ�
		attack_punch_1_se_info, // �p���`�U���P
		attack_sword_1_se_info, // �\�[�h�U���P
		attack_sowrd_2_se_info, // �\�[�h�U���Q
		attack_sowrd_3_se_info, // �\�[�h�U���R
		attack_sowrd_4_se_info, // �\�[�h�U���S
		attack_sowrd_5_se_info, // �\�[�h�U���T

		damage_se_info,     // �U�����󂯂���
		roar_se_info,       // ���K��  
		run_se_info,        // ����
		se_info_max
	};

	// �eSE�̏��
	struct SEInfo
	{
		// �Đ�������SE�ԍ�
		int se_num;
		// �Đ�����^�C�v
		int play_type;
		// ���[�v���邩�ǂ���
		bool loop;
		// �Đ�����^�C�~���O
		int se_start_frame;
	};
	SEInfo m_se_info[se_info_max]
	{
		// �U���ԍ��ƍ��킹�����̂ōU���̃G�t�F�N�g��񂩂��ɒ��ׂ�
		// �L�b�N1
		{ punch_attack_se, DX_PLAYTYPE_BACK, true, 1},
		// ���U���P
		{ sword_attack_se_1, DX_PLAYTYPE_BACK, true, 10},
		// ���U���Q
		{ sword_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// ���U���R
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// ���U��4
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// ���U��5
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},


		// �U�����󂯂���
		{damage_se,DX_PLAYTYPE_BACK, true, 1},
		// ���K
		{roar_se,DX_PLAYTYPE_BACK, true, 1},
		// ����
		{run_se, DX_PLAYTYPE_LOOP, true, 1},
	};
};

//class Monster :public MonsterBase
//{
//public:
//	//=========================================================================
//	// �萔�̐錾
//	//=========================================================================
//	static constexpr float MONSTER_MOVE_SPEED = 1.5f;   //! �ړ��X�s�[�h
//	static constexpr float MONSTER_ROT_SPEED = 0.5f;    //! ��]�X�s�[�h
//	static constexpr float TARGET_DISTANCE = 170.0f;    //! �^�[�Q�b�g�Ƃ̋���
//	static constexpr float JUMP_UP_SPEED = 5.0f;        //! �W�����v�̎��̏㏸�X�s�[�h
//	static constexpr float JUMP_DOWN_SPEED = 2.0f;      //! �W�����v�̎��̉��ɗ����Ăق����X�s�[�h 
//	static constexpr int HP_MAX = 500;                  //! HP�̍ő�l
//	//=========================================================================
//	// �񋓑̂̐錾
//	//=========================================================================
//public:
//	//! �A�j���[�V�����̎�ޗp�̗񋓑�
//	//! @detail �U���A�j���[�V�����̓R���{�̑����������Ԃɐݒ肷��
//	enum Animation_Max
//	{
//		idle, //!< �ҋ@
//		run,  //!< ����
//		attack_1, //!< �U���P
//		rolling,  //!< ���[�����O
//		jump,     //!< �W�����v�A�N�V����
//
//		anim_max //!< �A�j���[�V�����̍ő吔
//	};
//
//	//! �U���A�j���[�V�����̈�ԍŏ�
//	static constexpr int ATTACK_ANIM_START = attack_1;
//	//! �U���A�j���[�V�����ő�l
//	static constexpr int ATTACK_ANIM_MAX = jump - ATTACK_ANIM_START;
//
//
//	//! �����X�^�[�̏��
//	enum MonsterMode
//	{
//		IDLE,      //!< �ҋ@
//		RUN,       //!< ������
//		ATTACK,    //!< �U��
//	};
//
//	enum Jump
//	{
//		STANDBY,     //! �X�^���o�C
//		GOUP,           //! �オ��^�C�~���O
//		MOVE,           //! �W�����v���̈ړ��̃^�C�~���O
//		DROPDOWN,  //! �����Ă���^�C�~���O
//
//		max
//	};
//	//! �W�����v�̏�Ԃ�ۑ�����ϐ�
//	int jump_num = 0;
//public:
//
//	//=========================================================================
//	// �֐��̐錾
//	//=========================================================================
//	
//	//! @brief �R���X�g���N�^
//	Monster();
//	//! @brief �f�X�g���N�^
//	~Monster();
//
//	//! �p���悩��̊֐��̃I�[�o�[���C�h
//	//! @brief ��������
//	void Init()override;
//	//! @brief �X�V����
//	//! @param �ړ��̎��̃^�[�Q�b�g�̍��W
//	//! @param �^�[�Q�b�g�̔��a
//	//! @param �J�����I�u�W�F�N�g
//	void Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera) override;
//
//	//! @brief �`�揈��
//	void Draw() override;
//	//! @brief �I������
//	void Exit() override;
//
//
//	//! @brief �����蔻��̍X�V����
//	void CDUpdate() override;
//
//	//! @brief �X�e�[�^�X�o�[�̐ݒ�p�֐�
//	void StatusBarInit() override;
//	//! @brief �X�e�[�^�X�o�[�̕`��֐�
//	void StatusBarDraw() override;
//
//	//! @brief �A�j���[�V�����ǂݍ��݊֐�
//	void Anima_Load_Init();
//
//	//! @brief �v���C���[�̈ړ��p�֐�
//	void Move_Update();
//
//	//! @brief �U���J�n�������łȂ����𔻒f����֐�
//	void Attack_First();
//
//	//! @brief �U���p�̊֐�
//	void Attack_Update();
//
//	//! @brief �W�����v�U���p�̊֐�
//	void Attack_Jump();
//
//	//! @brief �W�����v�U���̍X�V����
//	void Jump_Update();
//
//	//! @brief �R���{�֐�
//	void Combo_Update();
//
//	//! @drief �s�������A�j���[�V�����������_���őI�Ԃ��߂̊֐�
//	int Set_Rand_Attack();
//
//public:
//	//-----------------------------------------------
//	// �ϐ��̐錾
//	//-----------------------------------------------
//	//! �A�C�h����Ԃ��̃t���O
//	bool m_idle_flag = false;
//	//! �����Ă����ȉ��̃t���O
//	bool m_run_flag = false;
//
//	//! �U����Ԃ��ǂ����̃t���O
//	bool m_attack_flag = false;
//	//! �v���C���[�̃��[�h���Ǘ�����ϐ�
//	int m_monster_mode = -1;
//	//! ���̃A�j���[�V�����ԍ���ۑ�����p�̕ϐ�
//	int m_now_attack_anim = -1;
//
//
//	//! �ǎC�蔻��̂��߂ɂ���������W��ۑ����Ă����ϐ�
//	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
//	//! �ړ��̍ۂ̓����蔻��p�̃T�C�Y
//	Vector3 move_hit_size = { 1.0f ,0.0f,1.0f };
//
//	//------------------------------------------
//	// �R���{�֘A
//	//------------------------------------------
//	// �R���{�̍ő吔
//	static constexpr int COMBO_MAX = 3;
//	//! �R���{�p�t���O
//	bool m_combo_flag = false;
//
//	//! �����Ăق������̃t���O
//
//	//! ���̃A�j���[�V�����ԍ���ۑ�
//	int m_next_anim = -1;
//	//! �R���{����߂Ăق������̃t���O
//	bool m_stop_combo_flag = false;
//
//
//
//public:
//
//
//
//	//=================
//	// �o�[�^��UI
//	//=================
//	UIBra m_hp;
//	// HP�̎c��
//	int m_hp_value = HP_MAX;
//};
