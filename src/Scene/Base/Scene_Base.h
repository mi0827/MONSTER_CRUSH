#pragma once

// シーンクラスたちのベースクラス
class Scene_Base
{
public:
	//--------------------
	// 定数
	//--------------------
	static constexpr int SAMPLEPLAYER = 0; // サンプルプレイヤーの番号
	static constexpr int MUTANT = 0;           // ミュータントの番号
	static constexpr int MONSTER = 1;         // モンスターの番号

	//! @brief どのキャラクターを使うかを設定する関数
	//! @param どのプレイヤーを使うかの番号
	//! @param どのモンスターを使うかの番号
	void SetCharacter(int player_num = 0, int monster_num = 0);

	//! @brief 初期処理
	virtual void Init() = 0;
	//! @brief
	//! @param BGMのボリューム
	//! @param SEのボリューム
	virtual void Update() = 0;
	//! @brief 描画処理
	virtual void Draw() = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;
	//! @brief フィールドとの当たり判定
	virtual void HitField() = 0;


	//! @bom,se の音量を設定する用の関数
	// void SetSoundSize(int bgm_volume, int se_volume);

	//! @brief シャドーマップ用の初期化関数
	void ShadowMapInit();

	//! @brief プレイヤーのシャドーマップの範囲の設定
	void SetPlayerShadowMapArea(Vector3 player_pos);

	//! @brief 描画に使用するシャドウマップの設定を解除（これを使わないとシャドーマップが使えない）
	void UseShadowMapSet();

	//! @brief シャドーマップの削除
	void ExitShadowMap();

	//! @brief スクリーン座標に文字を描画するための座標を設定する関数
	//! @param 設定したい座標
	//! @param 設定した座標からどれだけずらしたいかの値(デフォルトでずらさないように設定しておく)
	VECTOR DrawStringWrold(Vector3 pos, Vector3 shift_pos = { 0.0f,0.0f,0.0f });

	//! @brief 次に行いたいシーンをセットする関数
	//! @param 次に行いたいシーン番号
	void SetNextScene(int next_scene);



public:
	//! 一秒何フレームかの変換用定数
	const int FLAME_MAX = 60;

	//! シーンの切り替えを行っていいかフラグ (trueになったらシーンの切り替え可能)
	bool m_scene_change_judge = false;

	//! プレイヤー用のシャドーマップ用変数
	int m_player_shadowMap_handle = 0;
	//! シャドーマップ用の変数
	int m_shadowMap_handle = 0;

	// シャドーマップのライトの角度
	float m_light_angle = 0;
	// ライトのベクトル
	VECTOR m_light_direction;


	//! どのプレイヤーを使うかの番号を保存する
	int m_player_num = 0;
	//! どのモンスターを使うかの番号を保存する
	int m_monster_num = 0;

	//! カメラの向きを保存する用の変数
	Vector3 m_camera_rot{0.0f,0.0f,0.0f};


	// すべてのシーン番号
	enum Scene
	{
		Title,
		QuestArea,
		Battle,
		End
	};


	//! 現在のシーン
	int m_now_scene = Title;
	//! 次に行いたいシーン
	int m_next_scene = -1;

};