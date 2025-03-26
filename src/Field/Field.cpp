#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/Model/Model.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"


#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h"
#include "src/Field/Field.h"


// 1���ǂO�������Ȃ�
// �t�B�[���h�̃I�u�W�F�N�g��u�����W�p�̓񎟌��z��
//	����������
Field::Field()
{

}

Field::~Field()
{

}

//---------------------------------------------------------------------------
//��������
//---------------------------------------------------------------------------
void Field::Init()
{
	// ���f���̃p�X�̃f�[�^�̓ǂݍ���
	m_model_data_path.LoadText("Data/Field/FieldText/ModelText.txt");
	// ���f���̍��W�̃f�[�^�̓ǂݍ���
	m_object_pos_info.LoadText("Data/Field/FieldText/ObjectPosData.txt");
	// ���f���̌����̃f�[�^�̓ǂݍ���
	m_object_rot_info.LoadText("Data/Field/FieldText/ObjectRotData.txt");
	// ���f���̃T�C�Y�̃f�[�^�̓ǂݍ���
	m_object_scale_info.LoadText("Data/Field/FieldText/ObjectScaleData.txt");

	// �t�B�[���h���f���̓ǂݍ���
	//m_field_model.LoadModel("Data/Field/ground/Stage00.mv1");
	m_field_model.LoadModel(m_model_data_path.lines[0].c_str());
	// �n�ʃ��f���̓����蔻��̐ݒ�
	SetHitGround(&m_field_model);
	// �t�B�[���h�ݒ�
	m_field_transform.pos.set(0, -10, 0);
	m_field_transform.scale.set(0.5f, 0.5f, 0.5f);

	// �t�B�[���h�̃I�u�W�F�N�g�̏�����
	ObjectInit();
}

void Field::ObjectInit()
{
	int model_num = 0;
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (i <= tree24)
		{
			model_num = terr;
		}
		else
		{
			model_num = stone;
		}

		// ���f���̓ǂݍ��ݏ���
		m_field_object[i].model.LoadModel(m_model_data_path.lines[model_num].c_str());
		// ���f���̍��W�ݒ�
		m_object_pos_info.ConversionNumbers(i, &m_field_object[i].transform.pos.x, &m_field_object[i].transform.pos.y, &m_field_object[i].transform.pos.z);
		// ���f���̌����̐ݒ�
		m_object_rot_info.ConversionNumbers(i, &m_field_object[i].transform.rot.x, &m_field_object[i].transform.rot.y, &m_field_object[i].transform.rot.z);
		// ���f���̃T�C�Y�̐ݒ�
		m_object_scale_info.ConversionNumbers(i, &m_field_object[i].transform.scale.x, &m_field_object[i].transform.scale.y, &m_field_object[i].transform.scale.z);
	}

	// �{�b�N�X�̍��W��ۑ����邽�߂̂���
	Vector3 pos = { 100.0f,0.0f,300.0f };
	// �{�b�N�X�̃T�C�Y
	Vector3 size = { 30.0f,60.0f,500.0f };
	m_hit_wall[0].CreateBox(pos, size);

	pos.set(500.0f, 0.0f, 300.0f);
	size.set(30.0f, 60.0f, 500.0f);
	m_hit_wall[1].CreateBox(pos, size);

	pos.set(300.0f, 0.0f, 100.0f);
	size.set(500.0f, 60.0f, 30.0f);
	m_hit_wall[2].CreateBox(pos, size);

	pos.set(300.0f, 0.0f, 500.0f);
	size.set(500.0f, 60.0f, 30.0f);
	m_hit_wall[3].CreateBox(pos, size);


}

//---------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------
void Field::Update()
{

}

//---------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------
void Field::Draw(Vector3 camera_pos, float camera_length,Vector3 player_pos)
{
	// ���������ɕ`�悷��
	// �`��u�����h���[�h���A���t�@�u�����h�i�T�O���j�ɂ���

	m_field_model.DrawModel(&m_field_transform);

	// �������ɂ��Ȃ�����`��
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (ObjectDrawSituation(camera_pos, camera_length, player_pos, m_field_object[i].transform.pos))
		{
			m_field_object[i].model.DrawModel(&m_field_object[i].transform);
		}
	}
	// �������ɂ��镪�̕`��
	for (int i = 0; i < MODEL_MAX; i++)
	{
		if (i >= stone1)
		{
			if (ObjectDrawSituation(camera_pos, camera_length, player_pos, m_field_object[i].transform.pos) == false)
			{
				// �{��������
				MV1SetOpacityRate(m_field_object[i].model.m_model, 0.3f);
				// ���f���̕`��
				m_field_object[i].model.DrawModel(&m_field_object[i].transform);
				// �`��u�����h���[�h���m�[�u�����h�ɂ���
				MV1SetOpacityRate(m_field_object[i].model.m_model, 1.0f);
			}
		}
	}	
}

//---------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------
void Field::Exit()
{

}






