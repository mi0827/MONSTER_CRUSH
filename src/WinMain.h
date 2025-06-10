#pragma once

#define WIN32_LEAN_AND_MEAN		// 大半の不要なWin32 APIを除外
#define NOMINMAX				// min/max マクロを定義させない

// Windows プログラムを使うので
#include <Windows.h>

// DXライブラリを使うので
#include <DxLib.h>
#include <time.h> // 時間を使うのにいる
#include <math.h> // サイン、コサインや平方根など、数学的な関数です。

#include <vector>
#include <algorithm>

#pragma warning (disable : 4819)

#include <d3d9.h>
//#include <d3dx9.h>

//! 画面サイズ
#define SCREEN_W	1920//800//1280//1920
#define SCREEN_H	1080//450//720 //1080
//! 画面サイズX軸の半分のサイズ
static constexpr int SCREEN_W_HALF = SCREEN_W / 2;
//! 画面サイズY軸の半分のサイズ
static constexpr int SCREEN_H_HALF = SCREEN_H / 2;
//! ゲームパッドの振動する値
static constexpr int PAD_VIDRATION_STRENGTH = 1000; // 振動大
static constexpr int PAD_VIDRATION_MEDIUM = 600;    // 振動中
static constexpr int PAD_VIDRATION_SMALL = 200;     // 振動小
//! ゲームパッドの振動させるモーター番号
static constexpr int PAD_VIDRATION_MOTOR = -1;

//! よく使うカラー
//! 赤
static const int RED = GetColor(255, 0, 0);
//! 青
static const int BLUE = GetColor(0, 0, 255);
//! 緑
static const int LIME = GetColor(0, 255, 0);
//! 黒
static const int BLACK = GetColor(0, 0, 0);
//! 白
static const int WHITE = GetColor(255, 255, 255);
//! 黄色
static const int YELLOW = GetColor(255, 255, 0);
//! ダークイエロー
static const int DARKYELLOW = GetColor(128, 128, 0);
//! ダークオレンジ
static const int DARKORANGE = GetColor(255, 128, 0);
//! オレンジ　
static const int ORANGE = GetColor(255, 100, 50);
//! グレイ
static const int GRAY = GetColor(128, 128, 128);
 
 
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

//! @brief  マウスの位置を固定するかを変動できるかを切り替えれる
//! @param true : 変動可能、false : 固定化
void ChangeMousePosMove(bool mouse_pos_flag);

// 度をラジアンに変換する関数
float TO_RADIAN(float degree);
// ラジアンを度に変換する関数
float TO_DEGREE(float radian);

#include "src/System/Vector3.h"
void DrawCircle3D_XZ(Vector3 center, float radius, int color, bool fill = false);


