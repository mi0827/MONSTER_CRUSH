#pragma once

//! @file Camera.h
//! @brief �J�����N���X
class Camera
{
public:
	//---------------
	// �֐��̒�`
	//---------------
	// �R���X�g���N�^
	Camera();

	void Init();        //�@��������

	void PlayFieldInit(); // �v���C��ʂ̏����ݒ�



	// �v���C���[�̏����󂯎��`�ɂ��܂�
	// ����̓v���C���[�̂����ɃJ�������������̏o
	// �v���C���[�̍��W�ƌ������󂯎���悤�ɂ��܂�
	//! @brief �X�V����
	//! @param �ڂ������ڕW�̍��W
	void MouseCamera(Vector3* target_pos);

	//! @brief �^�[�Q�b�g�J�����̍X�V����
	//! @param �J�����̎薈���ɂ���^�[�Q�b�g�̍��W
	//! @param �J�����̉����ɂ���^�[�Q�b�g�̍��W
	void TargetCamera(Vector3* target_pos1, Vector3* target_pos2);
	bool target_camera_on = false;


	//! @brief �ǂ���̃J�������g�p���邩�̕ύX�����p�̊֐�
	//! @param �ǂ���̃J�������g���� true : �}�E�X�J�����Afalse : �^�[�Q�b�g�J����
	//! @param �J�����̎薈���ɂ���^�[�Q�b�g�̍��W
	//! @param �J�����̉����ɂ���^�[�Q�b�g�̍��W
	void UseCameraUpdate(bool change_camera, Vector3* target_pos1, Vector3* target_pos2);

	//! @brief �J�������ǂɖ��܂�Ȃ��悤�ɂ���
	void HitObject(Vector3* obj_pos, Vector3* obj_size);
	//! @brief �`�揈���̑O�ɃJ�����A��ʂ̃Z�b�g
	void DrawSet();
	//! @brief �`�揈��
	void Draw();
	//! @brief �I������
	void Exit();

	//! @brief �J�����ړ��̍ۂ̃}�E�X�̊��x��ݒ�
	void SetCameraSensi(float mouse_sensi);

	//! @brief �J�����̍��������Ȃǂ�ݒ肷��֐�
	//! @param �J�����̌�����W�̍�����(�f�t�H���g�Ńv���C���[���̒l�����Ă���)
	//! @param �J��������ڕW�܂ł̋���(�f�t�H���g�Ńv���C���[���̒l�����Ă���)
	void SetCamera(float height = CAMERA_HEIGHT_PLAYER, float length = CAMERA_LENGTH_MAX);

	//! @brief �ڂ������ڕW���܂��悤�ɃJ�����̈ړ�����
	//! @param �ڂ������^�[�Q�b�g
	//! @param �U�������������(1 : �E, 2 : ��)
	//! @param �ړ�������������
	void MoveCamera(Vector3* target_pos, int direction, float speed);

	//! @brief �J�����V�F�C�N���s��(�ŏI�I�ɗh�ꂪ��܂�)�֐�
	//! @param �J�����̐U������p���[
	//! @param �ǂꂭ�炢�̎��ԗh��ė~������
	void CameraShakeLimited(float power, float time);


	//! @brief �J�����̌����̐ݒ�
	//! @param �ݒ肵��������
	void SetCameraRot(Vector3 rot);

	//! @brief �J�����̌����擾����֐�
	Vector3 GetCameraRot();

	//! @brief �J�����ƃ^�[�Q�b�g�Ƃ̋������}�E�X�z�C�[���ŕύX�ł���悤�ɂ���
	void ChangeDistance();

	enum Direction
	{
		right, // �E
		left,   // ��
	};

	// �J�����ƈڂ��W�I�Ƃ̋���
	static constexpr float CAMERA_LENGTH_MAX = 75.0f;      // �v���C���[�Ƃ̍ő�l
	static constexpr float CAMERA_LENGTH_MIN = 50.0f;       // �v���C���[�Ƃ̍ŏ��l
	static constexpr float CAMERA_MONSER_LENGTH = 100.0f;  // �����X�^�[
	// �J�������ڂ����������̂���y���W���ω�������l
	static constexpr float CAMERA_HEIGHT_PLAYER = 5.0f;    // �v���C���[
	static constexpr float CAMERA_HEIGHT_MONSTER = 20.0f;  // �����X�^�[

	// �J�����̉�]�X�s�[�h
	static constexpr float MOUSE_SENSI_CORRECT = 0.01f;  // �}�E�X�p
	static constexpr float PAD_CAMERA_ROT_SPEED = 3.0f;    // �p�b�h�p

	static constexpr float UP_ANGLE_MAX = 30.0f;           // �J�����̏�A���O���̍ő�
	static constexpr float LOWER_ANGLE = 10.0f;             // �J�����̉��A���O���̍Œ�i�n�ʂɖ��܂�Ȃ����x�j
	static constexpr float BOX_SIZE = 4.0f;                     // �{�b�N�X�̃T�C�Y
	static constexpr float BOX_SIZE_HALF = (BOX_SIZE / 2.0f); // �����̃T�C�Y

	// �^�[�Q�b�g�J�����̐U��������x
	static constexpr float TARGET_ROT_SPEED = 2.3f;
	// �^�[�Q�b�g�J�����̐U������\�͈�
	static constexpr float RANGE = 500;

	//---------------
	// �ϐ��̒�`
	//---------------
public:
	//! �ڂ������^�[�Q�b�g�Ƃ̋���
	float m_length = 0;

	//! �J������������W
	Vector3 m_look;
	Vector3 m_look_2{ 0.0f,0.0f,0.0f };
	float m_look_height = 0; // �J������������W��Y���W�̃v���X���p�̕ϐ�


	//! �J�����̍��W
	Vector3 m_pos;
public:
	//! �J�����̌���
	Vector3 m_rot;

	//! �}�E�X�J�[�\���p�ϐ�
	float m_mouse_wheel = 0;
private:
	

	//! �}�E�X�̈ړ��ʗp�̕ϐ�
	float m_mouse_move_x = 0.0f; //! X���W�̈ړ���
	float m_mouse_move_y = 0.0f; //! Y���W�̈ړ���
	Vector3 m_before_pos = { 0.0f,0.0f,0.0f };   //! ���t���[���ړ��O�̍��W�������p�̕ϐ�
	Vector3 m_hit_box_size = { 0.0f,0.0f,0.0f }; //! �ǂƂ̓����蔻��p�̕ϐ�

	//! �}�E�X�̊��x
	float m_camera_mouse_sensi = 0;


	//�I�J�����V�F�C�N�ł̐U���̋���
	float m_power = 0;
	//! �J�����V�F�C�N�łǂꂾ���̍��W�����炷��
	Vector3 m_shake_pos;
	// �ǂꂾ���h��Ă��Ăق����̂�
	float m_shake_time = 0;

public:
	int pad_no = 0;				//! ���Ԃ̃p�b�h���g���Ă���̂�
	void SetPadNo(int no)
	{
		pad_no = no;
	}

	// ���̓�̕ϐ��͂��Ƃŏ����\��
	// �^�[�Q�b�g�J�����Ɏg�p������ϕۑ��p�ϐ�
	float inner_product;
	// �^�[�Q�b�g�J�����Ɏg�p����O�ϕۑ��p�̕ϐ�
	Vector3 cross_product;




};
