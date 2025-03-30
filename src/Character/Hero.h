#pragma once

//! @file HeroPlayer.h
//! @brief �q�[���[�̃N���X
//! @brief �L�����N�^�[�x�[�X�N���X���p��

class Hero :public CharacterBase
{
public:

	//-----------------------------------------------
	// �֐��̐錾
	//-----------------------------------------------
	//! @brief �R���X�g���N�^
	Hero();
	//! @brief �f�X�g���N�^
	~Hero();

	//-----------------------------------------------
   // �֐��̐錾
   //-----------------------------------------------


	//! �p���悩��̊֐��̃I�[�o�[���C�h
	//! @brief ��������
	void Init()override;
	//! @brief �X�V����
	//! @param �J�����̌���
	void Update(Vector3* camera_rot) override;

	//! @brief �����Ă鎞�̍X�V����
	//! @param �J�����̌���
	void LiveUpdate(Vector3* camera_rot) override;

	//! @brief ���񂾂Ƃ��̍X�V���� 
	void DieUpdate() override;


	//! @brief �`�揈��
	void Draw() override;
	//! @brief �I������
	void Exit() override;

	//! @brief �����蔻��̍X�V����
	void CDUpdate() override;

	//! @brief �X�e�[�^�X�o�[�̐ݒ�p�֐�
	void StatusBarInit() override;
	//! @brief �X�e�[�^�X�o�[�̕`��֐�
	void StatusBarDraw() override;

	//! @brief �����蔻����s���ė~�����^�C�~���O��ۑ�����֐�
	void SetHitTimeInit() override;

	//! @brief �A�j���[�V�����ǂݍ��݊֐�
	//! �U���A�j���[�V�����̎n�܂�A�j���[�V�����ԍ�
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
	void SEUpdate(int se_info_num) override;

	//! @brief �v���C���[�̏��(�t���O)�Ǘ��֐�
	//! @param �v���C���[�̏��
	void PlayerMode(int mode) override;

	//! @brief �U�����󂯂����̍X�V����
	void ComeAttackUpdate() override;
public:

	//-----------------------------------------------
	// �萔�̐錾
	//-----------------------------------------------
	static constexpr float  PLAYER_MOVE_SPEED = 2.0f;     //! �ړ��X�s�[�h
	static constexpr float  HERO_R = 5.0f;                //! ��]�X�s�[�h
	static constexpr float PLAYER_ROLLING_SPEED = 1.1f;   //! �v���C���[�̃��[�����O�X�s�[�h 
	static constexpr int HP_MAX = 100;                    //! HP�̍ő�l
	static constexpr int PLAYER_HIT_R = 1;                //! �v���C���[�̉~�̓����蔼�a�̔��a
	//-----------------------------------------------
	// �񋓑̂̐錾
	//-----------------------------------------------
public:

	//! �A�j���[�V�����̎�ޗp�̗񋓑�
	//! @detail �U���A�j���[�V�����̓R���{�̑����������Ԃɐݒ肷��
	enum Animation_Max
	{
		idle, //!< �ҋ@
		run, //!< ����
		rolling, //! ���[�����O
		hit_damage, //! �_���[�W���󂯂���
		die, //! �|���ꂽ��

		// ���N���b�N�U��
		attack_sword_anim_1,      //< �\�[�h�U���P
		attack_sword_anim_2,      //< �\�[�h�U���Q
		attack_sword_anim_3,      //< �\�[�h�U���R

		attack_sword_anim_4, //< �\�[�h�U���S
		attack_kick_anim_1,   //< �L�b�N�U��1
		attack_kick_anim_2,   //< �L�b�N�U��1

		counter_anim,              // �J�E���^�[�U��

		anim_max //!< �A�j���[�V�����̍ő吔
	};

	static constexpr int ATTACK_ANIM_STAR = attack_sword_anim_1;                     //! �A�j���[�V�����̍U���n�ȊO�̃A�j���[�V�����̐�
	static constexpr int ATTACK_ACTION = anim_max - ATTACK_ANIM_STAR; //! �U���A�j���[�V�����̍ő吔

	// �U���ԍ��̍Đݒ�
	enum AttackAnim
	{
		attack_sword_1 = attack_sword_anim_1 - ATTACK_ANIM_STAR,	   //< �U���Q
		attack_sword_2 = attack_sword_anim_2 - ATTACK_ANIM_STAR,	   //< �U���R
		attack_sword_3 = attack_sword_anim_3 - ATTACK_ANIM_STAR,    //< �U���P
		attack_sword_4 = attack_sword_anim_4 - ATTACK_ANIM_STAR, //< �L�b�N�U���P
		attack_kick_1 = attack_kick_anim_1 - ATTACK_ANIM_STAR,	//< �L�b�N�U���Q
		attack_kick_2 = attack_kick_anim_2 - ATTACK_ANIM_STAR,	//< �L�b�N�U���R

		attack_counter = counter_anim - ATTACK_ANIM_STAR,       //< �J�E���^�[�U��

		attack_max
	};

	// �����蔻����Ƃ��Ăق����t���[���̍\����
	struct AttackTiming
	{
		// �n�܂�̃t���[��
		float start_frame;
		// �I���̃t���[��
		float end_frame;
		// �q�b�g�X�g�b�v���s���Ă������ǂ���
		bool can_hit_stop;
	};


	// �����蔻����s���Ăق����^�C�~���O�̐ݒ�
	// ���̂Ƃ��뉼
	// ��i�W�܂łɊ���������
	AttackTiming attack_frame[attack_max] =
	{
		// ���U���P
		{ 33.0f, 52.0f, false},
		// ���U���Q
		{ 48.0f, 62.0f, false},
		// ���U���R
		{	52.0f, 70.0f, true},
		// ���U��4
		{	32.0f, 44.0f, false},
		// �L�b�N1
		{	25.0f, 45.0f, false},
		// �L�b�N2
		{	37.0f, 60.0f, true},
		// �J�E���^�[�U��
		{   93.0f,  120.0f,true},
	};

	// �e�U���̃_���[�W
	int m_attack_damage[attack_max]
	{
		30,40,60,25,30,50,100,
	};

	//! �G�t�F�N�g�̎�ޗp�̗񋓑�
	enum Effect
	{
		attack_sword_effect, // �U�����̌��̍U���G�t�F�N�g
		attack_kick_effect,    // �L�b�N���̃G�t�F�N�g
		damage_effect,        // �U�����󂯂����̃G�t�F�N�g
		rolling_effect,           // ���[�����O�G�t�F�N�g
		effect_max
	};
	// �G�t�F�N�g���������A�j���[�V�����̎�ނ�񋓑̂ŊǗ�
	enum EffectInfoNum
	{
		// �U���ԍ��ƍ��킹��������U���ɍ����悤�ɍU������ݒ�
		attack_sword_1_effect_info, // ���U���P
		attack_sword_2_effect_info, // ���U���Q
		attack_sowrd_3_effect_info, // ���U���R
		attack_sowrd_4_effect_info, // ���U���S
		attack_kick_1_effect_info,    // �L�b�N�P
		attack_kick_2_effect_info,    // �L�b�N�Q
		attack_counter_effect_info,  // �J�E���^�[

		// ��������͍U���Ƃ͕ʂ̃G�t�F�N�g
		damage_effect_info,     // �U�����󂯂����̃G�t�F�N�g
		rolling_effect_info,       // ���[�����O���̃G�t�F�N�g  

		effect_info_max
	};
	// �G�t�F�N�g�ׂ̍����ݒ肢�̓��e(�U���֘A�̃A�j���[�V�����ɂ���G�t�F�N�g�p)
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

	// �G�t�F�N�g�̏��
	EffectInfo m_effect_info[effect_info_max] =
	{
		// �U���ԍ��ƍ��킹�����̂ōU���̃G�t�F�N�g��񂩂��ɒ��ׂ�
		// ���U���P
		{ {2.0f,2.0f,2.0f},{10.0f,10.0f,10.0f},{-40.0f,-90.0f,0.0f}, 15},
		// ���U���Q
		{ {2.5f,2.5f,2.5f},{8.0f,8.0f,8.0f},{90.0f,-90.0f,0.0f}, 30},
		// ���U���R
		{ {3.5f,3.5f,3.5f},{8.0f,8.0f,8.0f},{-225.0f,90.0f,160.0f}, 20},
		// ���U��4
		{ {2.0f,2.0f,2.0f},{8.0f,8.0f,8.0f},{225.0f,90.0f,160.0f}, 10},
		// �L�b�N1
		{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}, 1},
		// �L�b�N2
		{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}, 1},
		// �J�E���^�[�U��
		{ {3.0f,3.0f,3.0f},{8.0f,8.0f,8.0f},{195.0f,90.0f,160.0f}, 85},

		// �U�����󂯂���
		{{1.2f,1.2f,1.2f},{0.0f,10.0f,0.0f},{90.0f,-90.0f,90.0f}, 1},
		// ���[�����O
	    {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{90.0f,-180.0f,90.0f}, 1}
	};


	// SE�̎�ޗp�̗񋓑�
	enum SE
	{
		sword_attack_se_1, // ���U���P
		sword_attack_se_2, // ���U���Q
		kick_attack_se_1,    // �L�b�N�U���P
		kick_attack_se_2,    // �L�b�N�U���Q
		run_se,                   // ����
		rolling_se,               // ���[�����O
		damage_se,            // �U�����󂯂����̃T�E���h
		se_max
	};
	// SE�������������̂̏��̗񋓑�
	enum SEInfoNum
	{
		// �U���ԍ��ƍ��킹��������U���ɍ����悤�ɍU������ݒ�
		attack_sword_1_se_info, // ���U���P
		attack_sword_2_se_info, // ���U���Q
		attack_sowrd_3_se_info, // ���U���R
		attack_sowrd_4_se_info, // ���U���S
		attack_kick_1_se_info,    // �L�b�N�P
		attack_kick_2_se_info,    // �L�b�N�Q
		attack_counter_se_info,  // �J�E���^�[

		// ��������͍U���Ƃ͕ʂ̃G�t�F�N�g
		run_se_info,           // �����p
		rolling_se_info,       // ���[�����O���̃G�t�F�N�g  
		damage_se_info,     // �U�����󂯂����̃G�t�F�N�g

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
		// ���U���P
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 10},
		// ���U���Q
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// ���U���R
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// ���U��4
		{ sword_attack_se_2, DX_PLAYTYPE_BACK, true, 1},
		// �L�b�N1
		{ kick_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// �L�b�N2
		{ kick_attack_se_1, DX_PLAYTYPE_BACK, true, 1},
		// �J�E���^�[�U��
		{  sword_attack_se_1, DX_PLAYTYPE_BACK, true, 0},

		// ����
		{run_se, DX_PLAYTYPE_LOOP, true, 1},
		// ���[�����O
		{rolling_se, DX_PLAYTYPE_BACK, true, 1},
		// �U�����󂯂���
		{damage_se,DX_PLAYTYPE_BACK, true, 1},
	};
	//------------------------------------------
	// �R���{�֘A
	//------------------------------------------
	// �R���{�̍ő吔
	static constexpr int COMBO_MAX = 3;

	//=================
	// �o�[�^��UI
	//=================
	UIBra m_hp;

public:
	//-----------------------------------------------
	// �N���X�̃I�u�W�F�N�g�̐錾
	//-----------------------------------------------


	//! �J�v�Z���R���W����
	//=================
	// �����蔻��
	//=================
	CapsuleCollision m_sword;     //!< �\�[�h�̓����蔻��

};
