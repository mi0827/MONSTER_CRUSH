#pragma once
//! @file QusetAreaScene.h
//! @brief �^�C�g���V�[���̃N���X
class QuestAreaScene : public Scene_Base
{
public:
	//! �R���X�g���N�^
	QuestAreaScene();
	//! �f�X�g���N�^
	~QuestAreaScene();

	//!  �b����悤�ɂȂ�͈͂̔��a
	static constexpr float AREA_FIELD_R = 15.0f;

	//! @brief ��������
	void Init()override;

	//! @brief �X�V����
	void Update()override;

	//! @brief �`�揈��
	void Draw()override;

	//! @brief �I������
	void Exit()override;

	//! @brief �t�B�[���h�ƃL�����N�^�[�̓����蔻��
	void HitField()override;

	//! @brief �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
	//! //! @param BGM�̉���
	//! @param SE�̉���
	//! @param �}�E�X���x
	void OptionValuesReflect(int bgm, int se, int mouse)override;

	//! @brief �b����悤�ɂȂ�G���A�̐ݒ�
	void InitArea();

	//! @brief ��t��̖ڈ�̍X�V����
	void LandMarkUpdate();

	//! @brief �N�G�X�g�G���A�̍X�V����
	void QuestAreaUpdate();

	//! @brief ��b���Ă��Ȃ���Ԃ̍X�V����
	void ModeNormalUpdate();

	//! @brief ��b�͂��߂̏���
	void TalkStart();

	//! @brief ��b�p�[�g�̍X�V����
	void TalkUpdate();

	//! @brief �N�G�X�g���󂯂Ă���Ƃ��̍X�V����
	void AcceptingQuestUpdate();

	//! @brief ��b���Ă��Ȃ���Ԃ̕`�揈��
	void ModeNormalDraw();

	//! @brief ��b�X�^�[�g���̕`�揈��
	void TalkStartDraw();

	//! @brief ��b�p�[�g�̕`�揈��
	void TalkDraw();

	//! @brief �N�G�X�g���󂯂Ă���Ƃ��̕`�揈��
	void AcceptingQuestDraw();

	//! @brief �{�b�g�֘A�̍X�V����
	void BotUpdate();

private:
	// ���̃V�[���̏�Ԃ��Ǘ�
	enum SceneNum
	{
		normal, // ��b�����Ă��Ȃ��Ƃ�
		talk_start, // �b�n��
		receptionist_talk,  // ��t�삪��b��
		accepting_quest, // �N�G�X�g��t��

		scene_max
	};
	// �N�G�X�g�V�[�����ǂ̏�ԉ���⊮����
	int scene_mode_num = normal;

	bool m_start_flag = false; // �^�C�g����ʂŃ{�^���������ꂽ���ǂ����̃t���O

	//! �J�����N���X�̃I�u�W�F�N�g
	Camera camera;

	//! ��t��̃I�u�W�F�N�g
	Receptionist receptionist;
	//! �b����G���A�p�̃J�v�Z��
	CapsuleCollision m_area;
	//! �b����G���A�ɓ������������Ă��Ȃ����̃t���O
	bool m_area_hit = false;
	//! �v���C���[����b�����ǂ���
	bool m_talk_flag = false;
	//! �e�e�L�X�g�̃t�H���g�T�C�Y
	static constexpr int TEXT_FONT_SIZE = 30;
	//! �t�H���g�̓����x�̍ő�l
	static constexpr int TEXT_BLEND_MAX = 255;
	//! �t�H���g�̓����x�̍ŏ��l
	static constexpr int TEXT_BLEND_MIN = 0;
	//! �u�����h����ύX���邽�߂̕ϐ�
	int m_text_blend_value = 0;
	//! �t���[�����J�E���g���邽�߂̂���
	int m_frame_count = 0;
	//! �u�����h��������A�����ɒB�����Ƃ��𔻒f���邽�߂̃t���O
	//! true : �B�����Afalse : �B���Ă��Ȃ�
	bool m_blend_flag = false;
	//! �u�����h�����グ�邩�����邩�̃t���O
	//! true : ���₷�Afalse : ���炷
	bool m_blend_change_flag = true;

	//-------------------------------------------------
	// �v���C���[�ɉ��������炢�����𑣂��e�L�X�g�֘A
	//-------------------------------------------------
	enum PlayerText
	{
		palyer_story1,
		palyer_story2,
		palyer_story_max
	};
	//! �N�G�X�g�G���A�̃e�L�X�g
	Text m_quest_area_text;
	//! �e�L�X�g�̉��s�ڂ����Ă���̂���ۑ�����ϐ�
	int m_quest_area_text_line = 0;



	//-------------------------------------------------
	// �N�G�X�g���j���[�֘A
	//-------------------------------------------------
	// �N�G�X�g�e�L�X�g�̐�
	enum QuestText
	{
		quest_quest1, // �N�G�X�g�P
		quest_quest2, // �N�G�X�g�Q
		quest_confirmatio1, // �ǂ̃N�G�X�g���������𕷂��Ă���
		quest_yes, // �ԓ�YES
		quest_no,  // �ԓ�NO
		quest_confirmation2, // ���̃N�G�X�g�ɂ��܂���
		quest_max
	};
	//! �N�G�X�g���j���[�e�L�X�g
	Text m_quest_text;
	//! �N�G�X�g�I���e�L�X�g�̉��s�ڂ���ۑ����邽�߂�
	int m_quest_text_line = 0;



	//-------------------------------------------------
	// ��t���b�֘A
	//-------------------------------------------------
	// ��t��̃e�L�X�g�̐�
	enum ReceptionText
	{
		recept_convo_text_1, // ��t��̊���
		recept_yes, // �ԓ�YES
		recept_no,  // �ԓ�NO
		recept_convo_text_2, // �o������̂��̊m�F
		recept_convo_text_3, // �{�b�g�̑��݂�m�点�Ă����
		recept_convo_text_4, // �ԓ���NO���Ԃ��Ă����Ƃ�
		recept_teception_max
	};
	//! �N�G�X�g��t��̃e�L�X�g
	Text m_reception_text;
	//! ��t��̃e�L�X�g�̉��s�ڂ���ۑ����邽�߂�
	int m_reception_text_line = 0;



	//! �`�悷��e�L�X�g�̔ԍ��Ƃ��ĊǗ�
	enum LandMarkText
	{
		f_text,                // F�L�[����������̃��b�Z�[�W
		x_text,               // X�{�^������������̃��b�Z�[�W
		excamation_pos, // �r�b�N���}�[�N

		landmark_text_max
	};

	//! ������X�N���[�����W�ɕ`�悷�邽�߂̍\����
	struct TextState
	{
		////! �t�H���g�T�C�Y
		//int font_size;
		//! �������ۑ����邽�߂̂���
		const char* text;
		//! �`�悵�������W
		Vector3 draw_pos;
		//! ���炵�������̍��W
		Vector3 shift_pos;
		//! ���̃X�N���[����ʂ���o����`�悵�Ȃ��悤�ɂ��邽�߂̂���
		bool draw_flag;
	};
	// �`�悵����������̐����쐬
	TextState m_landmark_text[landmark_text_max]
	{
		{	"F : �b��", {0.0f,0.0f,0.0f},{-8.0f,10.0f,0.0f},false},
	    {	"X : �b��", {0.0f,0.0f,0.0f},{-8.0f,10.0f,0.0f},false},
		{	" ! ", {0.0f,0.0f,0.0f},{0.0f,20.0f,0.0f},false},
	};
	// �ڈ�̕����̑傫��
	static constexpr int FONT_SIZE = 100;


	// ���݂ǂ̃e�L�X�g��`�悷��̂����Ǘ����邽�߂̗񋓑�
	enum TextNum
	{
		landmark_text, // �ڈ�ƂȂ��Ă���e�L�X�g
		player_text,      // �v���C���[���b���Ă���Ƃ��ɏo�Ă���e�L�X�g
		reception_text, // ��t�삪�b���Ă���Ƃ��ɏo�Ă���e�L�X�g
		quest_text,      // �N�G�X�g��ʂ�`�悵�Ă���Ƃ��̏���

		text_max
	};

	//---------------------------------------------
	// �N�G�X�g�֘A
	//---------------------------------------------
	// �N�G�X�g�I����ʂ̏��
	enum QuestSelectionMode
	{
		quest_selection, // �N�G�X�g�I��
		reply_selection,  // �ԓ��I��
	};
	// �N�G�X�g�I����ʂ̏��
	int m_quest_selection_num = 0;
	// �ǂ̃N�G�X�g��I�񂾂��ۑ�����ϐ�
	int m_quest_num = 0;
	// �ԓ����ǂ��炩�Ȃ̂���ۑ�����ϐ�
	int m_reply_num = 0;
	// �I����ʂőI�����Ă�����̂��������߂̕ϐ�
	int m_select_num = 0;


	Vector2 m_quest_draw_pos[quest_max] =
	{
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
		{SCREEN_W / 2, SCREEN_H / 2},
	};


	//---------------------------------------------
	// ��b�֘A(��̏��A��t��)
	//---------------------------------------------
	// ��t��̉�b���ǂ̏�ԂȂ̂����Ǘ�
	enum TalkMode
	{
		quest_before_accepting, // �N�G�X�g���󂯂�O
		quest_confirmation,     // �N�G�X�g�o���m�F
		quest_after_accepting,  // �N�G�X�g���󂯂���@(��قǐ���������悤�ɑ���)

		talk_mode_max
	};
	int m_quest_acception_num = quest_before_accepting;



	// �N���b���Ă���̂���`�悷�邽�߂̂���
	struct TextName
	{
		//! �N���b���Ă���̂��̖��O
		const char* name;
		//! �`�悵�������W
		Vector2 draw_pos;
	};

	TextName m_text_info[text_max]
	{
		{"Player", {0.0f,0.0f}},
		{"Player", {0.0f,0.0f}},
		{"��̏�", {0.0f,0.0f}},
		{"�N�G�X�g", {0.0f,0.0f}},

	};
	static constexpr int MANE_FONT_SIZE = 30;
	// �e�L�X�g�̕`����p
	Vector2 m_text_draw_pos;
	// �ڈ�̕`����W
	Vector2 m_landmark_draw_pos;
private:

	//! �{�b�g�̃I�u�W�F�N�g
	Bot m_bot;

	//! �q�b�g�X�g�b�v
	HitStop m_hit_stop;

	//! ��摜��ۑ����邽�߂̂���
	enum FaceImage
	{
		player_image,		// �v���C���[
		receptionist_image,	// ��t��

		face_image_max,
	};
	int m_face_image[face_image_max];
	// ��摜�̃T�C�Y
	static const int FACE_IMAGE_SIZE = 100;
};