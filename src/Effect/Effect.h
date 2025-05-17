#pragma once
//! @file Effect.h
//! @brief �G�t�F�N�g�p�̃N���X
class Effect
{
public:
	//! @brief �R���X�g���N�^
	Effect();
	//! @brief �f�X�g���N�^
	~Effect();

	//! @brief �G�t�F�N�g�p�̔z��ϐ��̊m�ۊ֐�
	//! @param �G�t�F�N�g�̍ő��
	void NewArraySecure(int effect_max);

	//! @brief �m�ۂ����G�t�F�N�g���J������֐�
	void EffectDelete();

	//! @brief �G�t�F�N�g�̓ǂݍ��ݗp�֐�
	//! @param �G�t�F�N�g�ۑ��p�ԍ�
	//! @param �G�t�F�N�g�̍Đ����x
	void LoadEffect(const char file_path[256], int effect_noint ,float effect_play_speed);

	//! @brief �G�t�F�N�g���Đ�
	//! @param �Đ��������G�t�F�N�g�ԍ�
	//! @param �G�t�F�N�g�̍��W
	void PlayEffect(int effect_num, Vector3 pos);

	//! @brief �G�t�F�N�g�̍��W��ݒ�
	//! @param ���炵�������W
	void SetEffectPos( Vector3 pos = { 0.0f,0.0f,0.0f });

	//! @brief �G�t�F�N�g�̍��W�����f���̌����ɍ��킹�Ă��炷�֐�
	//! @param �ݒ肵�����L�����̍��W
	//! @param ���炵�������W
	//! @param �ݒ肵�������f���̌���
	void SetEffectRotPos(Vector3 character_pos, Vector3 pos, Vector3 rot);

	//! @brief �G�t�F�N�g�̍��W�����f���̃m�[�h�̍��W�A�m�[�h�̌����ɍ��킹�Ă��炷�֐�
	//! @param �ݒ肵����Node�̍��W
	//! @param ���炵�������W
	//! @param �ݒ肵����Node�̌���
	void SetEffectRotPosNode(Vector3 character_pos, Vector3 pos, Vector3 rot);

	//! @brief �G�t�F�N�g�̌����̐ݒ�
	//! @param X���̉�]
	//! @param Y���̉�]
	//! @param Z���̉�]
	void SetEffectRot(float rot_x, float rot_y, float rot_z);

	//! @brief �G�t�F�N�g�ׂ̍����T�C�Y�̐ݒ�
	//! @param �e�����ɑ΂��Ă̊g�嗦(Vector3)
	void SetEffectSize( Vector3 size);

	//! @brief �G�t�F�N�g�̍Đ����x�̐ݒ�
	//! @param �Đ����x
	void SetEffectPlaySpeed( float speed);

	//! @brief �G�t�F�N�g�̐F�̐ݒ�
	//! @param �ݒ肵�����J���[
	void SetEffectColor( COLOR_F color);

	//! @brief �G�t�F�N�g���Đ������ǂ����𒲂ׂ�
	//! @return	�Đ������ǂ���?
	//! @return	0:�Đ����A - 1 : �Đ�����Ă��Ȃ��A�������͍Đ��I��
	int IsPlayingEffect();

	//! @brief �Đ����̃G�t�F�N�g���I��������
	void StopEffect();

	//! @brief �G�t�F�N�g�̕`��
	void EffectDraw();
private:
	//! �G�t�F�N�g��ۑ�����p�̕ϐ�
	struct EffectInfo
	{
		//! �G�t�F�N�g�̓��ꕨ
		int effect_handle;
		//! �G�t�F�N�g�̍Đ����x
		int effect_play_speed;
		
	};
	std::vector<EffectInfo>m_effect_info;
	//! ���ݍĐ����̃G�t�F�N�g�ԍ���ۑ�����ϐ�
	int m_playing_effect_num;

private:
	//! ���ݍĐ����̃G�t�F�N�g�̂�ۑ�����ϐ�
	std::vector<int> m_playing_effect;
	
	//! �G�t�F�N�g�̍Đ������̃t���O
    int m_playing_effect_flag;

	//! ���f���̍��W
	Vector3 m_model_pos;
public:
	//! �G�t�F�N�g���Đ��Đ����Ă������̃t���O
	bool m_play_effect_flag = true;
};
