#include "DxLib.h"
#include <math.h>
#include "src/WinMain.h"



// キー入力に関する変数
char KeyBuffer[256];
int KeyFrame[256];
// マウス入力に関する変数
int MouseLeftFrame;
int MouseRightFrame;
// 上の4つの変数はキーボードとマウスの押された時
// を取得するために必要な関変数

// マウスの左右移動に関する変数
int NowMouseX;
int NowMouseY;
int BeforeMouseX;
int BeforeMouseY;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	int StageModelHandle;
	int CharaModelHandle;
	int StageShadowMapHandle;
	int CharaShadowMapHandle;
	float Angle;
	VECTOR CharaPosition;

	SetBackgroundColor(100, 100, 100);
	// ＤＸライブラリの初期化
	if (DxLib_Init() < 0)
	{
		// エラーが発生したら終了
		return -1;
	}

	// 描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);

	// キャラクターモデルの読み込み
	CharaModelHandle = MV1LoadModel("Data/Model/Player/Player.mv1");

	// ステージモデルの読み込み
	StageModelHandle = MV1LoadModel("Data/ground.mqoz");

	// ステージモデル用のシャドウマップハンドルの作成
	StageShadowMapHandle = MakeShadowMap(2048, 2048);

	// キャラクターモデル用のシャドウマップハンドルの作成
	CharaShadowMapHandle = MakeShadowMap(2048, 2048);

	// カメラの位置と向きを設定
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 1500.0f, -1500.0f), VGet(0.0f, 800.0f, -800.0f));

	// 描画する奥行き方向の範囲を設定
	SetCameraNearFar(160.000f, 80000.000f);

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(StageShadowMapHandle, VGet(-4000.0f, -1.0f, -4000.0f), VGet(4000.0f, 4000.0f, 4000.0f));
	SetShadowMapDrawArea(CharaShadowMapHandle, VGet(-4000.0f, -1.0f, -4000.0f), VGet(4000.0f, 4000.0f, 4000.0f));

	// ライトの方向を設定
	SetLightDirection(VGet(0.5f, -0.5f, 0.5f));

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(StageShadowMapHandle, VGet(0.5f, -0.5f, 0.5f));
	SetShadowMapLightDirection(CharaShadowMapHandle, VGet(0.5f, -0.5f, 0.5f));

	// ステージモデル用のシャドウマップへの描画の準備
	ShadowMap_DrawSetup(StageShadowMapHandle);
	MV1SetScale(StageModelHandle, VGet(100, 100, 100)); // モデルの大きさ(10分の１のサイズ)
	// ステージモデル用のシャドウマップへステージモデルの描画
	MV1DrawModel(StageModelHandle);

	// ステージモデル用のシャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// キャラクターの位置となる角度を初期化
	Angle = 0.0f;

	// メインループ
	while (ProcessMessage() == 0)
	{
		// 画面をクリア
		ClearDrawScreen();


		// キャラクターの位置となる角度を変更
		Angle += 0.0125f;
		if (Angle > DX_PI_F * 2.0f)
		{
			Angle -= DX_PI_F * 2.0f;
		}

		// キャラクターの座標を算出
		CharaPosition.x = sin(Angle) * 1000.0f;
		CharaPosition.z = cos(Angle) * 1000.0f;
		CharaPosition.y = 0.0f;

		// キャラクターモデルの座標を変更
		MV1SetPosition(CharaModelHandle, CharaPosition);


		// キャラクターモデル用のシャドウマップへの描画の準備
		ShadowMap_DrawSetup(CharaShadowMapHandle);

		// キャラクターモデル用のシャドウマップへキャラクターモデルの描画
		MV1DrawModel(CharaModelHandle);

		// キャラクターモデル用のシャドウマップへの描画を終了
		ShadowMap_DrawEnd();


		// 描画にキャラクターモデル用のシャドウマップと
		// ステージモデル用のシャドウマップのどちらも使用する
		SetUseShadowMap(0, StageShadowMapHandle);
		SetUseShadowMap(1, CharaShadowMapHandle);

		// ステージモデルの描画
		MV1DrawModel(StageModelHandle);

		// キャラクターモデルの描画
		MV1DrawModel(CharaModelHandle);

		// 描画に使用するシャドウマップの設定を解除
		SetUseShadowMap(0, -1);
		SetUseShadowMap(1, -1);


		// 裏画面の内容を表画面に反映
		ScreenFlip();
	}

	// シャドウマップの削除
	//DeleteShadowMap(StageShadowMapHandle);
	DeleteShadowMap(CharaShadowMapHandle);

	// ステージモデルの削除
	MV1DeleteModel(StageModelHandle);

	// キャラクターモデルの削除
	MV1DeleteModel(CharaModelHandle);

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}


//---------------------------------------------------------------------------------
//	キーが押された瞬間を取得する
bool PushHitKey(int key)
{
	if (KeyFrame[key] == 1)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------
//	マウスが押されているかを取得する
bool CheckMouseInput(int button)
{
	if (GetMouseInput() & button)
	{
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------
//	マウスが押された瞬間を取得する
bool PushMouseInput(int button)
{
	if (button & MOUSE_INPUT_LEFT)
	{
		if (MouseLeftFrame == 1)
		{
			return true;
		}
	}

	if (button & MOUSE_INPUT_RIGHT)
	{
		if (MouseRightFrame == 1)
		{
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------------
//	マウスの座標を取得する
int GetMouseX() // X座標
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_x;
}
int GetMouseY() // Y座標
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_y;
}
//---------------------------------------------------------------------------------
//	マウスの移動量を取得する
int GetMouseMoveX()
{
	return NowMouseX - BeforeMouseX;
}
int GetMouseMoveY()
{
	return NowMouseY - BeforeMouseY;
}

//---------------------------------------------------------------------------------
//	度をラジアンに変換する関数
float TO_RADIAN(float degree)
{
	return degree * 3.14159265f / 180.0f;
}

//---------------------------------------------------------------------------------
//	ラジアンを度に変換する関数
float TO_DEGREE(float radian)
{
	return radian * 180.0f / 3.14159265f;
}