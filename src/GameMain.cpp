#include "WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Vector2.h"

#include "src/Action/Combo.h"
#include "src/System/Transform.h"
#include "src/System/InputPad.h"

#include "src/Model/Model.h"
#include "src/Animation/Animation.h"
#include "src/Effect/Effect.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/Hit/Hit.h"

#include "src/System/Move.h"
#include "src/System/TargetMove.h"

#include "src/System/UIBar.h"
#include "src/Character/CharacterBase.h"
#include "src/Character/MonsterBase.h"
#include "src/Character/SamplePlayer.h"
#include "src/Character/Hero.h"
#include "src/Character/Monster.h"
#include "src/Character/Mutant.h"

#include "src/Field/Field.h"

#include "src/System/DamageAction.h"
#include "src/Action/Attack.h"

#include "src/Camera.h"

#include "src/Scene/Base/Scene_Base.h"
#include "src/Scene/TitleScene.h"
#include "src/Scene/GameScene.h"
#include "src/Scene/EndScene.h"

#include "GameMain.h"

// 各シーンのオブジェクト
Scene_Base* scene;


// ライトとシャドーマップの設定はゲームメインで行う


// 影に使うやつ
//! プレイヤー用のシャドウマップ
//int player_shadowmap_hanndle;
////! シャドーマップ用の変数
//int ShadowMapHandle;
//float Angle;
//VECTOR LightDirection;


int light_handle;

//! ライトの座標用変数

int scene_num = 0; // 今どのシーン名のを見る用の変数
// 各シーンでの使い分けをするためのシーンの列挙隊
enum Scene
{
	Titele, // タイトル
	Play,  // メインのプレイシーン
	End,   // エンド

	Scene_Max // シーンの最大数
};


//-------------------------------------------------------------
// 初期処理
//-------------------------------------------------------------
void GameInit()
{
	// とりあえず今はタイトルシーンをつけておく
	scene = new TitleScene;

	// キャラクターのせってい
	//scene->SetCharacter(scene->SAMPLEPLAYER,scene->MUTANT);

	scene->Init();

	// デフォルトのライトは平行光源のまま少し暗めにします
	//SetLightDifColor(GetColorF(0.2f, 0.2f, 0.2f, 1.0f));
	//SetLightEnable(FALSE);

	// デフォルトカラーをもらってくる
	COLOR_F color = GetLightDifColor();
	// もう一つのライトの作成
	// 今使うとまぶしすぎる
	// キャラクターにだけ背後からのライトからの影響を受けない
	Vector3 light_pos = { 1000.0f, 100.0f, 0.0f };
	light_handle = CreateDirLightHandle(VGet(0.0f, 0.0f, 1.0f));
	// ライトのポジションの設定
	SetLightPositionHandle(light_handle, light_pos.VGet());
	// 色の設定
	SetLightDifColorHandle(light_handle, color);

	
}

//-------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------
void GameUpdate()
{
	switch (scene_num)
	{
	case Titele: // タイトルシーン

		scene->Update();
		if (scene->m_scene_change_judge) {                             // シーンの切り替えの許可が下りれば
			scene->Exit();                                           // dekete前に終了処理を回す
			Scene_Change_Judge(scene_num, Play);                     // シーンの切り替え
			delete scene;                                            // シーンの切り替えの前にタイトルシーンを初期化
			scene = new GameScene;                                   // 次のシーンをnewしておく
			scene->Init();                                           // 次のシーンの初期処理もここで済ます
		}
		break;

	case Play:  // プレイシーン
		scene->Update();
		if (scene->m_scene_change_judge) {                              // シーンの切り替えの許可が下りれば
			scene->Exit();                                            // dekete前に終了処理を回す
			Scene_Change_Judge(scene_num, End);  // シーンの切り替え	                                                        
			delete scene;                                            // シーンの切り替えの前にタイトルシーンを初期化
			scene = new EndScene;                                    // 次のシーンをnewしておく
			scene->Init();                                           // 次のシーンの初期処理もここで済ます
		}
		break;
	case End:  // エンドシーン
		scene->Update();
		if (scene->m_scene_change_judge) {                             // シーンの切り替えの許可が下りれば
			scene->Exit();                                           // dekete前に終了処理を回す
			Scene_Change_Judge(scene_num, Titele);                   // シーンの切り替え
			delete scene;                                            // シーンの切り替えの前にタイトルシーンを初期化
			scene = new TitleScene;                                 // 次のシーンをnewしておく
			scene->Init();                                           // 次のシーンの初期処理もここで済ます
		}
		break;
	}

	// ３：子の変数の値をシェーダーに渡します
	//SetPSConstF(25, shader_base_pos);
}

//-------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------
void GameDraw()
{

	// シーンの描画処理
	scene->Draw();

	////	シェーダーを使って描画します
	//MV1SetUseOrigShader(TRUE);
	////	頂点シェーダーのセット
	//SetUseVertexShader(vertex_shader);
	////	ピクセルシェーダーのセット
	//SetUsePixelShader(pixel_shader);

}

// 終了処理
void GameExit()
{
	////	シェーダーファイルの終了処理
	//DeleteShader(vertex_shader);
	//DeleteShader(pixel_shader);

	// シーンの終了処理
	scene->Exit();
	
	// ライトの削除
	//DeleteLightHandle(light_handle);
	
}

//----------------------------------------------
// 今のシーンから次のシーンに切り替える関数
//----------------------------------------------
void Scene_Change_Judge(int& now_scene, const int& next_scene)
{
	// 今のシーン番号に次行いたいシーン番号を入れる
	now_scene = next_scene;
}




