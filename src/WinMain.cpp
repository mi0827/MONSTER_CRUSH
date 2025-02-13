#include "WinMain.h"
#include "GameMain.h"

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"


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

//! @brief  マウスを画面の真ん中に固定するかどうかのフラグ
//! @brief true : 変動可能、 false : 固定する
bool mouse_pos_middle_flag = false;


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Time; // 時間をとるための変数

	// ６：Log.txtを出力しないようにします
	SetOutApplicationLogValidFlag(FALSE);
	// ３：ウィンドウモードでかいします
	ChangeWindowMode(TRUE);
	// ７：ウィンドウタイトルの文字列の設定
	SetMainWindowText("Monsuta");
	// ８：背景の塗りつぶしの色を設定
	SetBackgroundColor(100, 100, 100);


	// ９：ウィンドウサイズの設定
	// 800 × 450 : ウィンドウの大きさ
	// 32 : 色数（32ビットカラー）*直接てな色の数じゃない
	SetGraphMode(SCREEN_W, SCREEN_H, 32);

	// 画面用のＺバッファのビット深度を設定する( 16 or 24 or 32 )
	SetCreateDrawValidGraphZBufferBitDepth(32);
	SetZBufferBitDepth(32);

	// 抜き色の設定(画像の中の　R:255, G:0, B:255)
	SetTransColor(255, 0, 255);

	// 起動画面を複数立ち上げることができるように
	SetDoubleStartValidFlag(TRUE);
	// ウィンドウが前面にあっても後ろにあっても動く状態にします
	SetAlwaysRunFlag(TRUE);

	// 10 :画面描画設定(描画領域を二つにして後ろに書き込んで完了したものを前にして画面に出す）
	SetDrawScreen(DX_SCREEN_BACK);


	//-----------------------------------------------------------
	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	//-----------------------------------------------------------
	// シェーダーファイルはこっちじゃないと読み込めない
	//SetUseDirect3DVersion(DX_DIRECT3D_9EX);

   // ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)	return -1;			// エラーが起きたら直ちに終了


	// Effekseerを初期化する。
	// 引数には画面に表示する最大パーティクル数を設定する。
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}
	// Effekseer対応
	//-----------------------------------------------------------
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定する。
	// ウインドウとフルスクリーンの切り替えが発生する場合は必ず実行する。
	// ただし、DirectX11を使用する場合は実行する必要はない。
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	//-----------------------------------------------------------


	//srand(GetNowCount() % RAND_MAX);

	for (int i = 0; i < 256; i++)
	{
		KeyFrame[i] = 0;
	}
	MouseLeftFrame = 0;
	MouseRightFrame = 0;

	SetCameraNearFar(0.1f, 3000.0f);
	SetupCamera_Perspective(TO_RADIAN(45.0f));

	// 下の二つの処理がないと３D空間を作れない
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	ChangeLightTypeDir(VGet(0.8f, -1.2f, 1.0f));

	//InputPadInit();
	// ゲームメインの初期処理
	GameInit();

	// マウスの表示状態の設定
	SetMouseDispFlag(FALSE);
	
	// フォントの変更
	ChangeFont("ＭＳ 明朝");
	// フォントを見やすくしている
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_4X4);

	// ４：メインループ
	while (TRUE) {
		//// DXライブラリのカメラとEffekseerのカメラを同期する。
		//Effekseer_Sync3DSetting();

		Time = GetNowCount();

		// １１：描画領域をクリア
		ClearDrawScreen();
		InputPadUpdate();

		GetHitKeyStateAll(KeyBuffer); // キーボードのすべてのキーの押下状態を取得する

		// キーが押された時をとる処理
		for (int i = 0; i < 256; i++)
		{
			if (KeyBuffer[i])	KeyFrame[i]++; // 何か押されたらカウントを増やす
			else				KeyFrame[i] = 0; // 何も押されてなっかたらカウントを0にする
		}

		// 上の二つのコメントと同様のことをしている
		if (CheckMouseInput(MOUSE_INPUT_LEFT))	MouseLeftFrame++;
		else									MouseLeftFrame = 0;
		// 上と同様
		if (CheckMouseInput(MOUSE_INPUT_RIGHT))	MouseRightFrame++;
		else									MouseRightFrame = 0;
		
		NowMouseX = GetMouseX();
		NowMouseY = GetMouseY();

		// ゲーム更新処理
		GameUpdate();

		// Effekseer更新処理
		UpdateEffekseer3D();

		// ゲーム描画処理
		GameDraw();
	

		BeforeMouseX = NowMouseX;
		BeforeMouseY = NowMouseY;

		if (mouse_pos_middle_flag == false)
		{
			//	マウス座標を設定する（画面の真ん中に固定する）
			SetMousePoint(SCREEN_W / 2, SCREEN_H / 2);
		}
		
		
		// １２：描画が完了したものを画面に映します
		ScreenFlip();

		// これがあるとフレームレートが固定できる
		while (GetNowCount() - Time < 17) {}

		// ５：赤いXボタンを押したらループを抜ける（ウィンドウを閉じたら）
		if (ProcessMessage())
		{
			break;
		}

		// ５：右シフトキーを押したら終了
		if (CheckHitKey(KEY_INPUT_RSHIFT))
		{
			break;
		}
	}
	// ゲーム終了処理
	GameExit();
	InputPadExit();

	//--------------------------
	// Effekseerを終了する。
	Effkseer_End();
	//--------------------------

	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}

//---------------------------------------------------------------------------------
//	キーが押された瞬間を取得する
//---------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------
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
//	マウスのX座標を取得する
//---------------------------------------------------------------------------------
int GetMouseX() // X座標
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_x;
}

//---------------------------------------------------------------------------------
//	マウスのX座標を取得する
//---------------------------------------------------------------------------------
int GetMouseY() // Y座標
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_y;
}

//---------------------------------------------------------------------------------
//	マウスのX座標の移動量を取得する
//---------------------------------------------------------------------------------
int GetMouseMoveX()
{
	return NowMouseX - BeforeMouseX;
}

//---------------------------------------------------------------------------------
//	マウスのX座標の移動量を取得する
//---------------------------------------------------------------------------------
int GetMouseMoveY()
{
	return NowMouseY - BeforeMouseY;
}

//---------------------------------------------------------------------------------
//	固定されたマウスのX座標の移動量を取得する
//---------------------------------------------------------------------------------
int GetFixedMouseMoveX()
{
	int move_x;
	// 画面の中心ｘ座標
	int fixed_x = SCREEN_W / 2;
	move_x = GetMouseX() - fixed_x;
	return move_x;
}

//---------------------------------------------------------------------------------
//	固定されたマウスのY座標の移動量を取得する
//---------------------------------------------------------------------------------
int GetFixedMouseMoveY()
{
	int move_y;
	// 画面の中心ｘ座標
	int fixed_y = SCREEN_H / 2;
	move_y = GetMouseY() - fixed_y;
	return move_y;
}

//---------------------------------------------------------------------------------
//	マウスの位置を固定するかを変動できるかを切り替えれる
//---------------------------------------------------------------------------------
void ChangeMousePosMove(bool mouse_pos_flag)
{
	mouse_pos_middle_flag = mouse_pos_flag;
}

//---------------------------------------------------------------------------------
//	度をラジアンに変換する関数
//---------------------------------------------------------------------------------
float TO_RADIAN(float degree)
{
	return degree * 3.14159265f / 180.0f;
}

//---------------------------------------------------------------------------------
//	ラジアンを度に変換する関数
//---------------------------------------------------------------------------------
float TO_DEGREE(float radian)
{
	return radian * 180.0f / 3.14159265f;
}