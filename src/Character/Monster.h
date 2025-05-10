


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

	//! @brief �U�����󂯂����̃G�t�F�N�g�̍X�V����
	void DamageEffectUpdate() override;

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
		idle_anim,       //!< �ҋ@
		run_anim,        //!< ����
		die_anim,        //!< ���S
		shout_anim,      //!< ����
		hit_damage_anim, //!< �U�����󂯂���
		stun_down_anim,  //!< �X�^���œ|��鎞
		stun_up_anim,    //!< �X�^���ŋN���オ��Ƃ�

		punch_attack_anim,          //!< �p���`�U��
		bigpunch_attack_anim,      //!< ��p���`�U��
		upperpunch_attack_anim,  //!< �A�b�p�[�U��
		kick_attack_anim,             //!< �L�b�N�U��
		tackle_attack_anim,          //!< �^�b�N���U��
		breath_attack_anim,         //!< �u���X�U��
		rolling_anim,                    //!< ���[�����O
		jump_anim,                      //!< �W�����v�A�N�V����

		anim_max //!< �A�j���[�V�����̍ő吔
	};

	//! �U���A�j���[�V�����̈�ԍŏ�
	static constexpr int ATTACK_ANIM_START = punch_attack_anim;
	//! �U���A�j���[�V�����ő�l�ijump�𔲂������W�����v�𔲂��Ă����Ȃ��ƃR���{�U���̎��ȃo�O��j
	static constexpr int ATTACK_ANIM_MAX = rolling_anim - ATTACK_ANIM_START;

	//! �U���A�N�V�����̐�
	static constexpr int ATTACK_ACTION_MAX = anim_max - punch_attack_anim;

	// �U���ԍ��̍Đݒ�
	enum AttackAnim
	{
		attack_end = -1,                                                //< �R���{�U���̏I���
		attack_punch = punch_attack_anim - ATTACK_ANIM_START,           //< �p���`�U��
		attack_bigpunch = bigpunch_attack_anim - ATTACK_ANIM_START,     //< ��p���`�U��
		attack_upperpunch = upperpunch_attack_anim - ATTACK_ANIM_START, //< �A�b�p�[�U��
		attack_kick = kick_attack_anim - ATTACK_ANIM_START,             //< �L�b�N�U��
		attack_takle = tackle_attack_anim - ATTACK_ANIM_START,           //< �^�b�N���U��
		attack_breath = breath_attack_anim - ATTACK_ANIM_START,         //< �u���X�U��
		attack_rolling = rolling_anim - ATTACK_ANIM_START,              //< ���[�����O�U��
		attack_jump = jump_anim - ATTACK_ANIM_START,                    //< �W�����v�U��

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
		//---------------------------------------------------------------------------------------
		//       �R���{�P�@       |          �R���{�Q        |         �R���{�R        | �R���{�I��
		//---------------------------------------------------------------------------------------
		  {attack_punch,             attack_breath,           attack_end,           attack_end},
		  {attack_bigpunch,        attack_breath,           attack_end,           attack_end},
		  {attack_kick,                attack_bigpunch,       attack_end,           attack_end},
		  {attack_upperpunch,     attack_punch,           attack_end,           attack_end},
		  {attack_punch,             attack_kick,              attack_end,            attack_end},
		  {attack_breath,            attack_end,               attack_end,           attack_end},
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

	// �U���̓����蔻����s���Ăق����^�C�~���O�̐ݒ�̕ϐ�
	AttackFrame attack_frame[attack_max] =
	{
		// �p���`
		{ 50.0f, 70.0f, },
		// ��p���`
		{ 68.0f, 100.0f, },
		// �A�b�p�[
		{ 50.0f, 95.0f, },
		// �L�b�N
		{ 25.0f, 50.0f, },
		// �^�b�N��
		{ 44.0f, 86.0f},
		// �u���X
		{ 100.0f,240.0f},
		// ���[�����O
		{ 10.0f, 50.0f, },
		// �W�����v
		{ 15.0f, 105.0f, },

	};

	//! �e�U���̃_���[�W
	int m_attack_damage[attack_max]
	{
		20, 30, 30, 10, 10, 50, 20, 20,
	};

	//! �G�t�F�N�g�̎�ޗp�̗񋓑�
	//enum Effect
	//{
	//	punch_attack_effect,           // �p���`�U�����̃G�t�F�N�g
	//	big_punch_attack_effect,       // ��p���`�U�����̃G�t�F�N�g
	//	breath_attack_effect,          // �u���X�U���̃G�t�F�N�g
	//	damage_effect,                 // �_���[�W���󂯂����̃G�t�F�N�g
	//	effect_max
	//};
	//// �G�t�F�N�g���������A�j���[�V�����̎�ނ�񋓑̂ŊǗ�
	//enum EffectInfoNum
	//{
	//	// �U���ԍ��ƍ��킹��������U���ɍ����悤�ɍU������ݒ�
	//	attack_punch_effect_info,       // �p���`�U��
	//	attack_big_punch_effect_info,   // ��p���`�U��
	//	attack_upper_punch_effect_info, // �A�b�p�[�U��
	//	attack_kick_effect_info,        // �L�b�N�U��
	//	attack_tackle_effect_info,      // �^�b�N���U��
	//	attack_breath_effect_info,      // �u���X�U��
	//	// ��������͍U���Ƃ͕ʂ̃G�t�F�N�g
	//	damage_effect_info,     // �U�����󂯂����̃G�t�F�N�g
	//	roar_effect_info,       // ���K���̃G�t�F�N�g  
	//	effect_info_max
	//};
	
	//! �G�t�F�N�g�̎�ޗp�̗񋓑�
	enum Effect
	{
		punch_attack_effect,           // �p���`�U�����̃G�t�F�N�g
		big_punch_attack_effect,       // ��p���`�U�����̃G�t�F�N�g
		breath_attack_effect,          // �u���X�U���̃G�t�F�N�g

		effect_max
	};

	// �G�t�F�N�g���������A�j���[�V�����̎�ނ�񋓑̂ŊǗ�
	enum EffectInfoNum
	{
		// �U���ԍ��ƍ��킹��������U���ɍ����悤�ɍU������ݒ�
		attack_punch_effect_info,       // �p���`�U��
		attack_big_punch_effect_info,   // ��p���`�U��
		attack_upper_punch_effect_info, // �A�b�p�[�U��
		attack_kick_effect_info,        // �L�b�N�U��
		attack_tackle_effect_info,      // �^�b�N���U��
		attack_breath_effect_info,      // �u���X�U��

		// ��������͍U���Ƃ͕ʂ̃G�t�F�N�g
		roar_effect_info,       // ���K���̃G�t�F�N�g  

		effect_info_max
	};
	//! �G�t�F�N�g�̎�ޗp�̗񋓑�
	enum DamageEffect
	{
		damage_effect,                 // �_���[�W���󂯂����̃G�t�F�N�g
		damage_effect_max
	};



	struct EffectInfo
	{
		// �G�t�F�N�g�̃X�P�[��
		Vector3 size;
		// ���f���̃m�[�h�ɃG�t�F�N�g��ݒ肵�����ꍇ�̃m�[�h(�C�]�Ȃ�������-1������)
		int nodo_index;
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
		// �p���`�U��
		{ {4.0f,4.0f,4.0f}, 9, {20.0f,5.0f,20.0f},{90.0f,180.0f,0.0f}, 30},
		// ��p���`�U��
		{ {8.0f,8.0f,8.0f}, 9, {20.0f,5.0f,20.0f},{35.0f,-90.0f,0.0f}, 50},
		// �A�b�p�[�U��
		{ {4.0f,4.0f,4.0f}, 9, {10.0f,5.0f,10.0f},{0.0f, 0.0f, 0.0f}, 30},
		// �L�b�N�U��
		{ {6.0f,6.0f,6.0f}, 9, {15.0f,20.0f,15.0f},{35.0f,-90.0f,0.0f}, 30},
		// �^�b�N���U��
		{ {5.0f,5.0f,5.0f}, 9, {15.0f,24.0f,15.0f},{85.0f,-90.0f,0.0f}, 1},
		// �u���X�U��
		{ {8.0f,8.0f,8.0f}, 9, {30.0f,15.0f,30.0f},{0.0f,-90.0f,0.0f}, 100},
		
		// ���K���̃G�t�F�N�g  
		{ {1.0f,1.0f,1.0f}, 9, {5.0f,12.0f,5.0f},{0.0f,0.0f,0.0f}, 1}
	};
	// �_���[�W���󂯂����̃G�t�F�N�g
	EffectInfo m_damage_effect_info = { {1.0f,1.0f,1.0f}, -1, {0.0f,15.0f,0.0f},{0.0f,0.0f,0.0f}, 1 };

	// SE�̎�ޗp�̗񋓑�
	enum SE
	{
		punch_attack_se_1,  // �p���`�U��
		big_punch_attack_se, // ��p���`�U���Q
		breath_attack_se,    // �u�E���U��
		damage_se,            // �_���[�W���󂯂���
		roar_se,                  // ���K��
		run_se,                   // ����
		se_max
	};

	// SE�������������̂̏��̗񋓑�
	enum SEInfoNum
	{
		// �U���ԍ��ƍ��킹��������U���ɍ����悤�ɍU������ݒ�
		attack_punch_se_info, // �p���`�U��
		attack_big_punch_se_info, // ��p���`�U��
		attack_upper_punch_se_info, // �A�b�p�[�U��
		attack_kick_se_info, // �L�b�N�U��
		attack_tackle_se_info, // �^�b�N���U��
		attack_breath_se_info, // �u���X�U��

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
		// �p���`�U��
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 40},
		// ��p���`�U��
		{ big_punch_attack_se, DX_PLAYTYPE_BACK, true, 60},
		// �A�b�p�[�U��
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 40},
		// �L�b�N�U��
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 20},
		// �^�b�N���U��
		{ punch_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// �u���X�U��
		{ breath_attack_se, DX_PLAYTYPE_BACK, true, 1},


		// �U�����󂯂���
		{damage_se,DX_PLAYTYPE_BACK, true, 1},
		// ���K
		{roar_se,DX_PLAYTYPE_BACK, true, 1},
		// ����
		{run_se, DX_PLAYTYPE_LOOP, true, 1},
	};

	//=================
	// �����蔻��
	//=================
	CapsuleCollision m_left_feet; //!< ����̂����蔻��
	CapsuleCollision m_right_feet; //!< �E��̓����蔻��
	CapsuleCollision m_big_punch_hit; //!< ��p���`��p�̓����蔻��
	CapsuleCollision m_breath_hit; //!< �u���X�U����p�̓����蔻��
};

