#pragma once

//! @file EndScene.h
//! @brief �^�C�g���V�[���̃N���X
class EndScene : public Scene_Base
{
	//int effeckt_h;
public:

	//! �R���X�g���N�^
	EndScene();
	//! �f�X�g���N�^
	~EndScene();


	//! @brief ��������
	void Init()override;

	//! @brief �X�V����
	void Update()override;

	//! @brief �`�揈��
	void Draw()override;

	//! @brief �I������
	void Exit()override;

	//! @brief �t�B�[���h�ƃL�����N�^�[�Ƃ̓����蔻��
	void HitField()override;

	//! @brief �I�v�V�������j���[�ł�����Ă����l�𔽉f����֐�
	//! @param BGM�̉���
	//! @param SE�̉���
	//! @param �}�E�X���x
	void OptionValuesReflect(int bgm, int se, int mouse)override;

	//! @brief �T�E���h�̍Đ�
	//! @param �Đ�������SE�ԍ�
	void SoundPlay(int se_num);


private:


	// �e�L�X�g�I�u�W�F�N�g
	Text m_text;

	// �G���h�V�[���ŕ`�悷��e�L�X�g�̐�
	enum EndText
	{
		text1, // �^�C�g���ɖ߂�e�L�X�g
		text2, // �N�G�X�g�V�[���ɖ߂�e�L�X�g

		text_max
	};

	//! �e�L�X�g�̕`��ʒu
	Vector2 m_text_draw_pos[text_max] =
	{
		{ (100),(SCREEN_H / 2)},
		{ (100 + SCREEN_W / 2),(SCREEN_H / 2)}
	};

	//! ��ʐ؂�ւ��p�̕ϐ�
	int m_title_scene = 0;
	//! �ǂ������󂵂Ă��邩
	int m_select_num = 0;
	//! �I���}�X�ɂ������Ă��邩�ǂ����̃t���O
	bool m_hit_select_flag = false;
	// �^�C�g����ʂŃ{�^���������ꂽ���ǂ����̃t���O
	bool m_start_flag = false;

	// �J�����N���X�̃I�u�W�F�N�g
	Camera camera;

	//! SE�p�̃I�u�W�F�N�g
	Sound m_se;
	enum se_num
	{
		se_1,

		se_max
	};

private:
	//! �V�[���ύX�łǂ���̃V�[���Ɉړ����邩��ۑ�����
	int m_change_scene = 0;

	//! �G���h�V�[���ŉ��t���[���i��ł���̂���ۑ����邽�߂̕ϐ�
	int m_frame_count = 0;
	//! �G���h�V�[���Ŏw��̃t���[���߂���Ə���ɃV�[����ς���
	static constexpr int END_SCENE_FRAME_MAX = 480; // ���݂̃Q�[�����U�O�t���[���Ȃ̂łP�O�b
};
