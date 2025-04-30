#pragma once

//! @file Spotlight.h
//! @brief �X�|�b�g���C�g�p�̃N���X
class Spotlight
{
public:
	// 9��13���̂��[�Ղ�ɃX�|�b�g���C�g�̎g�p�������Ă���
	//! �R���X�g���N�^
	Spotlight();
	//! �f�X�g���N�^
	~Spotlight();

	//! �X�|�b�g���C�g�̈�ԍŏ��ɂ���ݒ�
	//! @param
	void InitSetLight(Vector3 pos, Vector3 spot_pos, float outAngle, float inangle, float range, float atten0, float atten1, float atten2);

	//! ���C�g�̍��W���Z�b�g����
	void SetLightPos(Vector3* pos);

	//! ���C�g���Ƃ炷���W���Z�b�g����
	void SetLightSpotPos(Vector3* spot_pos);

	//! �X�|�b�g���C�g�̍��W�̍X�V����
	void UpdateTransform(Vector3* pos, Vector3* spot_pos);

private:
	//! ���C�g�n���h��
	int m_light_handle = 0;
public:
	//! �L�����̂��Ƃ���납��Ƃ炷�X�|�b�g���C�g

	float out_angle = 1.0f;
	float in_angle = 1.0f;
	float range = 1.0f;
	float atten0 = 1.0f;
	float atten1 = 1.0f;
	float atten2 = 1.0f;



};