#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"
#include "Model.h"

//-----------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------
Model::Model()
{
	m_model = 0;
}

//-----------------------------------------------
// �f�X�g���N�^
//-----------------------------------------------
Model::~Model()
{
	// ���f���f�[�^�̍폜
	MV1DeleteModel(m_model);
}

//-----------------------------------------------
// ���f���ǂݍ��݊֐�
//-----------------------------------------------
void Model::LoadModel(const char data[256])
{
	// ���f���̓ǂݍ���
	m_model = MV1LoadModel(data);

	GetMaterilaInfo();
}

//-----------------------------------------------
// �`��p�֐�
//-----------------------------------------------
void Model::DrawModel(Transform* transform)
{

	// ���f���̃T�C�Y�̒���
	m_transform = *transform;
	//m_transform.scale.set(0.1f, 0.1f, 0.1f);
	//m_transform.scale.set(1.0f, 1.0f, 1.0f);

	MV1SetPosition(m_model, VGet(m_transform.pos.x, m_transform.pos.y, m_transform.pos.z)); // �`�悷��v���C���[���f���̍��W�̐ݒ�
	MV1SetRotationXYZ(m_model, VGet(TO_RADIAN(m_transform.rot.x), TO_RADIAN(m_transform.rot.y + 180), TO_RADIAN(m_transform.rot.z))); // ���f���̉�]
	MV1SetScale(m_model, m_transform.scale.VGet()); // ���f���̑傫��(10���̂P�̃T�C�Y)
	MV1DrawModel(m_model); // ���f���̕`��

}

//-----------------------------------------------
// ���f���̍��W��������Ă���p�֐�
//-----------------------------------------------
Vector3 Model::GetModelPos()
{
	return m_transform.pos;
}

//-----------------------------------------------
// ���f���̍��W���Z�b�g����p�֐�
//-----------------------------------------------
void Model::SetModelPos(Vector3 pos)
{
	m_transform.pos.set(pos);
}

//-----------------------------------------------
// ���f���̌��������炤�p�֐�
//-----------------------------------------------
Vector3 Model::GetModelRot(Vector3 rot)
{
	return m_transform.rot;
}

//-----------------------------------------------
// ���f���Ɍ������Z�b�g���邽�ߗp�֐�
//-----------------------------------------------
void Model::SetModelRot(Vector3 rot)
{
	m_transform.rot.set(rot);
}

//-----------------------------------------------
// ���f���̃X�P�[����ύX���邽�߂̊֐�
//-----------------------------------------------
void Model::SetModelScale(Vector3 scale)
{
	m_transform.scale.set(scale);
}

//-----------------------------------------------
// ���f���̎w��̃m�[�h�̍��W���擾����
//-----------------------------------------------
Vector3 Model::GetNodePos(int node_index)
{
	Vector3 pos;
	// �w�肵���m�[�h�̍��W������Ă���
	pos = MV1GetFramePosition(m_model, node_index);

	return pos;
}

//-----------------------------------------------
// ���f���̎w��̃m�[�h�̏����擾����
//-----------------------------------------------
MATRIX Model::GetNodeWorldMatrix(int node_index)
{
	MATRIX mat = MV1GetFrameLocalWorldMatrix(m_model, node_index);

	return mat;
}


//-----------------------------------------------
// ���f���̎w��̃m�[�h�̊J�X�����擾����
//-----------------------------------------------
Vector3 Model::GetNodeRot(int node_index)
{
	MATRIX mat = GetNodeWorldMatrix(node_index);

	Vector3 rot = { mat.m[2][1],mat.m[2][2],mat.m[2][3] };
	return rot;
}


//-----------------------------------------------
// �}�e���A���̊e�����擾����
//-----------------------------------------------
void Model::GetMaterilaInfo()
{
	m_material_max = MV1GetMaterialNum(m_model);

	// �}�e���A���̐����z��𑝂₷
	m_material.resize(m_material_max);

	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Diffuse = MV1GetMaterialDifColor(m_model, i);
		m_material[i].Ambient = MV1GetMaterialAmbColor(m_model, i);
		m_material[i].Emissive = MV1GetMaterialEmiColor(m_model, i);
		m_material[i].Specular = MV1GetMaterialSpcColor(m_model, i);
		m_material[i].Power = MV1GetMaterialSpcPower(m_model, i);
	}
}

//-----------------------------------------------
// �}�e���A����Diffuse�̐ݒ�p�֐�
//-----------------------------------------------
void Model::SetMaterialDiffuse(COLOR_F color)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Diffuse = { color.r,color.g,color.b, color.a };
		// �󂯎���Ă����J���[��ݒ肷��
		MV1SetMaterialDifColor(m_model, i, m_material[i].Diffuse);
	}
}

//-----------------------------------------------
// �}�e���A����Ambient�̐ݒ�p�֐�
//-----------------------------------------------
void Model::SetMaterialAmbient(COLOR_F color)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Ambient = { color.r,color.g,color.b, color.a };
		// �󂯎���Ă����J���[��ݒ肷��
		MV1SetMaterialAmbColor(m_model, i, m_material[i].Ambient);
	}
}

//-----------------------------------------------
// �}�e���A����Specular�̐ݒ�p�֐�
//-----------------------------------------------
void Model::SetMaterialSpecular(COLOR_F color)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Specular = { color.r,color.g,color.b, color.a };
		// �󂯎���Ă����J���[��ݒ肷��
		MV1SetMaterialSpcColor(m_model, i, m_material[i].Specular);
	}
}

//-----------------------------------------------
// �}�e���A����Emissive�̐ݒ�p�֐�
//-----------------------------------------------
void Model::SetMaterialEmissive(COLOR_F color)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Emissive = { color.r,color.g,color.b, color.a };
		// �󂯎���Ă����J���[��ݒ肷��
		MV1SetMaterialEmiColor(m_model, i, m_material[i].Emissive);
		m_material[i].Emissive = MV1GetMaterialEmiColor(m_model, i);
	}
}

//-----------------------------------------------
// �}�e���A���̃X�y�L�����n�C���C�g�̑N���x�̐ݒ�p�֐�
//-----------------------------------------------
void Model::SetMaterialPower(float power)
{
	for (int i = 0; i < m_material_max; i++)
	{
		m_material[i].Power = power;
		// �󂯎���Ă����X�y�L�����[�̂�ݒ肷��
		MV1SetMaterialSpcPower(m_model, i, m_material[i].Power);
	}
}

//-----------------------------------------------
// ���f����Y����]�ɐ�����������
//-----------------------------------------------
void Model::RotationYLimits()
{
	if (m_transform.rot.y > 360)
	{
		m_transform.rot.y = 0;
	}
	if (m_transform.rot.y < 0)
	{
		m_transform.rot.y = 360;
	}
}
