#pragma once
//! @file SceneBase.h
//! @brief �V�[���N���X�����̃x�[�X�N���X
class Scene_Base
{
public:
	//--------------------
	// �萔
	//--------------------
	static constexpr int SAMPLEPLAYER = 0; // �T���v���v���C���[�̔ԍ�
	static constexpr int MUTANT = 0;           // �~���[�^���g�̔ԍ�
	static constexpr int MONSTER = 1;         // �����X�^�[�̔ԍ�

	//! @brief �R���X�g���N�^
	Scene_Base();
	//! @brief �f�X�g���N�^
	~Scene_Base();

	//! @brief �ǂ̃L�����N�^�[���g������ݒ肷��֐�
	//! @param �ǂ̃v���C���[���g�����̔ԍ�
	//! @param �ǂ̃����X�^�[���g�����̔ԍ�
	void SetCharacter(int player_num = 0, int monster_num = 0);

	//! @brief �S�V�[���Ŏg�����̏�����
	void BaseInit();
	// @brief �S�V�[���Ŏg�����̂̕`��
	// @param �V�[���ԍ�
	// @param �`����W
	void BaseDraw(int scene_num, Vector2 draw_pos);

	//! @brief ��������
	virtual void Init() = 0;
	//! @brief �X�V����
	//! @param BGM�̃{�����[��
	//! @param SE�̃{�����[��
	virtual void Update() = 0;
	//! @brief �`�揈��
	virtual void Draw() = 0;
	//! @brief �I������
	virtual void Exit() = 0;
	//! @brief �t�B�[���h�Ƃ̓����蔻��
	virtual void HitField() = 0;

	//! @brief �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
	//! @param BGM�̉���
	//! @param SE�̉���
	//! @param �}�E�X���x
	virtual void OptionValuesReflect(int bgm, int se, int mouse) = 0;


	//! @brief ��̕`��
	void SkyDraw();

	//! @brief �V���h�[�}�b�v�p�̏������֐�
	void ShadowMapInit();

	//! @brief �w��ꏊ�ɂ̃V���h�[�}�b�v�͈̔͂̐ݒ�
	//! @param �V���h�[�}�b�v�n���h��
	//! @param �w��̍��W
	void SetShadowMapArea(int shadowMap_handle, Vector3 player_pos);

	//! @brief �`��Ɏg�p����V���h�E�}�b�v�̐ݒ�������i������g��Ȃ��ƃV���h�[�}�b�v���g���Ȃ��j
	void UseShadowMapSet();

	//! @brief �V���h�[�}�b�v�̍폜
	void ExitShadowMap();

	//! @brief �X�N���[�����W�ɕ�����`�悷�邽�߂̍��W��ݒ肷��֐�
	//! @param �ݒ肵�������W
	//! @param �ݒ肵�����W����ǂꂾ�����炵�������̒l(�f�t�H���g�ł��炳�Ȃ��悤�ɐݒ肵�Ă���)
	VECTOR DrawStringWorld(Vector3 pos, Vector3 shift_pos = { 0.0f,0.0f,0.0f });

	//! @brief ���ɍs�������V�[�����Z�b�g����֐�
	//! @param ���ɍs�������V�[���ԍ�
	void SetNextScene(int next_scene);


	//! @brief �t�F�[�h�A�E�g�����֐�
	void FadeOutUpdate();
	//! @brief �t�F�[�h�A�E�g����̃V�[���`�F���W�p�֐�
	//! @param ���̃V�[���ԍ�
	void FadeOutSceneChange(int next_scene);
	//! @brief �t�F�[�h�C������
	void FadeInUpdate();
	//! @brief �t�F�[�h�A�E�g��`�悷�鏈��
	void FadeDraw();

	//! @brief �v���C���[�̓ǂݍ��ݏ���
	//! @param �g�p�������L�����N�^�[�ԍ�
	//! @param hero : �����m�Abot�F�{�b�g��m
	void PlayerInit(int character_num);

	//! @brief �󂯂��N�G�X�g�ɂ���Đ키�����X�^�[�̔ԍ���Ԃ��p�̊֐�
	//! @return �󂯂��N�G�X�g�ԍ�
	int ReturnMonsterNum();

	//! @brief �N�G�X�g�Ŏ󂯂��Ă��ԍ��ɂ���ă����X�^�[��o�ꂳ���邽�߂̂���
    //! @param �󂯂��N�G�X�g�ԍ��i�����X�^�[�ԍ��j
	void SetMonster(int monster_num);

public:
	//! ��b���t���[�����̕ϊ��p�萔
	const int FLAME_MAX = 60;

	//! �V�[���̐؂�ւ����s���Ă������t���O (true�ɂȂ�����V�[���̐؂�ւ��\)
	bool m_scene_change_judge = false;

	//! �P�̐�p�̃V���h�[�}�b�v�p�ϐ��P
	int m_shadowMap_handle_1 = 0;
	//! �P�̐�p�̃V���h�[�}�b�v�p�ϐ��Q
	int m_shadowMap_handle_2 = 0;
	//! �V���h�[�}�b�v�p�̕ϐ�
	int m_shadowMap_handle = 0;

	// �V���h�[�}�b�v�̃��C�g�̊p�x
	float m_light_angle = 0;
	// ���C�g�̃x�N�g��
	VECTOR m_light_direction;


	//! �ǂ̃v���C���[���g�����̔ԍ���ۑ�����
	int m_player_num = 0;
	//! �ǂ̃����X�^�[���g�����̔ԍ���ۑ�����
	int m_monster_num = 0;

	//! �J�����̌�����ۑ�����p�̕ϐ�
	Vector3 m_camera_rot{ 0.0f,0.0f,0.0f };


	// ���ׂẴV�[���ԍ�
	enum Scene
	{
		Title, // �^�C�g���V�[��
		Story, // �X�g�[���[�V�[��
		QuestArea, // �N�G�X�g�󒍃V�[��
		Battle, // �o�g���V�[��
		End, // �G���h�V�[��

		scene_max
	};
	//! ���݂̃V�[��
	int m_now_scene = Title;

	//! ���ɍs�������V�[��
	int m_next_scene = -1;

	//! �e�V�[���ōs���鏇��
	enum SceneTurn
	{
		//FadeIn,
		Main,
		FadeOut
	};
	//! ���݂̃^�[��
	int m_turn = Main;



private:
	enum TextNum
	{
		menu_text,
		pad_menu_text,
		target_camera_text,
		pad_target_camera_text

	};
	// ���ׂẴV�[���Ŏg���e�L�X�g
	Text m_text;
	// �e�L�X�g�̐�
	static constexpr int TEXT_MAX = 4;

	//! �V�[���ł̃t���[���J�E���g
	int m_frame_count = 0;
	//! �t�F�[�h�A�E�g�E�C���^�C���J�E���g
	int m_fade_time = 0;
	//! �t�F�[�h�A�E�g�E�C���̊���
	int m_fade_ratio = 0;
	//! �ǂ̂��炢�̒l�ω�����̂��̒l
	int m_fade_value = 0;

	// ��̍��W�Ȃ�
	Transform m_sky_transform;
	// ��p�̃��f��
	Model m_sky_model;
public:
	// �t�B�[���h�I�u�W�F�N�g
	TitleField m_field_1;
	// �t�B�[���h�I�u�W�F�N�g
	Field m_field_2;
	// �v���C���[�̃I�u�W�F�N�g
	CharacterBase* m_player;
	enum CharacterNum
	{
		hero,
		bot,
	};
	// �L�����N�^�[�ԍ�
	int m_character_num = hero;


};