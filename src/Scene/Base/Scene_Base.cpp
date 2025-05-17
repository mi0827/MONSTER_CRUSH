#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/Transform.h"
#include "src/System/InfoText.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"

#include "src/Action/Combo.h"
#include "src/System/Move.h"

#include "src/Character/CharacterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/TitleField.h"
#include "src/Field/Field.h"

#include "src/System/Text.h"
#include "Scene_Base.h"

Scene_Base::Scene_Base()
{
}

Scene_Base::~Scene_Base()
{
	
}

// --------------------------------------------------------------------------
// �ǂ̃L�����N�^�[���g�����̐ݒ�
//---------------------------------------------------------------------------
void Scene_Base::SetCharacter(int player_num, int monster_num)
{
	// �L�����N�^�[�ԍ��̕ۑ�
	m_player_num = player_num;
	m_monster_num = monster_num;
}

// --------------------------------------------------------------------------
// �S�V�[���Ŏg�����̏�����
//---------------------------------------------------------------------------
void Scene_Base::BaseInit()
{
	// �e�L�X�g�f�[�^�̓ǂݍ���
	m_text.LoadText("Data/Text/Option.txt", TEXT_MAX);

	// ��̐ݒ�
	// ��f�[�^�̓ǂݍ���
	m_sky_model.LoadModel("Data/Model/Sky/sky.mqoz");

	m_sky_transform.pos.set(0.0f, 500.0f, 0.0f);
	m_sky_transform.rot.set(0.0f, 0.0f, 0.0f);
	m_sky_transform.scale.set(20.0f, 20.0f, 20.0f);
}

// --------------------------------------------------------------------------
// �S�V�[���Ŏg�����̂̕`��
//---------------------------------------------------------------------------
void Scene_Base::BaseDraw(int scene_num, Vector2 draw_pos_)
{
	// �����炽�ȃt�H���g�T�C�Y�����Ă�
	SetFontSize(30);
	float h = GetFontSize();
	// �`�悷����W��ݒ�
	Vector2 draw_pos = { draw_pos_.x,draw_pos_.y };
	// �e�L�X�g�̕`��
	// �Q�[���p�b�h���ڑ�����Ă��邩�ŕύX����
	if (GetJoypadNum() >= 1)
	{
		m_text.TextDraw(pad_menu_text, draw_pos, m_text.OPTION_BACK_SIZE);
		if (scene_num == Battle)
		{
			draw_pos = { draw_pos_.x, draw_pos_.y + h * 2 + 10 };
			m_text.TextDraw(pad_target_camera_text, draw_pos, m_text.OPTION_BACK_SIZE);
		}
	}
	else
	{
		m_text.TextDraw(menu_text, draw_pos, m_text.OPTION_BACK_SIZE);
		if (scene_num == Battle)
		{
			draw_pos = { draw_pos_.x, draw_pos_.y + h * 2 + 10 };
			m_text.TextDraw(target_camera_text, draw_pos, m_text.OPTION_BACK_SIZE);
		}
	}
}

// --------------------------------------------------------------------------
// ��̕`��
// --------------------------------------------------------------------------
void Scene_Base::SkyDraw()
{
	// ������f���̓��C�g�̉e���͂Ȃ���Ԃɂ�����
	SetUseLighting(FALSE);
	m_sky_model.DrawModel(&m_sky_transform);
	// ���C�g�����ɖ߂�
	SetUseLighting(TRUE);
}

// --------------------------------------------------------------------------
// �V���h�[�}�b�v�̏�������
// --------------------------------------------------------------------------
void Scene_Base::ShadowMapInit()
{
	// �V���h�[�}�b�v�̍쐬
	m_shadowMap_handle_1 = MakeShadowMap(4096, 4096);
	m_shadowMap_handle_2 = MakeShadowMap(4096, 4096);
	m_shadowMap_handle = MakeShadowMap(4096, 4096);
	// ���C�g�̊p�x�̏�����
	m_light_angle = 10.0f;

	// ���C�g�̕����x�N�g���̎Z�o
	m_light_direction.x = sin(m_light_angle);
	m_light_direction.z = cos(m_light_angle);
	m_light_direction.y = -1.0f;
	VectorNormalize(&m_light_direction, &m_light_direction);

	// ���C�g������ݒ�
	SetLightDirection(m_light_direction);

	// �V���h�[�}�b�v���z�肷�郉�C�g�̕����̃Z�b�g
	SetShadowMapLightDirection(m_shadowMap_handle_1, m_light_direction);
	SetShadowMapLightDirection(m_shadowMap_handle_2, m_light_direction);
	SetShadowMapLightDirection(m_shadowMap_handle, m_light_direction);
	// �V���h�[�}�b�v�ɕ`�悷��͈͂̐ݒ�
	SetShadowMapDrawArea(m_shadowMap_handle, VGet(-8192.0f, -0.1f, -8192.0f), VGet(+8192.0f, 3000.0f, +8192.0f));

}

// --------------------------------------------------------------------------
// �v���C���[�̃V���h�[�}�b�v�͈̔͐ݒ�
// --------------------------------------------------------------------------
void Scene_Base::SetShadowMapArea(int shadowMap_handle, Vector3 player_pos)
{
	// �V���h�[�}�b�v�ɕ`�悷��͈͂̐ݒ�
	// ���͂��́@�͈͂Ńv���C���[�̃V���h�E�}�b�v��ݒ肵�Ă��邪
	// �w��̉e���C�ɓ���Ȃ���Γ�ڂ�Y���W�̒l���グ�邩�S�̓I�ɕ`��͈͂��L����
	SetShadowMapDrawArea(shadowMap_handle,
		VGet(player_pos.x - 50.0f, -0.1f, player_pos.z - 50.0f),
		VGet(player_pos.x + 50.0f, +100.0f, player_pos.z + 50.0f));
}

// --------------------------------------------------------------------------
// �`��Ɏg�p����V���h�E�}�b�v�̐ݒ������
// --------------------------------------------------------------------------
void Scene_Base::UseShadowMapSet()
{
	SetUseShadowMap(1, -1);
	SetUseShadowMap(0, -1);
}

// --------------------------------------------------------------------------
// �V���h�[�}�b�v�̍폜
// --------------------------------------------------------------------------
void Scene_Base::ExitShadowMap()
{
	DeleteShadowMap(m_shadowMap_handle_1);
	DeleteShadowMap(m_shadowMap_handle_2);
	DeleteShadowMap(m_shadowMap_handle);
}

// --------------------------------------------------------------------------
// ��������X�N���[�����W�ɕ`�悷�邽�߂̒l��Ԃ��֐�
// --------------------------------------------------------------------------
VECTOR Scene_Base::DrawStringWorld(Vector3 pos, Vector3 shift_pos)
{
	// �ݒ肳�ꂽ���W�ɕ������`�悷�邽�߂̏���
	// �RD��ԍ��W���QD�̉�ʍ��W�ɕϊ�
	VECTOR wpos = pos.VGet();

	// ���炵���������炷
	wpos.x += shift_pos.x;
	wpos.y += shift_pos.y;

	// ���̍��W�iwpos�j���֐��ɓn���ăX�N���[�����W�i�QD��ʍ��W : pos2d�j���擾
	VECTOR pos2d = ConvWorldPosToScreenPos(wpos);

	return pos2d;
}

// --------------------------------------------------------------------------
// ���ɍs�������V�[�����Z�b�g����֐�
// --------------------------------------------------------------------------
void Scene_Base::SetNextScene(int next_scene)
{
	// ���ɍs�������V�[���̕ۑ�
	m_next_scene = next_scene;
	// ���ɍs�������V�[�������݂̃V�[���ɓ���ւ���
	m_now_scene = next_scene;
}

// --------------------------------------------------------------------------
// �t�F�[�h�A�E�g���邽�߂̏���
// --------------------------------------------------------------------------
void Scene_Base::FadeOutUpdate()
{
	// �t���[���J�E���g���w��̒l�ȏ�ɂȂ�����
	if (m_frame_count >= FLAME_MAX)
	{
		// �J�E���g�����Z�b�g
		m_frame_count = 0;
		// �t�F�[�h�A�E�g�̎��̓t�F�[�h�C���̃^�[���ɂȂ�
		m_turn = Main;
	}

	// �t���[���J�E���g��f���₷
	m_frame_count++;

	// ��b�łǂꂾ���̒l�ω����邩�̊������o��
	m_fade_ratio = 255 / FLAME_MAX;
	// ����l�̗ʂ��o��
	m_fade_value = m_fade_ratio * m_frame_count;
}


// --------------------------------------------------------------------------
// �t�F�[�h�A�E�g������ɃV�[���̐؂�ւ������邽�߂̏���
// --------------------------------------------------------------------------
void Scene_Base::FadeOutSceneChange(int next_scene)
{
	// �t�F�[�h�A�E�g�֐��̒��ŃJ�E���g���ꂽ�J�E���g��
	// �w��̒l�ȏ�ɂȂ�����V�[���̕ς���
	if (m_frame_count >= FLAME_MAX)
	{
		// ���ɍs�������V�[���̐ݒ�
		SetNextScene(next_scene);
		// �V�[���ύX�t���O�𗧂Ă�
		m_scene_change_judge = true;
	}
	// �t�F�[�h�A�E�g����
	FadeOutUpdate();
}

// --------------------------------------------------------------------------
// �t�F�[�h�C�����邽�߂̏���
// --------------------------------------------------------------------------
void Scene_Base::FadeInUpdate()
{
	// �t���[���J�E���g���w��̒l�ȏ�ɂȂ�����
	if (m_frame_count >= FLAME_MAX)
	{
		// �J�E���g�����Z�b�g
		m_frame_count = 0;
		// �t�F�[�h�C���̌�̓��C���̃^�[���ɂȂ�
		m_turn = Main;
	}

	// �t���[���J�E���g��f���₷
	m_frame_count++;

	// ��b�łǂꂾ���̒l�ω����邩�̊������o��
	m_fade_ratio = 255 / FLAME_MAX;
	// ����l�̗ʂ��o��
	m_fade_value = m_fade_ratio * m_frame_count;
}

// --------------------------------------------------------------------------
// �t�F�[�h�̕`�揈��
// --------------------------------------------------------------------------
void Scene_Base::FadeDraw()
{
	// �t�F�[�h�A�E�g�͂��܂������Ă��邪�t�F�[�h�C�������܂������Ă��Ȃ��̂��Ɏ��Ԃ�����΍Ē��킵�Ă݂�
	switch (m_turn)
	{
		//case FadeIn:
		// �����x�̕ύX
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA,  255 - m_fade_value);
		//// �����ǂ̂̕`��
		//DrawBox(0, 0, SCREEN_W, SCREEN_H, 0, TRUE);
		//// �Â��̕ύX
		////SetDrawBright( m_fade_value,  m_fade_value,  m_fade_value);
		//break;
	case Main:
		// �����x�̕ύX
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		// �Â��̕ύX
		SetDrawBright(255, 255, 255);
		break;

	case FadeOut:
		// �����x�̕ύX
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fade_value);
		// �����ǂ̂̕`��
		DrawBox(0, 0, SCREEN_W, SCREEN_H, 0, TRUE);
		// �Â��̕ύX
		SetDrawBright(255 - m_fade_value, 255 - m_fade_value, 255 - m_fade_value);
		break;
	}

}

// --------------------------------------------------------------------------
// �v���C���[�̓ǂݍ��ݏ���
// --------------------------------------------------------------------------
void Scene_Base::PlayerInit(int character_num)
{
	switch (m_character_num)
	{
	case hero: // �����m
		m_player = new Hero;
		break;
	case bot: // �{�b�g��m
		//m_player = new SamplePlayer;
		break;
	}
}

// --------------------------------------------------------------------------
// �󂯂��N�G�X�g�ɂ���Đ키�����X�^�[�̔ԍ���Ԃ��p�̊֐�
// --------------------------------------------------------------------------
int Scene_Base::ReturnMonsterNum()
{
	return m_monster_num;
}

// --------------------------------------------------------------------------
//  �N�G�X�g�Ŏ󂯂��Ă��ԍ��ɂ���ă����X�^�[��o�ꂳ���邽�߂̂���
// --------------------------------------------------------------------------
void Scene_Base::SetMonster(int monster_num)
{
	m_monster_num = monster_num;
}

