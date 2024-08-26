#pragma once

// シーンクラスたちのベースクラス
class Scene_Base
{
public:


	//! @brief 初期処理
	virtual void Init() = 0;
	//! @brief
	//! @param BGMのボリューム
	//! @param SEのボリューム
	virtual void Update(int bgm_volume, int se_volume) = 0;
	//! @brief 描画処理
	virtual void Draw() = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;

	//! @brief シャドーマップ用の初期化関数
	void ShadowMapInit();

	//! @brief プレイヤーのシャドーマップの範囲の設定
	void SetPlayerShadowMapArea(Vector3 player_pos);

public:
	const int FLAME_MAX = 60; // 一秒何フレームか

	bool m_scene_change_judge = false;  // シーンの切り替えを行っていいか (trueになったらシーンの切り替え可能)

	//! プレイヤー用のシャドーマップ用変数
	int m_player_shadowMap_handle = 0;
	//! シャドーマップ用の変数
	int m_shadowMap_handle = 0;

	// シャドーマップのライトの角度
	float m_light_angle = 0;
	// ライトのベクトル
	VECTOR m_light_direction;
};