#include "src/WinMain.h"
#include "src/System/Vector2.h"
#include "src/System/Vector3.h"
#include "src/System/InfoText.h"
#include "src/System/Transform.h"
#include "src/Model/Model.h"
#include "src/Field/FieldBase.h"


//-------------------------------------------------------------
// �I�u�W�F�N�g���J�����ƃv���C���[�ƃJ�����̊Ԃɂ��邩�𒲂ׂ�֐�
//-------------------------------------------------------------
bool FieldBase::ObjectDrawSituation(Vector3 camera_pos, float camera_radius, Vector3 player_pos, Vector3 obj_pos)
{
	// �J�����̈��͈̔͂ɓ��������̂������̂�����
	float length = GetVector3Distance(camera_pos, obj_pos);
	float camera_length = camera_radius + 50;
	// �J�����̂ƃv���C���[�̊ԂɃI�u�W�F�N�g�����邩�𒲂ׂ�
	if ((obj_pos.x >= camera_pos.x  && obj_pos.x <= player_pos.x) || (camera_pos.x >= obj_pos.x && obj_pos.x >= player_pos.x))
	{
		if (length <= camera_length)
		{
			return false;
		}
	}
	if ((obj_pos.z >= camera_pos.z && obj_pos.z <= player_pos.z) || (camera_pos.z >= obj_pos.z && obj_pos.z >= player_pos.z))
	{
		if (length <= camera_length)
		{
			return false;
		}
	}
	// ���ʂɕ`�悵�Ă悢
	return true;
}
