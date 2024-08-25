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

// aisdjla
//aaaaaa

// プルできるかの実験


//! カメラクラスのオブジェクト
Camera camera;

//! サンプルプレイヤークラスのオブジェクト
SamplePlayer player;
//! ヒーロークラスのオブジェクト
//Hero hero;
//! モンスタークラスのオブジェクト
//Monster monster;
Mutant mutant;

//! 攻撃アクションの当たり判定用のオブジェクト
// このクラスをただの関数でもよかったけど中にbool 型の変数が一つあるのでクラスにした
Attack player_attack_hit;
Attack monster_attack_hit;


// シェーダーの基準の座標となる用の変数
FLOAT4 shader_base_pos_;
//	頂点シェーダー用の変数
int vertex_shader = 0;
//	ピクセルシェーダー用の変数
int pixel_shader = 0;

// 仮のボックスの描画
BoxCollision box1;

// 仮の地面モデルの入れ物
int ground = 0;


// ライトとシャドーマップの設定はゲームメインで行う


// 影に使うやつ
//! プレイヤー用のシャドウマップ
int player_shadowmap_hanndle;
//! シャドーマップ用の変数
int ShadowMapHandle;
float Angle;
VECTOR LightDirection;


int light_handle;
int light_handle_2;
//! ライトの座標用変数

// フィールドオブジェクト
Field field;


//-------------------------------------------------------------
// 初期処理
//-------------------------------------------------------------
void GameInit()
{
	//// シェーダーファイルの読み込み（ＤＸライブラリ用に変換されたファイル）
	//vertex_shader = LoadVertexShader( "shader/SampleVS.vso");	//	頂点シェーダー

	//pixel_shader = LoadPixelShader( "shader/SamplePS.pso" );    //	ピクセルシェーダー

	// カメラの初期設定
	camera.PlayField_Init();

	// プレイヤーの初期処理
	player.Init();

	// ヒーローの初期処理
	//hero.Init();
	// モンスターの初期処理
	//monster.Init();
	mutant.Init();
	// 地面モデルのも見込み
	ground = MV1LoadModel("Data/ground.mqoz");
	// 仮のボックス
	Vector3 pos = { 10.0f,0.0f,10.0f };
	Vector3 size = { 10.0f,10.0f,10.0f };
	box1.CreateBox(pos, size);

	// シャドーマップの作成
	player_shadowmap_hanndle = MakeShadowMap(4096, 4096);
	ShadowMapHandle = MakeShadowMap(4096, 4096);
	//	ShadowMapHandle = MakeShadowMap(16384, 16384);

		// ライトの角度を初期化
	Angle = 10.0f;

	// ライトの方向ベクトルの算出
	LightDirection.x = sin(Angle);
	LightDirection.z = cos(Angle);
	LightDirection.y = -1.0f;
	VectorNormalize(&LightDirection, &LightDirection);

	// ライトの方向を設定
	SetLightDirection(LightDirection);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(player_shadowmap_hanndle, LightDirection);
	SetShadowMapLightDirection(ShadowMapHandle, LightDirection);
	// シャドーマップに描画する範囲の設定
	SetShadowMapDrawArea(ShadowMapHandle, VGet(-2000.0f, -0.1f, -2000.0f), VGet(+2000.0f, 3000.0f, +2000.0f));


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

	// フィールドの初期化
	field.Init();
}

//-------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------
void GameUpdate()
{



	// プレイヤーの更新処理
	player.Update(&camera.m_rot);

	// ヒーローの更新処理
	//hero.Update(&camera.m_rot);

	// モンスターの更新処理
	// 今はプレイヤーと同じ更新処理だがモンスター独自の更新処理も追加する
	//monster.Update(&player.m_transform, player.m_hit_r);
	mutant.Update(&player.m_transform, player.m_hit_r);
	// カメラの更新処理
	camera.Update(&player.m_transform.pos);

	// プレイヤーとボックスの当たり判定があったときだけ壁すりをしてほしい
	if (CheckBoxHit3D(player.m_transform.pos, player.m_move_hit_size, box1.m_box.hit_pos, box1.m_box.half_size))
	{
		player.MoveHitUpdate( &box1);

	}
	// モンスターとプレイヤーの移動の当たり判定
	if (CheckCapsuleHit(mutant.m_body, player.m_body))
	{
		// 当たっていたら
		player.m_move.Move_Hit_Capsule(&player.m_transform.pos, player.m_body.m_capsule.radius,
			&mutant.m_body);
		//	Damage_Count(10, 5, &player.m_hp_value);
	}

	AttackUpdate();




	//HitAttack(monster.m_body, player.m_body, monster.m_animation);

	printfDx("HP:%3d ", mutant.m_hp_value);

	// ３：子の変数の値をシェーダーに渡します
	//SetPSConstF(25, shader_base_pos);
}

//-------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------
void GameDraw()
{


	////	シェーダーを使って描画します
	//MV1SetUseOrigShader(TRUE);
	////	頂点シェーダーのセット
	//SetUseVertexShader(vertex_shader);
	////	ピクセルシェーダーのセット
	//SetUsePixelShader(pixel_shader);


	// シャドーマップに描画する範囲の設定
	// 今はこの　範囲でプレイヤーのシャドウマップを設定しているが
	// 背後の影が気に入らなければ二つ目のY座標の値を上げるか全体的に描画範囲を広げろ
	SetShadowMapDrawArea(player_shadowmap_hanndle,
		VGet(player.m_transform.pos.x - 200.0f, -0.1f, player.m_transform.pos.z - 200.0f),
		VGet(player.m_transform.pos.x + 200.0f, +1500.0f, player.m_transform.pos.z + 200.0f));


	// 仮ボックスの描画
	box1.Draw(255, 100);


	//-------------------------------------------------------------
	// シャドウマップの作成（ここで各オブジェクトのシャドーマップの設定）
	//-------------------------------------------------------------
	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(player_shadowmap_hanndle);
	{
		// プレイヤーの描画処理
		player.Draw();

		// ヒーローの描画処理
		//hero.Draw();
	}
	ShadowMap_DrawSetup(ShadowMapHandle);
	{
		// シャドウマップへキャラクターモデルの描画
		//MV1SetPosition(ground, VGet(0.0f, 0.0f, 0.0f)); // 描画するプレイヤーモデルの座標の設定
		//MV1SetRotationXYZ(ground, VGet(TO_RADIAN(0.0f), TO_RADIAN(0.0f), TO_RADIAN(0.0f))); // モデルの回転
		//MV1SetScale(ground, VGet(10, 10, 10)); // モデルの大きさ(10分の１のサイズ)
		//MV1DrawModel(ground); // モデルの描画
		field.Draw();
		// モンスターの描画
		mutant.Draw();
	}

	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	//-------------------------------------------------------------
	// 各モデルの描画
	//-------------------------------------------------------------

	// バックバッファに描画する
	SetDrawScreen(DX_SCREEN_BACK);

	// カメラの描画処理
	camera.Draw();


	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(1, player_shadowmap_hanndle);
	{
		player.Draw();

		// ヒーローの描画処理
		//hero.Draw();
	}
	SetUseShadowMap(0, ShadowMapHandle);
	{
		// シャドウマップへキャラクターモデルの描画

		//MV1SetPosition(ground, VGet(0.0f, 0.0f, 0.0f)); // 描画するプレイヤーモデルの座標の設定
		//MV1SetRotationXYZ(ground, VGet(TO_RADIAN(0.0f), TO_RADIAN(0.0f), TO_RADIAN(0.0f))); // モデルの回転
		//MV1SetScale(ground, VGet(10, 10, 10)); // モデルの大きさ(10分の１のサイズ)
		//MV1DrawModel(ground); // モデルの描画
		field.Draw();
		// モンスターの描画
		mutant.Draw();
	}

	// フラグがっているかを確認するためのもの（最後に消す）
	printfDx("run:%d ", player.m_now_attack);

	// 攻撃時の当たり判定がうまく一定ないので可視化しておく
	printfDx("playerhit:%d ", player_attack_hit.hit);
	printfDx("monsterhit:%d ", monster_attack_hit.hit);
	//printfDx("idle:%d ", player.m_idle_flag);
	//printfDx("rolling:%d ", player.m_rolling_flag);
	//printfDx("attack:%d ", player.m_attack_flag);
	//printfDx("combo_flag:%d ", player.m_combo_flag);

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(1, -1);
	SetUseShadowMap(0, -1);
	
	// ステータスバーの描画
	StatusDraw();
}

// 終了処理
void GameExit()
{
	////	シェーダーファイルの終了処理
	//DeleteShader(vertex_shader);
	//DeleteShader(pixel_shader);

	// シャドーマップの削除
	DeleteShadowMap(player_shadowmap_hanndle);
	DeleteShadowMap(ShadowMapHandle);
	// ライトの削除
	//DeleteLightHandle(light_handle);
	//DeleteLightHandle(light_handle_2);
}

//-------------------------------------------------------------
// ステータスバーの描画
//-------------------------------------------------------------
void StatusDraw()
{
	player.Status_Bar_Draw();
	mutant.Status_Bar_Draw();
}

//-------------------------------------------------------------
// 攻撃関係の更新処理
//-------------------------------------------------------------
void AttackUpdate()
{
	// プレイヤーの攻撃
	if (player.m_attack_flag)
	{
		// 一つのアニメーションの間に一回だけ当たったらtrueを返すようにするようにしたいがうまくいかない
		// 詳しくは関数の中に書く

		// playerの攻撃の時に取りたい当たり判定とモンスターの体との当たり判定をとる
		// 当たり判定がうまく一定ないのはこの関数の中身のせい
		if (player_attack_hit.HitAttack(mutant.m_body, player->m_attack_hit_damage[player.m_now_attack].m_attack_hit, player.m_animation) == true)
		{
			// ダメージを入れるのは攻撃アニメーションの間に一回だけ
			Damage_Count(player->m_attack_hit_damage[player.m_now_attack].m_attack_damage, 5, &mutant.m_hp_value);
		}
	}


	//モンスターの攻撃
	if (mutant.m_attack_flag)
	{
		// モンスターの攻撃時に使いたい当たり判定とplayerの体との当たり判定
		if (monster_attack_hit.HitAttack(mutant.attack_hit_damage[mutant.m_now_attack].m_attack_hit, player.m_body, player.m_animation) == true)
		{
			// ダメージを入れるのは攻撃アニメーションの間に一回だけ
			// モンスターの当たり判定とダメージの設定はアニメーションがもっといいのが見つかったら
			Damage_Count(mutant.attack_hit_damage[mutant.m_now_attack].m_attack_damage, 5, &player.m_hp_value);
		}
	}
}



