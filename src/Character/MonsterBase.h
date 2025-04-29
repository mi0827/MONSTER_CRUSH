#pragma once



// �����X�^�[��p�̃L�����x�[�X�N���X

//! 
//! @fule MonsterBase.h
//! @brief �����X�^�[�̃x�[�X�N���X


class MonsterBase
{
public:
	//! �R���X�g���N�^
	MonsterBase();

	//! �f�X�g���N�^
	~MonsterBase();

	//! �������z�֐�
	//! @brief ��������
	//! //! @param �ړ��̍ۂ̃^�[�Q�b�g���W
	virtual void Init() = 0;
	//! @brief �`�揈��
	virtual void Draw() = 0;
	//! @brief �X�V����
	//! @param �ړ��̎��̃^�[�Q�b�g�̍��W
	//! @param �^�[�Q�b�g�̔��a
	//! @param �^�[�Q�b�g��body�̃J�v�Z���i�����蔻��j
	//! @param �J�����I�u�W�F�N�g
	virtual void Update(Transform* target_pos, float target_r, CapsuleCollision body, Camera* camera) = 0;

	//! @brief �����Ă鎞�̍X�V����
	//! @param �^�[�Q�b�g�̍��W
	//! @param �^�[�Q�b�g�̔��a
	//! @param �J�����I�u�W�F�N�g
	virtual void LiveUpdate(Transform* target_pos, float target_r, Camera* camera) = 0;

	//! @brief �o�ꉉ�o�p�̍X�V����
	virtual void EntryUpdate() = 0;

	//! @brief HP�����܂Ō������Ƃ��̃��x���A�b�v����
	virtual void ReinforceUpdate() = 0;

	//! @brief �U�����󂯂����̍X�V����
	virtual void ComeAttackUpdate() = 0;

	//! @brief ���񂾂Ƃ��̍X�V���� 
	virtual void DieUpdate() = 0;
	//! @brief �I������
	virtual void Exit() = 0;

	//! @brief �����蔻��̍X�V����
	virtual void CDUpdate() = 0;

	//! @brief �X�e�[�^�X�o�[�̐ݒ�p�֐�
	virtual void StatusBarInit() = 0;

	//! @brief �X�e�[�^�X�o�[�̕`��֐�
	virtual void StatusBarDraw() = 0;

	//! @brief �_���[�W���󂯂����̏���
	//virtual void DamageUpdate() = 0;

	//! @brief �A�j���[�V�����̎w��̃t���[���ňړ������鏈��
	//! @param �ړ��X�s�[�h
	//! @param �ړ��J�n�t���[��
	//! @param �ړ��I���t���[��
	void MoveBetween(float mov_speed, float mov_start_frame, float mov_end_frame);

	//! @brief ���[�����O�A�N�V�������Z�b�g����֐�
	//! @param ���[�����O�A�j���[�V�����ԍ�
	//! @param �^�[�Q�b�g���[�����O�A�N�V�������s��
	void SetRollingAction(int rolling_anim, int target_distance);

	//! @brief ���[�����O�A�N�V�����p�̊֐��i����j
	//! @param ���[�����O�����Ƃ��̈ړ��X�s�[�h
	void ActionRolling(const int rolling_speed, float rolling_start_frame, float rolling_end_frame);

	//! @brief �U���Ɋւ��邱�Ƃ̏����֐�
	//! �����蔻����s���Ăق����^�C�~���O��ۑ�����
	//! �R���{�p�^�[���̏�����
	//! �e�R���{�p�^�[���̐ݒ�
	virtual void SetAttackInfo() = 0;

	//! @brief �X�^���������p�̍X�V����
	//! @param �X�^�����̃_�E���A�j���[�V�����ԍ�
	//! @param �X�^�����̋N���オ��A�j���[�V�����ԍ�
	//! @param �X�^���l�̍ő��
	virtual void StunActionUpdate(int down_anim_num, int up_anim_num, int sutn_value_max);

	//! @brief �����X�^�[�̏��(�t���O)�Ǘ��֐�
	//! @param �����X�^�[�̏��
	virtual void MonsterMode(int mode) = 0;

	//! @brief �A�j���[�V�����ǂݍ��ݗp�֐�
	virtual void AnimLoadInit() = 0;

	//! @brief �G�t�F�N�g�̓ǂݍ��݂��܂Ƃ߂�֐�
	virtual void EffectLoadInit() = 0;

	//! @brief �G�t�F�N�g�̍X�V����
	//! @param �s�������G�t�F�N�g�ԍ�
	//! @param �s�������G�t�F�N�g�̏��ԍ�
	virtual void EffectUpdate( int effect_num, int effect_info_num) = 0;

	//! @brief �U�����󂯂����̃G�t�F�N�g�̍X�V����
	virtual void DamageEffectUpdate() = 0;

	//! @brief SE�̓ǂݍ���
	virtual void SELoadInit() = 0;

	//! @brief SE�̍X�V����
	//! @param �s������SE�ԍ�
	virtual void SEUpdate(int se_num) = 0;

	//! @brief �U���A�j���[�V�����Ɋւ�����̐ݒ�
	//! @param �U���A�j���[�V�����̊J�n�ԍ�
	//! @param �U���A�j���[�V�����̍ő吔
	//! @param �����_���ōU����I�Ԏ��̂͂Ԃ��Ăق����U���ԍ�
	void SetAttackAnimInfo(int attack_anim_start, int attack_anim_max, int except_attack);

	//! @brief �A�C�h���A�N�V��������
	//! @param �A�C�h���A�j���[�V�����ԍ�
	void IdleActionUpdate(int idle_anim_num);

	//! @brief �ړ��A�N�V��������
	//! @param �ړ��p�A�j���[�V�����ԍ�
	void MoveAction(int ran_anim);

	//! @brief �R���{�p�^�[���̐���ݒ�
	//! @param �R���{�p�^�[���̍ő吔
	void ComboPatternNumberInit(int pattern_max);

	//! @brief �e�R���{�p�^�[���̐ݒ�
	//! @param ���p�^�[���ڂ̃R���{��
	//! @param �R���{�̐��̍ő吔
	//! @param �R���{���I�������̂��ƌ��̃t���[����
	//! @param �R���{�p�̍U���ԍ����ۑ�����Ă���z������炤
	void ComboPatternInfoInit(int pattern_num, int combo_num_max, int rear_crevice_frame, int* anim_num);

	//! @brief �U���͈͂ɐN���������ǂ����𔻒f����
	//! @return true : �U���͈͂ɓ������Afalse : �U���͈͊O
	bool HitAttackArea();

	//! @brief �ŏ��̍U�����s���p�֐�
	void FirstAttackAction();

	//! @brief �U���֘A�̍X�V����
	void AttackActionComboUpdate();

	//! @brief ���̃_���[�W���󂯂����ɑ���Ƃ̋������Ƃ�U�����Z�b�g����(���K�U��)
	//! @param ���K�U���p�̃A�j���[�V�����ԍ�
	//! @param se�̔ԍ�
	//! @param �J�����V�F�C�N�̂��߂̃J����
	void RoarSet(int anim_num, int se_num, Camera* camera);

	//! @brief ���K�U�����̏���
	//! @param �J�����V�F�C�N�̂��߂̃J����
	void RoarAction(Camera* camera);

	//! @brief �W�����v�U���J�n�̏���
	//! @param �W�����v�p�̃A�j���[�V�����ԍ�
	//! @param ����Ɨ���ĂĂق�������
	void JumpAction(int jump_anim, int target_distance);

	//! @brief �W�����v�U���̍X�V����
	//! @param �W�����v���̈ړ��X�s�[�h
	//! @param �W�����v�ł̈ړ��J�n�t���[��
	//! @param �W�����v�ł̈ړ��I���t���[��
	void JumpActionUpdate(float jump_mov_speed, float jump_mov_strat_frame, float jump_mov_end_frame);

	//! @brief �R���{�U���X�V����
	void ComboUpdate();

	//! @brief �s�������U���A�j���[�V�����������_���őI�Ԃ��߂̊֐�
	int SetRandAttack();

	//! @brief �x�[�X�N���X�ł̏�������
	//! @param �����X�^�[��HP�}�b�N�X
	//! @param �W�����v�̏㏸�X�s�[�h
	//! @param �W�����v�̉��~�X�s�[�h
	void BaseInit(int hp_num);

	//! @brief �����X�^�[�̈ړ��Ɋւ���^�[�Q�b�g�̐ݒ�
	//! @param �^�[�Q�b�g�̍��W
	//! @param �^�[�Q�b�g�̃J�v�Z���̓����蔻��̔��a
	//! @param �^�[�Q�b�g��body�̃J�v�Z��
	void BaseSetTarget(Transform* target_pos, const float m_target_hit_r, CapsuleCollision body);

	//! @brief �ړ��̍X�V����
	//! @param  �����Ă����Ԃ��̃t���O
	void MoveUpdate(bool* run_flag);

	//! @brief �����蔻����s���Ăق����^�C�~���O���Z�b�g����֐�
	//! @param �����蔻����Ƃ��Ăق����t���[���̃X�^�[�g
	//! @param �����蔻����I����Ăق����t���[���̏I���
	//! @param �ۑ��������U���ԍ�
	void SetHitTime(int attack_frame_start, int attack_frame_end, int attack_num);

	// �����蔻����Ƃ��Ă悢�^�C�~���O���𔻒f����֐�
	//! @brief �U���ԍ�
	bool AttackHitGoodTiming(int anim_num);

	//! @brief �����蔻��ƃ_���[�W��ۑ�����֐�
	//! @param �����蔻��p�̃J�v�Z��
	//! @param �^����_���[�W
	//! @param �U���ԍ�
	void SetHitDamage(CapsuleCollision attack_hit, int attack_damage, int attack_num);

	//! @brief �U�����̓����蔻���ݒ肷��p�̊֐�
	//! @param �U���A�j���[�V�����̍ő吔
	void SetAttackHitDamage(int attack_anim_max);

public:

	//! �����X�^�[�̏��
	enum MonsterMode
	{
		IDLE,       //!< �ҋ@���
		RUN,       //!< ������
		ATTACK,  //!< �U�����
		STUN,      //!< �X�^�����
		ROAR,       //!< ���K���
		DIE,        //!< ����
	};
	//! �����X�^�[�̏�Ԃ��Ǘ�����ϐ�
	int m_monster_mode = 0;

	//-----------------------------------------------
	// �񋓑̂ŊǗ�
	//-----------------------------------------------
	//! �����X�^�[�̏�ԁi�����j
	enum Situation
	{
		alive, // �����Ă�Ƃ�
		die,   // ����ł�
	};
	//! �����Ă邩��ۑ����邽�߂̕ϐ�
	// �ŏ��͐����Ă��Ԃ���n�߂�
	int m_life_and_death = alive;

	//! �����X�^�[�̂Ɏg�p����t���O
	//! �A�C�h����Ԃ��̃t���O
	bool m_idle_flag = false;
	//! 1�t���[���O�̍U���t���O�̏�Ԃ�ۑ����Ă���
	bool m_past_attack_flag = m_attack_flag;
	//! �_���[�W���󂯂����p�̃t���O
	bool m_damage_anim_flag = false;
	//! �����蔻����Ƃ��Ă������̃t���O
	bool m_can_hit_damage_flag;
	//! �X�^����Ԃ��̃t���O
	bool m_stun_flag;



private:
	//! �U���͈͂̑傫��(��U�����X�^�[�S�����ʂɂ���)
	int M_ATTACK_HIT_RANGE = 10;

	//! �U���A�j���[�V�����̃X�^�[�g�ԍ�
	int M_ATTACK_ANIM_START = 0;
	//! �U���A�j���[�V�����̍ő吔
	int M_ATTACK_ANIM_MAX = 0;
	//! �����_���ōU����I�ԍۂɂ͂Ԃ��Ăق����A�j���[�V�����ԍ�
	int M_ATTACK_ANIM_EXCEPT = 0;




public:
	//------------------------------------------
	// �X�e�[�^�X�֘A
	//------------------------------------------
	// HP�̍ő�l
	int m_hp_max;
	// HP�̎c��
	int m_hp_value;
	// HP�p�̃o�[
	UIBra m_hp_bra;

	//------------------------------------------
	// Run�֘A
	//------------------------------------------
	//! �����Ă����ȉ��̃t���O
	bool m_run_flag = false;
	//! �ǎC�蔻��̂��߂ɂ���������W��ۑ����Ă����ϐ�
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };
	//! �ړ��̍ۂ̓����蔻��p�̃T�C�Y
	Vector3 move_hit_size = { 1.0f ,0.0f,1.0f };
	//! ���g�̔��a������p�̕ϐ�
	float m_hit_r = 15.0f;
	//! ���g�̈ړ��X�s�[�h
	static constexpr float M_MOV_SPEED = 1.0f;
	//! ���g��]���x
	static constexpr float M_ROT_SPEED = 5.0f;
	//! �ǂ̂��炢�̃t���[���̊Ԃ͑����Ă���̂����J�E���g���邽�߂̊֐�
	int m_running_frame_count = 0;

private:
	//------------------------------------------
	// �A�C�h���֘A
	//------------------------------------------
	//! �ǂꂾ���̃t���[���~�܂��ė~�������Z�b�g���������ۂ̃t���O
	bool m_set_stop_frame_flag = false;
	//! �A�C�h����ԂŎ~�܂��Ă��Ăق����t���[��
	int m_stop_frame = 0;
	//! ���t���[���~�܂��Ă���̂��̃J�E���g
	int m_stop_frame_count = 0;
public:

	//------------------------------------------
	// �W�����v�֘A
	//------------------------------------------
	//! �W�����v�̂��߂̈ړ��ʗp�̕ϐ�
	Vector3 m_jump_mov{ 0.0f,0.0f,0.0f };
	//! �W�����v�̒��n�n�_�̍��W
	Vector3 m_jump_pos{ 0.0f,0.0f,0.0f };
	//! �W�����v�̎��ɐi�ދ���
	float m_jump_move;
	//! �W�����v�t���O
	bool m_jump_flag = false;

	//------------------------------------------
	// ���[�����O�֘A
	//------------------------------------------
	//! ���[�����O�t���O
	bool m_rolling_flag;


	//------------------------------------------
	// �U���֘A
	//------------------------------------------
	// �U����Ԃ̊Ǘ�
	enum Attack
	{
		ATTACKSET,
		UNDERATTACK,
	};
	//! �U����Ԃ�ۑ�����ϐ�
	int m_attack_info_num = ATTACKSET;
	//! ���̃A�j���[�V�����ԍ���ۑ�����p�̕ϐ�
	int m_now_attack_anim = 0;
	//! �����X�^�[�̌��ݍs���Ă���U���A�j���[�V�����ԍ���ۑ�����
	int m_now_attack = -1;
	//! �R���{�����ڂ�
	int m_combo_num = 0;
	//! ���K�U���p�̃t���O
	bool m_roar_flag = false;
	//! ���K�U�������x������
	int m_roar_count = 3;
	//! �U����Ԃ��ǂ����̃t���O
	bool m_attack_flag = false;
	//! �����X�^�[�̑O�������̍U���G���A�i���̃G���A�ɓ�������U��������j
	CapsuleCollision m_attack_area;
	Vector3 m_attack_area_1;
	Vector3 m_attack_area_2;
	//! �ǂ̂��炢�O���������̒l
	static constexpr int ATTACK_AREA_DISTANCE = 15;
	//! �U���G���A�ɔ��a
	static constexpr int ATTACK_AREA_R = 14;

	//------------------------------------------
	// �U���̓����蔻��֘A
	//------------------------------------------
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

	};
	std::vector< Attack_Hit_Damage*> m_attack_hit_damage;

	//------------------------------------------
	// �R���{�֘A
	//------------------------------------------
	// �R���{�����p�^�[�����p�ӂ��Ă���������_���Ń����X�^�[�ɑI�΂���
	struct ComboPattern
	{
		// �R���{�����q���邩�̐�
		int m_combo_num_max;
		// �R���{�Ŏg���A�j���[�V�����ԍ���ۑ����邽�߂ɕϐ�
		// 
		std::vector<int> m_combo_parts;
		// �R���{�̌�̂��ƌ����t���[����
		int m_rear_crevice_frame;
	};
	std::vector<ComboPattern> m_combo_pattern;
	//! �R���{�̃p�^�[���̍ő吔��ۑ�����
	int m_combo_pattern_max = 0;
	//! ���p�^�[���ڂ̃R���{���g�p���邩
	int m_combo_pattern_num = 0;
	// ���ݎg�p���Ă���R���{�p�^�[���ۑ�
	int m_now_combo_pattern_num = 0;
	// ��L�ō���Ă���R���{�̃p�^�[�������ł���΂��̉��̃t���O�֘A�͂���Ȃ��Ȃ�
	// �R���{�̍ő吔
	static constexpr int COMBO_MAX = 3;
	//! �R���{�p�t���O
	bool m_combo_flag = false;

	//! ���̃A�j���[�V�����ԍ���ۑ�
	int m_next_anim = -1;
	//! �R���{����߂Ăق������̃t���O
	bool m_stop_combo_flag = false;


	//------------------------------------------
	// �X�^���֘A
	//------------------------------------------
	// �X�^���̏�Ԃ��Ǘ�
	enum Stun
	{
		DOWN,          //! �|��Ă���Ƃ�
		UPSTANDBY,  //! �|��Ă���N���オ��܂ł̊�
		UP,                //! �N���オ��Ƃ�
	};
	//! �X�^���̏�Ԃ�ۑ�����ϐ�
	int m_stun_info_num = DOWN;
	//! �X�^���l�̎c��
	int m_stun_value;

	//! �X�^���l�̉񕜗�
	static constexpr int RECOVERY_STUN_VALUE = 1;
	//! ���t���[���U�����󂯂Ă��Ȃ���΃X�^���l���񕜂��邩
	static constexpr int STUN_VALUE_RECOVERY_FRAME = 60;
	// �X�^���p�̃o�[
	UIBra m_stun_bra;
protected:
	//! �U�����󂯂Ă��Ȃ��t���[�����J�E���g
	int m_not_damaged_frame = 0;
public:
	//-----------------------------------------------
	// �N���X�̃I�u�W�F�N�g�̐錾
	//-----------------------------------------------
	//! ���g�̏�������ϐ�
	Transform m_transform;
	//! �ړ��Ɏg���N���X
	TargetMove m_move;
	//! ���f���N���X�̃I�u�W�F�N�g
	Model m_model;
	//! �A�j���[�V�����N���X�̃I�u�W�F�N�g
	Animation m_animation;
	//! �R���{�N���X
	Combo m_combo;
	//! �G�t�F�N�g�N���X�I�u�W�F�N�g
	Effect m_effect;
	//! �U�����󂯂��Ƃ��悤�̃G�t�F�N�g�N���X�I�u�W�F�N�g
	Effect m_damage_effect;
	//! �T�E���h�N���X�̃I�u�W�F�N�g
	Sound m_se;

	//=================
	// �����蔻��
	//=================
	//! �J�v�Z���R���W����
	CapsuleCollision m_left_hand; //!< ����̂����蔻��
	CapsuleCollision m_right_hand; //!< �E��̓����蔻��
	CapsuleCollision m_body;       //!< �{�̂̂����蔻��

};
