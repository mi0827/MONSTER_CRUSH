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
#include "src/Field/TitleField.h"

// 1���ǂO�������Ȃ�
// �t�B�[���h�̃I�u�W�F�N�g��u�����W�p�̓񎟌��z��
//	����������
TitleField::TitleField()
{

}

TitleField::~TitleField()
{
}

//---------------------------------------------------------------------------
//��������
//---------------------------------------------------------------------------
void TitleField::Init()
{
	// �t�B�[���h���f���̓ǂݍ���
	m_field_model.LoadModel("Data/Field/ground/Stage00.mv1");
	// �n�ʃ��f���̓����蔻��̐ݒ�
	SetHitGround(&m_field_model);
	// �t�B�[���h�ݒ�
	m_field_transform.pos.set(0, -10, 0);
	m_field_transform.scale.set(1, 1, 1);

	// �t�B�[���h�̃I�u�W�F�N�g�̏�����
	ObjectInit();
}

void TitleField::ObjectInit()
{
	// �؂̏�������
	TreeInit();
	// �t�F���X�̏�������
	FenceInit();
	// �΂̏�������
	StoneInit();
	
	// ����̒l����̐ݒ�
	// ���W�̐ݒ�
	m_hit_around_pos[0].set(0.0f, 0.0f, 870.0f);      // ���
	m_hit_around_pos[1].set(130.0f, 0.0f,450.0f);    // ����
	m_hit_around_pos[2].set(- 130.0f, 0.0f,450.0f);  // �E��
	m_hit_around_pos[3].set(0.0f,0.0f,0.0f);            // �܂�
	// �T�C�Y�̐ݒ�
	m_hit_around_size[0].set(400.0f,90.0f, 10.0f);     // ���
	m_hit_around_size[1].set(20.0f, 90.0f, 900.0f);	 // ����
	m_hit_around_size[2].set(20.0f, 90.0f, 900.0f);	 // �E��
	m_hit_around_size[3].set(400.0f, 90.0f, 10.0f);	 // �܂�

	// �����蔻��̐ݒ�
	for (int i = 0; i < AROUND_MAX; i++)
	{
		m_hit_around[i].CreateBox(m_hit_around_pos[i], m_hit_around_size[i]);
	}
	

}

//---------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------
void TitleField::Update()
{

}

//---------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------
void TitleField::Draw()
{

	m_field_model.DrawModel(&m_field_transform);

	for (int i = 0; i < MODEL_MAX; i++)
	{
		m_field_object[i].model.DrawModel(&m_field_object[i].transform);
		//m_field_object[i].box_hit.Draw(255, 255);
	}

	// �ǂ̓����蔻��̕�����������i�f�o�b�O�̎������j
	/*for (int i = 0; i < AROUND_MAX; i++)
	{
		m_hit_around[i].Draw(255,255);
	}*/
	//// �؂̓����蔻��̕`��
	//for (int i = 0; i < TREE_MAX; i++)
	//{
	//	m_hit_tree[i].Draw();
	//}

	// �؂̓����蔻��̕`��
	/*for (int i = 0; i < TREE_MAX; i++)
	{
		m_hit_tree[i].Draw();
	}*/
	
}

//---------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------
void TitleField::Exit()
{

}

//---------------------------------------------------------------------------
//	�؂̃I�u�W�F�N�g�̏����ݒ�
//---------------------------------------------------------------------------
void TitleField::TreeInit()
{
	//--------------------------------------------------------------------------
	// ���f���f�[�^�̓ǂݍ���
	//--------------------------------------------------------------------------
	m_field_object[tree2].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree1].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree3].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree4].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree5].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree6].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree7].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree8].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree9].model.LoadModel( "Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree10].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree11].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree12].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree13].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree14].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree15].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree16].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree17].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree18].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree19].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree20].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree21].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree22].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree23].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree24].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree25].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree26].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");
	m_field_object[tree27].model.LoadModel("Data/Field/object/tree/Stage_Obj001.mv1");


	//--------------------------------------------------------------------------
	// ���W�ݒ�
	//--------------------------------------------------------------------------
	// �t�B�[���h�̎���̕���
	m_field_object[tree1].transform.pos.set(0.0f, 40.0f, 0.0f); // �����̈�ԑ傫����

	// ����
	m_field_object[tree2].transform.pos.set(100.0f, 30.0f, 0.0f);
	m_field_object[tree3].transform.pos.set(150.0f, 35.0f, -50.0f);
	m_field_object[tree4].transform.pos.set(170.0f, 40.0f, 0.0f);
	m_field_object[tree5].transform.pos.set(150.0f, 35.0f, 70.0f);
	m_field_object[tree6].transform.pos.set(150.0f, 40.0f, 140.0f);
	m_field_object[tree7].transform.pos.set(130.0f, 40.0f, 210.0f);
	m_field_object[tree8].transform.pos.set(150.0f, 40.0f, 280.0f);
	m_field_object[tree9].transform.pos.set(120.0f, 40.0f, 350.0f);
	m_field_object[tree10].transform.pos.set(150.0f, 50.0f, 420.0f);
	m_field_object[tree11].transform.pos.set(150.0f, 65.0f, 490.0f);
	m_field_object[tree12].transform.pos.set(120.0f, 80.0f, 560.0f);
	m_field_object[tree13].transform.pos.set(150.0f, 80.0f, 630.0f);
	m_field_object[tree27].transform.pos.set(150.0f, 60.0f, 700.0f);

	// �E��
	m_field_object[tree14].transform.pos.set(-100.0f, 30.0f, 0.0f);
	m_field_object[tree15].transform.pos.set(-150.0f, 35.0f, -50.0f);
	m_field_object[tree16].transform.pos.set(-170.0f, 30.0f, 0.0f);
	m_field_object[tree17].transform.pos.set(-150.0f, 30.0f, 70.0f);
	m_field_object[tree18].transform.pos.set(-150.0f, 30.0f, 140.0f);
	m_field_object[tree19].transform.pos.set(-130.0f, 40.0f, 210.0f);
	m_field_object[tree20].transform.pos.set(-150.0f, 40.0f, 280.0f);
	m_field_object[tree21].transform.pos.set(-120.0f, 45.0f, 350.0f);
	m_field_object[tree22].transform.pos.set(-150.0f, 55.0f, 420.0f);
	m_field_object[tree23].transform.pos.set(-150.0f, 65.0f, 490.0f);
	m_field_object[tree24].transform.pos.set(-120.0f, 80.0f, 560.0f);
	m_field_object[tree25].transform.pos.set(-150.0f, 70.0f, 630.0f);
	m_field_object[tree26].transform.pos.set(-130.0f, 60.0f, 700.0f);


	//--------------------------------------------------------------------------
	// �����̐ݒ�
	//--------------------------------------------------------------------------
	// �t�B�[���h�̎���̕���
	m_field_object[tree1].transform.rot.set(0.0f, 15.0f, 0.0f);

	// ����
	m_field_object[tree2].transform.rot.set(0.0f, -50.0f, 0.0f);
	m_field_object[tree3].transform.rot.set(0.0f, 29.0f, 0.0f);
	m_field_object[tree4].transform.rot.set(0.0f, 15.0f, 0.0f);
	m_field_object[tree5].transform.rot.set(0.0f, 16.0f, 0.0f);
	m_field_object[tree6].transform.rot.set(0.0f, 87.0f, 0.0f);
	m_field_object[tree7].transform.rot.set(0.0f, 35.0f, 0.0f);
	m_field_object[tree8].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[tree9].transform.rot.set(0.0f, 16.0f, 0.0f);
	m_field_object[tree10].transform.rot.set(0.0f, 160.0f, 0.0f);
	m_field_object[tree11].transform.rot.set(0.0f, 1.0f, 0.0f);
	m_field_object[tree12].transform.rot.set(0.0f, 173.0f, 0.0f);
	m_field_object[tree13].transform.rot.set(0.0f, 17.0f, 0.0f);
	m_field_object[tree27].transform.rot.set(0.0f, 17.0f, 0.0f);
	// �E��
	m_field_object[tree14].transform.rot.set(0.0f, 15.0f, 0.0f);
	m_field_object[tree15].transform.rot.set(0.0f, -50.0f, 0.0f);
	m_field_object[tree16].transform.rot.set(0.0f, 29.0f, 0.0f);
	m_field_object[tree17].transform.rot.set(0.0f, 15.0f, 0.0f);
	m_field_object[tree18].transform.rot.set(0.0f, 16.0f, 0.0f);
	m_field_object[tree19].transform.rot.set(0.0f, 87.0f, 0.0f);
	m_field_object[tree20].transform.rot.set(0.0f, 35.0f, 0.0f);
	m_field_object[tree21].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[tree22].transform.rot.set(0.0f, -90.0f, 0.0f);
	m_field_object[tree23].transform.rot.set(0.0f, -90.0f, 0.0f);
	m_field_object[tree24].transform.rot.set(0.0f, -90.0f, 0.0f);
	m_field_object[tree25].transform.rot.set(0.0f, 173.0f, 0.0f);
	m_field_object[tree26].transform.rot.set(0.0f, 17.0f, 0.0f);


	//--------------------------------------------------------------------------
	// �T�C�Y�̐ݒ�
	//--------------------------------------------------------------------------
	// �t�B�[���h�̎��蕔��
	m_field_object[tree1].transform.scale.set(1.5f, 1.5f, 1.5f);

	// ����
	m_field_object[tree2].transform.scale.set(0.5f, 0.5f, 0.5f);
	m_field_object[tree3].transform.scale.set(0.5f, 0.3f, 0.5f);
	m_field_object[tree4].transform.scale.set(0.3f, 0.3f, 0.3f);
	m_field_object[tree5].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree6].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree7].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree8].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree9].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree10].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree11].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[tree12].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[tree13].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[tree27].transform.scale.set(0.3f, 0.2f, 0.3f);
	// �E��
	m_field_object[tree14].transform.scale.set(0.5f, 0.5f, 0.5f);
	m_field_object[tree15].transform.scale.set(0.5f, 0.3f, 0.5f);
	m_field_object[tree16].transform.scale.set(0.3f, 0.3f, 0.3f);
	m_field_object[tree17].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree18].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree19].transform.scale.set(0.2f, 0.2f, 0.2f);;
	m_field_object[tree20].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree21].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree22].transform.scale.set(0.2f, 0.2f, 0.2f);
	m_field_object[tree23].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[tree24].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[tree25].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[tree26].transform.scale.set(0.3f, 0.2f, 0.3f);


	//--------------------------------------------------------------------------
	// �����蔻��̐ݒ�
	//--------------------------------------------------------------------------
	m_hit_tree[tree1  ].CreateCapsule(m_field_object[tree1].transform.pos, { 0.0f,50.0f,0.0f }, 100);
	// ����
	m_hit_tree[tree2].CreateCapsule(m_field_object[tree2].transform.pos, { 0.0f,30.0f,0.0f }, 30);
	m_hit_tree[tree3].CreateCapsule(m_field_object[tree3].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree4].CreateCapsule(m_field_object[tree4].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree5].CreateCapsule(m_field_object[tree5].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree6].CreateCapsule(m_field_object[tree6].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree7].CreateCapsule(m_field_object[tree7].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree8].CreateCapsule(m_field_object[tree8].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree9].CreateCapsule(m_field_object[tree9].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree10].CreateCapsule(m_field_object[tree10].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree11].CreateCapsule(m_field_object[tree11].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree12].CreateCapsule(m_field_object[tree12].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree13].CreateCapsule(m_field_object[tree13].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	// �E��
	m_hit_tree[tree14].CreateCapsule(m_field_object[tree14].transform.pos, { 0.0f,30.0f,0.0f }, 30);
	m_hit_tree[tree15].CreateCapsule(m_field_object[tree15].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree16].CreateCapsule(m_field_object[tree16].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree17].CreateCapsule(m_field_object[tree17].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree18].CreateCapsule(m_field_object[tree18].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree19].CreateCapsule(m_field_object[tree19].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree20].CreateCapsule(m_field_object[tree20].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree21].CreateCapsule(m_field_object[tree21].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree22].CreateCapsule(m_field_object[tree22].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree23].CreateCapsule(m_field_object[tree23].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree24].CreateCapsule(m_field_object[tree24].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree25].CreateCapsule(m_field_object[tree25].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree26].CreateCapsule(m_field_object[tree26].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	m_hit_tree[tree27].CreateCapsule(m_field_object[tree27].transform.pos, { 0.0f,30.0f,0.0f }, 10);
	

}

//---------------------------------------------------------------------------
//	�t�F���X�̃I�u�W�F�N�g�̏����ݒ�
//---------------------------------------------------------------------------
void TitleField::FenceInit()
{
	// ���f���f�[�^�̓ǂݍ���
	m_field_object[fence1].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence2].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence3].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence4].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence5].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence6].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence7].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence8].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence9].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence10].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");
	m_field_object[fence11].model.LoadModel("Data/Field/object/fence/Stage_Obj009.mv1");


	// ���W�ݒ�
	m_field_object[fence1].transform.pos.set(120.0f, 80.0f, 750.0f);
	m_field_object[fence2].transform.pos.set(120.0f, 80.0f, 800.0f);
	m_field_object[fence3].transform.pos.set(120.0f, 80.0f, 850.0f);
	m_field_object[fence4].transform.pos.set(-120.0f, 60.0f, 750.0f);
	m_field_object[fence5].transform.pos.set(-120.0f, 55.0f, 800.0f);
	m_field_object[fence6].transform.pos.set(-120.0f, 50.0f, 850.0f);

	m_field_object[fence7].transform.pos.set(100.0f, 80.0f, 870.0f);
	m_field_object[fence8].transform.pos.set(50.0f, 75.0f, 870.0f);
	m_field_object[fence9].transform.pos.set(0.0f, 70.0f, 870.0f);
	m_field_object[fence10].transform.pos.set(-50.0f, 60.0f, 870.0f);
	m_field_object[fence11].transform.pos.set(-100.0f, 50.0f, 870.0f);



	// �����̐ݒ�
	m_field_object[fence1].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence2].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence3].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence4].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence5].transform.rot.set(0.0f, 90.0f, 0.0f);
	m_field_object[fence6].transform.rot.set(0.0f, 90.0f, 0.0f);

	m_field_object[fence7].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence8].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence9].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence10].transform.rot.set(0.0f, 0.0f, 0.0f);
	m_field_object[fence11].transform.rot.set(0.0f, 0.0f, 0.0f);



	// �T�C�Y�̐ݒ�
	m_field_object[fence1].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence2].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence3].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence4].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence5].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence6].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence7].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence8].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence9].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence10].transform.scale.set(0.1f, 0.1f, 0.1f);
	m_field_object[fence11].transform.scale.set(0.1f, 0.1f, 0.1f);



	// �����蔻��̐ݒ�
	m_hit_fence[fence1 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence1].transform.pos, { 5.0f,20.0f,60.0f });
	m_hit_fence[fence2 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence2].transform.pos, { 70.0f, 20.0f,5.0f });
	m_hit_fence[fence3 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence3].transform.pos, { 70.0f, 20.0f,5.0f });
	m_hit_fence[fence4 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence4].transform.pos, { 5.0f,20.0f,40.0f });
	m_hit_fence[fence5 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence5].transform.pos, { 5.0f,20.0f,60.0f });
	m_hit_fence[fence6 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence6].transform.pos, { 5.0f,20.0f,60.0f });
	m_hit_fence[fence7 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence7].transform.pos, { 40.0f,20.0f,5.0f });
	m_hit_fence[fence8 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence8].transform.pos, { 60.0f,20.0f,5.0f });
	m_hit_fence[fence9 - FENCE_INDEX_STRAT].CreateBox(m_field_object[fence9].transform.pos, { 60.0f,20.0f,5.0f });

}

//---------------------------------------------------------------------------
//	�΂̃I�u�W�F�N�g�̏����ݒ�
//---------------------------------------------------------------------------
void TitleField::StoneInit()
{
	//// ���f���f�[�^�̓ǂݍ���
	//m_field_object[stone1].model.LoadModel("Data/Field/object/stone/Stage_Obj002.mv1");
	//m_field_object[stone2].model.LoadModel("Data/Field/object/stone/Stage_Obj002.mv1");
	//m_field_object[stone3].model.LoadModel("Data/Field/object/stone/Stage_Obj002.mv1");
	//m_field_object[stone4].model.LoadModel("Data/Field/object/stone/Stage_Obj002.mv1");
	//m_field_object[stone5].model.LoadModel("Data/Field/object/stone/Stage_Obj002.mv1");

	//// ���W�ݒ�
	//m_field_object[stone1].transform.pos.set(240.0f, 35.0f, 210.0f);
	//m_field_object[stone2].transform.pos.set(240.0f, 35.0f, 57.0f);
	//m_field_object[stone3].transform.pos.set(40.0f, 30.0f, 0.0f);
	//m_field_object[stone4].transform.pos.set(0.0f, 30.0f, 150.0f);
	//m_field_object[stone5].transform.pos.set(150.0f, 40.0f, 240.0f);

	//// �����̐ݒ�
	//m_field_object[stone1].transform.rot.set(0.0f, 90.0f, 0.0f);
	//m_field_object[stone2].transform.rot.set(0.0f, 0.0f, 0.0f);
	//m_field_object[stone3].transform.rot.set(0.0f, 90.0f, 0.0f);
	//m_field_object[stone4].transform.rot.set(0.0f, 90.0f, 0.0f);
	//m_field_object[stone5].transform.rot.set(0.0f, 0.0f, 0.0f);

	//// �T�C�Y�̐ݒ�
	//m_field_object[stone1].transform.scale.set(0.05f, 0.02f, 0.01f);
	//m_field_object[stone2].transform.scale.set(0.03f, 0.015f, 0.03f);
	//m_field_object[stone3].transform.scale.set(0.03f, 0.015f, 0.02f);
	//m_field_object[stone4].transform.scale.set(0.03f, 0.01f, 0.01f);
	//m_field_object[stone5].transform.scale.set(0.03f, 0.015f, 0.01f);

	//// �����蔻��̐ݒ�
	//m_hit_stone[stone1 - STONE_INDEX_STRAT].CreateBox(m_field_object[stone1].transform.pos, { 15.0f,20.0f,60.f });
	//m_hit_stone[stone2 - STONE_INDEX_STRAT].CreateBox(m_field_object[stone2].transform.pos, { 17.0f, 20.0f,87.0f });
	//m_hit_stone[stone3 - STONE_INDEX_STRAT].CreateBox(m_field_object[stone3].transform.pos, { 50.0f , 20.0f,15.0f });
	//m_hit_stone[stone4 - STONE_INDEX_STRAT].CreateBox(m_field_object[stone4].transform.pos, { 10.0f , 20.0f,30.0f });
	//m_hit_stone[stone5 - STONE_INDEX_STRAT].CreateBox(m_field_object[stone5].transform.pos, { 40.0f , 20.0f,10.0f });

}


