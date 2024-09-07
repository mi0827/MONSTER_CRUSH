#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "Scene_Base.h"

// -------------------------------------------------------------------------- -
// どのキャラクターを使うかの設定
//---------------------------------------------------------------------------
void Scene_Base::SetCharacter(int player_num, int monster_num)
{
	// キャラクター番号の保存
	m_player_num = player_num;
	m_monster_num = monster_num;
}

//-----------------------------------------------
// シャドーマップの初期処理
//-----------------------------------------------
void Scene_Base::ShadowMapInit()
{
	// プレイヤー用のシャドーマップの作成
	m_player_shadowMap_handle = MakeShadowMap(4096, 4096);
	// シャドーマップの作成
	m_shadowMap_handle = MakeShadowMap(4096, 4096);
	// ライトの角度の初期化
	m_light_angle = 10.0f;

	// ライトの方向ベクトルの算出
	m_light_direction.x = sin(m_light_angle);
	m_light_direction.z = cos(m_light_angle);
	m_light_direction.y = -1.0f;
	VectorNormalize(&m_light_direction, &m_light_direction);

	// ライト方向を設定
	SetLightDirection(m_light_direction);

	// シャドーマップが想定するライトの方向のセット
	SetShadowMapLightDirection(m_player_shadowMap_handle, m_light_direction);
	SetShadowMapLightDirection(m_shadowMap_handle, m_light_direction);
	// シャドーマップに描画する範囲の設定
	SetShadowMapDrawArea(m_shadowMap_handle, VGet(-8192.0f, -0.1f, -8192.0f), VGet(+8192.0f, 3000.0f, +8192.0f));

}

//-----------------------------------------------
// プレイヤーのシャドーマップの範囲設定
//-----------------------------------------------
void Scene_Base::SetPlayerShadowMapArea(Vector3 player_pos)
{
	// シャドーマップに描画する範囲の設定
// 今はこの　範囲でプレイヤーのシャドウマップを設定しているが
// 背後の影が気に入らなければ二つ目のY座標の値を上げるか全体的に描画範囲を広げろ
	SetShadowMapDrawArea(m_player_shadowMap_handle,
		VGet(player_pos.x - 200.0f, -0.1f, player_pos.z - 200.0f),
		VGet(player_pos.x + 200.0f, +1500.0f, player_pos.z + 200.0f));
}

//-----------------------------------------------
// 描画に使用するシャドウマップの設定を解除
//-----------------------------------------------
void Scene_Base::UseShadowMapSet()
{
	
	SetUseShadowMap(1, -1);
	SetUseShadowMap(0, -1);
}

//-----------------------------------------------
// シャドーマップの削除
//-----------------------------------------------
void Scene_Base::ExitShadowMap()
{
	DeleteShadowMap(m_player_shadowMap_handle);
	DeleteShadowMap(m_shadowMap_handle);
}
