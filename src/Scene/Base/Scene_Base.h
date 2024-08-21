#pragma once

// シーンクラスたちのベースクラス
class Scene_Base
{
public:


	// 初期処理
	virtual void Init() = 0;
	// 更新処理
	// 引数
	// １：BGMのボリューム
	// ２：SEのボリューム
	virtual void Update(int bgm_volume, int se_volume) = 0;
	// 描画処理
	virtual void Draw() = 0;
	// 終了処理
	virtual void Exit() = 0;

	
public:
	const int FLAME_MAX = 60; // 一秒何フレームか

	bool m_scene_change_judge = false;  // シーンの切り替えを行っていいか (trueになったらシーンの切り替え可能)

};