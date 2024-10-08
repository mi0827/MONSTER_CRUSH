#pragma once
// Windows プログラムを使うので
#include <Windows.h>

// DXライブラリを使うので
#include <DxLib.h>
#include <time.h> // 時間を使うのにいる
#include <math.h> // サイン、コサインや平方根など、数学的な関数です。

#include <vector>

#pragma warning (disable : 4819)

#include <d3d9.h>
//#include <d3dx9.h>

// 画面サイズ
#define SCREEN_W	1920//800//1280//1920
#define SCREEN_H	1080//450//720 //1080

#include "System/InputPad.h" // ゲームパッド

// EffekseerForDXLib.hをインクルードします。
#include "EffekseerForDXLib.h"

//------------------------
// 関数の定義
bool PushHitKey(int key); // キーボードが押されているかを見る
// マウスが押されたを見る物
bool CheckMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
// マウスが押されているかを見るもの
bool PushMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
// マウスの座標を取得する
int GetMouseX(); // X座標
int GetMouseY(); // Y座標
// マウスの移動量を取得
int GetMouseMoveX(); // X座標
int GetMouseMoveY(); // Y座標
// 固定されたマウスの移動量を取得
int GetFixedMouseMoveX();// X座標
int GetFixedMouseMoveY();// Y座標

// 度をラジアンに変換する関数
float TO_RADIAN(float degree);
// ラジアンを度に変換する関数
float TO_DEGREE(float radian);

#include "src/System/Vector3.h"
void DrawCircle3D_XZ(Vector3 center, float radius, int color, bool fill = false);

