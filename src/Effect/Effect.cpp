#include "src/WinMain.h"
#include "Effect.h"

//---------------------------------------------------------------
// エフェクト用の配列確保関数
//---------------------------------------------------------------
void Effect::EffectNew(int Effect_MAX, int* effect_container, int* m_effect_handle)
{
	// 作り直す
	effect_container = new int[Effect_MAX];
	effect_handle = new int[Effect_MAX];
	for (int i = 0; i < Effect_MAX; i++)
	{
		effect_container[i] = 0;
		effect_handle[i] = 0;
	}
}


//---------------------------------------------------------------------------
//  エフェクト用変数のdelete用関数
//---------------------------------------------------------------------------
void Effect::EffectDelete(int* effect_container, int* effect_handle)
{
	delete[] effect_container;
	delete[] effect_handle;
}


