#include "WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"
#include "src/System/InfoText.h"

#include "src/System/Transform.h"
#include "src/System/InputPad.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"
#include "src/Hit/HitStop.h"

#include "src/System/UIBar.h"
#include "src/Effect/Effect.h"
#include "src/Sound/Sound.h"
#include "src/Camera.h"


#include "src/System/Move.h"
#include "src/System/TargetMove.h"
#include "src/Action/Combo.h"

#include "src/Character/CharacterBase.h"
#include "src/Character/MonsterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"
#include "src/Character/Monster.h"
#include "src/Character/Mutant.h"
#include "src/Character/Receptionist.h"
#include "src/Character/Bot.h"

#include "src/Field/FieldBase.h"
#include "src/Field/HitField.h" 
#include "src/Field/Field.h"
#include "src/Field/TitleField.h"

#include "src/System/DamageAction.h"
#include "src/Action/Attack.h"

#include <fstream>
#include <string>
#include "src/System/Text.h"

#include "src/Scene/Base/Scene_Base.h"
#include "src/Scene/TitleScene.h"
#include "src/Scene/StoryScene.h"
#include "src/Scene/QuestAreaScene.h"
#include "src/Scene/GameScene.h"
#include "src/Scene/EndScene.h"

#include "src/System/Option.h"

#include "GameMain.h"
using namespace std;
// �e�V�[���̃I�u�W�F�N�g
Scene_Base* scene;

Option option;
// BGM
Sound m_bgm;


int light_handle;

//! ���C�g�̍��W�p�ϐ�
//-------------------------------------------------------------
// ��������
//-------------------------------------------------------------
void GameInit()
{

	// �I�v�V�������j���[�̏�������
	option.Init();

	// �Ƃ肠�������̓^�C�g���V�[�������Ă���
	// scene = new EndScene;
	//scene = new TitleScene;
	// scene = new QuestAreaScene;
	scene = new GameScene;

	// �L�����N�^�[�̂����Ă�
	//scene->SetCharacter(scene->SAMPLEPLAYER,scene->MUTANT);

	scene->Init();

	// ��F�A�̕����̖��邳��0.5�ɐݒ肷��( �f�t�H���g�� 0.33f �ł� )
	SetGlobalAmbientLight(GetColorF(0.2f, 0.2f, 0.2f, 0.0f));

	// BGM���V�[���̐��p�ӂ���
	m_bgm.NewArraySecureSound(scene->scene_max);
	// BGM�̏����ݒ�
	m_bgm.LoadSound("Data/BGM/Title.mp3", scene->Title);
	m_bgm.LoadSound("Data/BGM/Story.mp3", scene->Story);
	m_bgm.LoadSound("Data/BGM/questarea.mp3", scene->QuestArea);
	m_bgm.LoadSound("Data/BGM/Battle_1.mp3", scene->Battle);
	m_bgm.LoadSound("Data/BGM/End.mp3", scene->End);

	m_bgm.PlaySound_(scene->Title, DX_PLAYTYPE_BACK, true);
}

//-------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------
void GameUpdate()
{
	if (CheckHitKey(KEY_INPUT_RSHIFT))
	{
		delete scene->m_player;
	}

	// �I�v�V�������j���[���J���Ă��Ȃ��Ƃ�����
	if (!option.m_option_flag)
	{
		switch (scene->m_now_scene)
		{
		case scene->Title: // �^�C�g���V�[��


			scene->Update();
			// �V�[���̕ύX�t���O�������Ă����
			if (scene->m_scene_change_judge) {
				// ���ɍs���Ăق����V�[�����N�G�X�g�󒍃G���A�Ȃ�                     	                                                
				if (scene->m_next_scene == scene->Story)
				{
					scene->Exit();              // delete�O�ɏI����������
					delete scene;               // ���݂̃V�[���̍폜
					scene = new StoryScene; // ���̃V�[����new���Ă���
					scene->Init();              // ���̃V�[���̏��������������ōς܂�

					ChangeBgm(scene->m_now_scene);
				}
			}
			break;

		case scene->Story:
			scene->Update();
			// �V�[���̕ύX�t���O�������Ă����
			if (scene->m_scene_change_judge) {
				// ���ɍs���Ăق����V�[�����N�G�X�g�󒍃G���A�Ȃ�                     	                                                
				if (scene->m_next_scene == scene->QuestArea)
				{
					scene->Exit();              // delete�O�ɏI����������
					delete scene;               // ���݂̃V�[���̍폜
					scene = new QuestAreaScene; // ���̃V�[����new���Ă���
					scene->Init();              // ���̃V�[���̏��������������ōς܂�
					ChangeBgm(scene->m_now_scene);
				}
			}
			break;

		case scene->QuestArea: // �N�G�X�g�󒍃G���A
			scene->Update();
			// �V�[���̕ύX�t���O�������Ă����
			if (scene->m_scene_change_judge)
			{
				// ���ɍs���Ăق����V�[�����o�g���V�[����������
				if (scene->m_next_scene == scene->Battle)
				{
					scene->Exit();         // delete�O�ɏI����������
					delete scene;          // ���݂̃V�[���̍폜
					scene = new GameScene; // ���̃V�[����new���Ă���
					scene->Init();         // ���̃V�[���̏��������������ōς܂�
					ChangeBgm(scene->m_now_scene);
				}
			}
			break;

		case scene->Battle:  // �v���C�V�[��(�o�g���V�[��)
			scene->Update();
			// �V�[���̕ύX�t���O�������Ă����
			if (scene->m_scene_change_judge)
			{
				// ���ɍs���Ăق����V�[�����G���h�V�[����������
				if (scene->m_next_scene == scene->End)
				{
					scene->Exit();        // delete�O�ɏI����������
					delete scene;         // ���݂̃V�[���̍폜
					scene = new EndScene; // ���̃V�[����new���Ă���
					scene->Init();        // ���̃V�[���̏��������������ōς܂�
					ChangeBgm(scene->m_now_scene);
				}
			}

			break;

		case scene->End:  // �G���h�V�[��
			scene->Update();
			// �V�[���̕ύX�t���O�������Ă����
			if (scene->m_scene_change_judge)
			{

				// ���ɍs���Ăق����V�[�����^�C�g���V�[����������
				if (scene->m_next_scene == scene->Title)
				{
					scene->Exit();             // delete�O�ɏI����������
					delete scene;              // ���݂̃V�[���̍폜
					scene = new TitleScene;    // ���̃V�[����new���Ă���
					scene->Init();             // ���̃V�[���̏��������������ōς܂�
					ChangeBgm(scene->m_now_scene);
				}
				// ���ɍs���Ăق����V�[�����N�G�X�g�󒍃V�[����������@
				if (scene->m_next_scene == scene->QuestArea)
				{
					scene->Exit();              // delete�O�ɏI����������
					delete scene;               // ���݂̃V�[���̍폜
					scene = new QuestAreaScene; // ���̃V�[����new���Ă���
					scene->Init();              // ���̃V�[���̏��������������ōς܂�
					ChangeBgm(scene->m_now_scene);
				}
			}
			break;
		}
	}
	// BGM���r�؂ꂽ���͓����̂��Đ�����
	if (m_bgm.PlayingSound() == false)
	{
		m_bgm.PlaySound_(scene->m_now_scene, DX_PLAYTYPE_BACK, true);
	}
	// �I�v�V�������j���[�̍X�V����
	option.Update();

	// �I�v�V�����ŕύX���ꂽ�l�𔽉f�@�@
	scene->OptionValuesReflect(
		option.option_menu[option.BGM].m_value,
		option.option_menu[option.SE].m_value,
		option.option_menu[option.MOUSE].m_value);
	// BGM�̉��ʒ���
	m_bgm.SetSoundVolume(option.option_menu[option.BGM].m_value);

}


//-------------------------------------------------------------
// �`�揈��
//-------------------------------------------------------------
void GameDraw()
{
	// �V�[���̕`�揈��
	scene->Draw();
	// �G�t�F�N�g�̕`�揈��
	DrawEffekseer3D();

	option.Draw();


	scene->BaseDraw(scene->m_now_scene, { 0,200 });


}

// �I������
void GameExit()
{
	// �V�[���̏I������
	scene->Exit();

	// �V�[���x�[�X�N���X�̍폜
	delete scene;

}


//----------------------------------------------
// ���̃V�[�����玟�̃V�[���ɐ؂�ւ���֐�
//----------------------------------------------
void ChangeBgm(int bgm_num)
{
	// ���ݍĐ�����BGM���~�߂�
	m_bgm.StopSound();
	// ���ɍĐ�������BGM���Đ�����
	m_bgm.PlaySound_(bgm_num, DX_PLAYTYPE_BACK, true);
}

//----------------------------------------------
// ���̃V�[�����玟�̃V�[���ɐ؂�ւ���֐�
//----------------------------------------------
void Scene_Change_Judge(int& now_scene, const int& next_scene)
{
	// ���̃V�[���ԍ��Ɏ��s�������V�[���ԍ�������
	now_scene = next_scene;
}




