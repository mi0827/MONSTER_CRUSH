#include "WinMain.h"
#include "src/System/Random.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"

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
// 各シーンのオブジェクト
Scene_Base* scene;

Option option;
// BGM
Sound m_bgm;


int light_handle;

//! ライトの座標用変数
//-------------------------------------------------------------
// 初期処理
//-------------------------------------------------------------
void GameInit()
{

	// オプションメニューの初期処理
	option.Init();

	// とりあえず今はタイトルシーンをつけておく
	scene = new TitleScene;
	//scene = new QuestAreaScene;
	//scene = new GameScene;

	// キャラクターのせってい
	//scene->SetCharacter(scene->SAMPLEPLAYER,scene->MUTANT);

	scene->Init();

	// 例：陰の部分の明るさを0.5に設定する( デフォルトは 0.33f です )
	SetGlobalAmbientLight(GetColorF(0.2f, 0.2f, 0.2f, 0.0f));

	// BGMをシーンの数用意する
	m_bgm.NewArraySecureSound(scene->scene_max);
	// BGMの初期設定
	m_bgm.LoadSound("Data/BGM/Title.mp3", scene->Title);
	m_bgm.LoadSound("Data/BGM/Story.mp3", scene->Story);
	m_bgm.LoadSound("Data/BGM/questarea.mp3", scene->QuestArea);
	m_bgm.LoadSound("Data/BGM/Battle_1.mp3", scene->Battle);
	m_bgm.LoadSound("Data/BGM/End.mp3", scene->End);

	m_bgm.PlaySound_(scene->Title,DX_PLAYTYPE_BACK,true);


	
}

//-------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------
void GameUpdate()
{
	if (CheckHitKey(KEY_INPUT_RSHIFT))
	{
		delete scene->m_player;
	}

	// オプションメニューが開いていないときだけ
	if (!option.m_option_flag)
	{
		switch (scene->m_now_scene)
		{
		case scene->Title: // タイトルシーン
		

			scene->Update();
			// シーンの変更フラグが立っていれば
			if (scene->m_scene_change_judge) {
				// 次に行ってほしいシーンがクエスト受注エリアなら                     	                                                
				if (scene->m_next_scene == scene->Story)
				{
					scene->Exit();              // delete前に終了処理を回す
					delete scene;               // 現在のシーンの削除
					scene = new StoryScene; // 次のシーンをnewしておく
					scene->Init();              // 次のシーンの初期処理もここで済ます

					ChangeBgm(scene->m_now_scene);
				}
			}
			break;

		case scene->Story:
			scene->Update();
			// シーンの変更フラグが立っていれば
			if (scene->m_scene_change_judge) {
				// 次に行ってほしいシーンがクエスト受注エリアなら                     	                                                
				if (scene->m_next_scene == scene->QuestArea)
				{
					scene->Exit();              // delete前に終了処理を回す
					delete scene;               // 現在のシーンの削除
					scene = new QuestAreaScene; // 次のシーンをnewしておく
					scene->Init();              // 次のシーンの初期処理もここで済ます
					ChangeBgm(scene->m_now_scene);
				}
			}
			break;

		case scene->QuestArea: // クエスト受注エリア
			scene->Update();
			// シーンの変更フラグが立っていれば
			if (scene->m_scene_change_judge)
			{
				// 次に行ってほしいシーンがバトルシーンだったら
				if (scene->m_next_scene == scene->Battle)
				{
					scene->Exit();         // delete前に終了処理を回す
					delete scene;          // 現在のシーンの削除
					scene = new GameScene; // 次のシーンをnewしておく
					scene->Init();         // 次のシーンの初期処理もここで済ます
 					ChangeBgm(scene->m_now_scene);
				}
			}
			break;

		case scene->Battle:  // プレイシーン(バトルシーン)
			scene->Update();
			// シーンの変更フラグが立っていれば
			if (scene->m_scene_change_judge)
			{
				// 次に行ってほしいシーンがエンドシーンだったら
				if (scene->m_next_scene == scene->End)
				{
					scene->Exit();        // delete前に終了処理を回す
					delete scene;         // 現在のシーンの削除
					scene = new EndScene; // 次のシーンをnewしておく
					scene->Init();        // 次のシーンの初期処理もここで済ます
					ChangeBgm(scene->m_now_scene);
				}
			}

			break;

		case scene->End:  // エンドシーン
			scene->Update();
			// シーンの変更フラグが立っていれば
			if (scene->m_scene_change_judge)
			{
				
				// 次に行ってほしいシーンがタイトルシーンだったら
				if (scene->m_next_scene == scene->Title)
				{
					scene->Exit();             // delete前に終了処理を回す
					delete scene;              // 現在のシーンの削除
					scene = new TitleScene;    // 次のシーンをnewしておく
					scene->Init();             // 次のシーンの初期処理もここで済ます
					ChangeBgm(scene->m_now_scene);
				}
				// 次に行ってほしいシーンがクエスト受注シーンだったら　
				if (scene->m_next_scene == scene->QuestArea)
				{
					scene->Exit();              // delete前に終了処理を回す
					delete scene;               // 現在のシーンの削除
					scene = new QuestAreaScene; // 次のシーンをnewしておく
					scene->Init();              // 次のシーンの初期処理もここで済ます
					ChangeBgm(scene->m_now_scene);
				}
			}
			break;
		}
	}
	// BGMが途切れた時は同じのを再生する
	if (m_bgm.PlayingSound() == false)
	{
		m_bgm.PlaySound_(scene->m_now_scene, DX_PLAYTYPE_BACK, true);
	}
	// オプションメニューの更新処理
	option.Update();

	// オプションで変更された値を反映　　
	scene->OptionValuesReflect(
		option.option_menu[option.BGM].m_value,
		option.option_menu[option.SE].m_value,
		option.option_menu[option.MOUSE].m_value);
	// BGMの音量調整
	m_bgm.SetSoundVolume(option.option_menu[option.BGM].m_value);

}


//-------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------
void GameDraw()
{
	// シーンの描画処理
	scene->Draw();
	// エフェクトの描画処理
	DrawEffekseer3D();

	option.Draw();


	scene->BaseDraw(scene->m_now_scene, { 0,200 });


}

// 終了処理
void GameExit()
{
	// シーンの終了処理
	scene->Exit();

	// シーンベースクラスの削除
	delete scene;

}


//----------------------------------------------
// 今のシーンから次のシーンに切り替える関数
//----------------------------------------------
void ChangeBgm(int bgm_num)
{
	// 現在再生中のBGMを止める
	m_bgm.StopSound();
	// 次に再生したいBGMを再生する
	m_bgm.PlaySound_(bgm_num, DX_PLAYTYPE_BACK, true);
}

//----------------------------------------------
// 今のシーンから次のシーンに切り替える関数
//----------------------------------------------
void Scene_Change_Judge(int& now_scene, const int& next_scene)
{
	// 今のシーン番号に次行いたいシーン番号を入れる
	now_scene = next_scene;
}




