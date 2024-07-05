#pragma once

//---------------------------------------------------------------------------
//  エフェクト用
//---------------------------------------------#include "src/System/Vector3.h"------------------------------


// エフェクト用の配列変数の獲得関数
// 引数
// １：エフェクトの最大個数
// ２：エフェクトを入れておく用の変数
// ３：エフェクトを使う時に使うハンドル用の変数
void Effect_New(int Effect_MAX, int* effect_container, int* m_effect_handle);


// エフェクト用の変数のdelete用の関数
// 引数
// １：エフェクトを入れておく用の変数
// ２：エフェクトを使う時に使うハンドル用の変数
void Effect_Delete(int* effect_container, int* effect_handle);

int* EffectContainerNew(int size);