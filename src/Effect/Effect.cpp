#include "src/WinMain.h"

#include "src/System/Vector3.h"
#include "Effect.h"

Effect::Effect()
{

}
Effect::~Effect()
{

}
//---------------------------------------------------------------
// �G�t�F�N�g�p�̔z��m�ۊ֐�
//---------------------------------------------------------------
void Effect::NewArraySecure(int effect_max)
{
	// �G�t�F�N�g�̍ő吔���m�ۂ��Ă���
	//m_effect_max = effect_max;
	// �G�t�F�N�g�̔z����m��
	m_effect_info.resize(effect_max);
	// �Đ����̃G�t�F�N�g�̂�ۑ�����ϐ�
	m_playing_effect.resize(effect_max);
}


//---------------------------------------------------------------------------
//  �G�t�F�N�g�p�ϐ���delete�p�֐�
//---------------------------------------------------------------------------
void Effect::EffectDelete()
{
	// �m�ۂ����z����J������
	m_effect_info.clear();
	m_playing_effect.clear();
}

//---------------------------------------------------------------------------
//  �G�t�F�N�g�̓ǂݍ���
//---------------------------------------------------------------------------
void Effect::LoadEffect(const char file_path[256], int effect_no, float  effect_play_speed)
{
	// �G�t�F�N�g�̓ǂݍ���
	m_effect_info[effect_no].effect_handle = LoadEffekseerEffect(file_path);
	// �G�t�F�N�g�̍Đ����x�̐ݒ�
	m_effect_info[effect_no].effect_play_speed = (int)effect_play_speed;
}

//---------------------------------------------------------------------------
//  �G�t�F�N�g�̍Đ�
//---------------------------------------------------------------------------
void Effect::PlayEffect(int effect_num, Vector3 pos)
{
	// �Đ����̃G�t�F�N�g�ԍ���ۑ�����
	m_playing_effect_num = effect_num;
	// �G�t�F�N�g�̍��W���w��̍��W�ɃZ�b�g
	m_model_pos = pos;
	// �G�t�F�N�g���Đ������Ă��Ȃ��Đ�������
	// �G�t�F�N�g���d�����Ȃ��悤��
	m_playing_effect_flag = IsPlayingEffect();
	if (m_playing_effect_flag == -1)
	{
		// �G�t�F�N�g�̍Đ�
		m_playing_effect[m_playing_effect_num] = PlayEffekseer3DEffect(m_effect_info[m_playing_effect_num].effect_handle);
	}
	// ���ݍĐ����̃G�t�F�N�g�ɐݒ肵�Ă���
	// �G�t�F�N�g�̍Đ��X�s�[�h�̐ݒ�
	SetSpeedPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], (float)m_effect_info[effect_num].effect_play_speed);
	// �G�t�F�N�g�̐������W�̐ݒ�
	SetPosPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], pos.x, pos.y, pos.z);
}

//---------------------------------------------------------------------------
// �Đ����̃G�t�F�N�g�̍��W��ݒ�
//---------------------------------------------------------------------------
void Effect::SetEffectPos(Vector3 pos)
{
	// �G�t�F�N�g���Đ����Ȃ�
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// �ݒ肵�������W
		Vector3 pos_ = m_model_pos + pos;
		// �G�t�F�N�g�̐ݒ�
		SetPosPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], pos_.x, pos_.y, pos_.z);

	}
}

//---------------------------------------------------------------------------
//  �Đ����̃G�t�F�N�g�̍��W�����f���̌����ɍ��킹�Ă��炷�֐�
//---------------------------------------------------------------------------
void Effect::SetEffectRotPos(Vector3 character_pos, Vector3 pos2, Vector3 rot)
{
	// �G�t�F�N�g���Đ����Ȃ�
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// �ݒ肵�������f���̌����ɍ��킹�č��W���Z�b�g����
		Vector3 pos_ = m_model_pos;
		pos_.x += pos2.x * sinf(TO_RADIAN(rot.y));
		pos_.y += pos2.y;
		pos_.z += pos2.z * cosf(TO_RADIAN(rot.y));
		// �G�t�F�N�g�̍��W��ݒ�
		SetPosPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], pos_.x, pos_.y, pos_.z);
	}
}

//---------------------------------------------------------------------------
// �Đ����̃G�t�F�N�g�̍��W�����f���̃m�[�h�̍��W�A�m�[�h�̌����ɍ��킹�Ă��炷�֐�
//---------------------------------------------------------------------------
void Effect::SetEffectRotPosNode(Vector3 node_pos, Vector3 pos, Vector3 rot)
{
	// �G�t�F�N�g���Đ����Ȃ�
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// �ݒ肵�������f���̌����ɍ��킹�č��W���Z�b�g����
		Vector3 pos_ = node_pos;
		pos_.x += pos.x * sinf(TO_RADIAN(rot.y));
		pos_.y += pos.y;
		pos_.z += pos.z * cosf(TO_RADIAN(rot.y));
		// �G�t�F�N�g�̍��W��ݒ�
		SetPosPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], pos_.x, pos_.y, pos_.z);
	}
}




//---------------------------------------------------------------------------
//  �Đ����̃G�t�F�N�g�̌����̂�ݒ�
//---------------------------------------------------------------------------
void Effect::SetEffectRot(float rot_x, float rot_y, float rot_z)
{
	// �G�t�F�N�g���Đ����Ȃ�
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// �ݒ肵���������̂��o��
		Vector3 rot_;
		rot_.x = TO_RADIAN(rot_x);
		rot_.y = TO_RADIAN(rot_y);
		rot_.z = TO_RADIAN(rot_z);
		// �G�t�F�N�g�̌�����ݒ�
		SetRotationPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], rot_.x, rot_.y, rot_.z);
	}
}

//---------------------------------------------------------------------------
// �Đ����G�t�F�N�g�ׂ̍����T�C�Y�̐ݒ�
//---------------------------------------------------------------------------
void Effect::SetEffectSize(Vector3 size)
{
	// �G�t�F�N�g���Đ����Ȃ�
	int  a = IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]);
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// �G�t�F�N�g�̃T�C�Y��ݒ�
		SetScalePlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], size.x, size.y, size.z);
	}
}

//---------------------------------------------------------------------------
//  �Đ����G�t�F�N�g�̍Đ����x�̐ݒ�
//---------------------------------------------------------------------------
void Effect::SetEffectPlaySpeed(float speed)
{
	// �G�t�F�N�g���Đ����Ȃ�
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		SetSpeedPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], speed);
	}
}

//---------------------------------------------------------------------------
//  �G�t�F�N�g�̐F�̐ݒ�
//---------------------------------------------------------------------------
void Effect::SetEffectColor(COLOR_F color)
{
	// �G�t�F�N�g���Đ����Ȃ�
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// �G�t�F�N�g�̐F�̐ݒ�
		SetColorPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], (int)color.r, (int)color.g, (int)color.b, (int)color.a);
	}
}

//---------------------------------------------------------------------------
//  �G�t�F�N�g���Đ������ǂ����𒲂ׂ�(���ݍĐ����̃G�t�F�N�g)
//---------------------------------------------------------------------------
int Effect::IsPlayingEffect()
{
	return IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]);
}

//---------------------------------------------------------------------------
//  �Đ����̃G�t�F�N�g���~����
//---------------------------------------------------------------------------
void Effect::StopEffect()
{
	// �G�t�F�N�g���Đ����̎�������~����
	if(IsPlayingEffect() == 0)
	{
		StopEffekseer3DEffect(m_playing_effect[m_playing_effect_num]);
	}
}

//---------------------------------------------------------------------------
//  �G�t�F�N�g�̕`��
//---------------------------------------------------------------------------
void Effect::EffectDraw()
{
	DrawEffekseer3D();
}