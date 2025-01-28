#pragma once
//! @file SceneBase.h
//! @brief シーンクラスたちのベースクラス
class Scene_Base
{
public:
	//--------------------
	// 定数
	//--------------------
	static constexpr int SAMPLEPLAYER = 0; // サンプルプレイヤーの番号
	static constexpr int MUTANT = 0;           // ミュータントの番号
	static constexpr int MONSTER = 1;         // モンスターの番号

	//! @brief コンストラクタ
	Scene_Base();
	//! @brief デストラクタ
	~Scene_Base();

	//! @brief どのキャラクターを使うかを設定する関数
	//! @param どのプレイヤーを使うかの番号
	//! @param どのモンスターを使うかの番号
	void SetCharacter(int player_num = 0, int monster_num = 0);

	//! @brief 全シーンで使う物の初期化
	void BaseInit();
	// @brief 全シーンで使うものの描画
	// @param シーン番号
	// @param 描画座標
	void BaseDraw(int scene_num, Vector2 draw_pos);

	//! @brief 初期処理
	virtual void Init() = 0;
	//! @brief 更新処理
	//! @param BGMのボリューム
	//! @param SEのボリューム
	virtual void Update() = 0;
	//! @brief 描画処理
	virtual void Draw() = 0;
	//! @brief 終了処理
	virtual void Exit() = 0;
	//! @brief フィールドとの当たり判定
	virtual void HitField() = 0;

	//! @brief オプションメニューでもらってきた値を反映する関数
	//! @param BGMの音量
	//! @param SEの音量
	//! @param マウス感度
	virtual void OptionValuesReflect(int bgm, int se, int mouse) = 0;


	//! @brief 空の描画
	void SkyDraw();

	//! @brief シャドーマップ用の初期化関数
	void ShadowMapInit();

	//! @brief 指定場所にのシャドーマップの範囲の設定
	//! @param シャドーマップハンドル
	//! @param 指定の座標
	void SetShadowMapArea(int shadowMap_handle, Vector3 player_pos);

	//! @brief 描画に使用するシャドウマップの設定を解除（これを使わないとシャドーマップが使えない）
	void UseShadowMapSet();

	//! @brief シャドーマップの削除
	void ExitShadowMap();

	//! @brief スクリーン座標に文字を描画するための座標を設定する関数
	//! @param 設定したい座標
	//! @param 設定した座標からどれだけずらしたいかの値(デフォルトでずらさないように設定しておく)
	VECTOR DrawStringWorld(Vector3 pos, Vector3 shift_pos = { 0.0f,0.0f,0.0f });

	//! @brief 次に行いたいシーンをセットする関数
	//! @param 次に行いたいシーン番号
	void SetNextScene(int next_scene);


	//! @brief フェードアウト処理関数
	void FadeOutUpdate();
	//! @brief フェードアウトからのシーンチェンジ用関数
	//! @param 次のシーン番号
	void FadeOutSceneChange(int next_scene);
	//! @brief フェードイン処理
	void FadeInUpdate();
	//! @brief フェードアウトを描画する処理
	void FadeDraw();

	//! @brief プレイヤーの読み込み処理
	//! @param 使用したいキャラクター番号
	//! @param hero : 女剣士、bot：ボット戦士
	void PlayerInit(int character_num);

public:
	//! 一秒何フレームかの変換用定数
	const int FLAME_MAX = 60;

	//! シーンの切り替えを行っていいかフラグ (trueになったらシーンの切り替え可能)
	bool m_scene_change_judge = false;

	//! 単体専用のシャドーマップ用変数１
	int m_shadowMap_handle_1 = 0;
	//! 単体専用のシャドーマップ用変数２
	int m_shadowMap_handle_2 = 0;
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
	Vector3 m_camera_rot{ 0.0f,0.0f,0.0f };


	// すべてのシーン番号
	enum Scene
	{
		Title, // タイトルシーン
		Story, // ストーリーシーン
		QuestArea, // クエスト受注シーン
		Battle, // バトルシーン
		End // エンドシーン
	};
	//! 現在のシーン
	int m_now_scene = Title;

	//! 次に行いたいシーン
	int m_next_scene = -1;

	//! 各シーンで行われる順番
	enum SceneTurn
	{
		//FadeIn,
		Main,
		FadeOut
	};
	//! 現在のターン
	int m_turn = Main;

	// すべてのシーンで使うテキスト
	Text m_text;
	// テキストの数
	static constexpr int TEXT_MAX = 2;

private:
	//! シーンでのフレームカウント
	int m_frame_count = 0;
	//! フェードアウト・インタイムカウント
	int m_fade_time = 0;
	//! フェードアウト・インの割合
	int m_fade_ratio = 0;
	//! どのくらいの値変化するのかの値
	int m_fade_value = 0;

	// 空の座標など
	Transform m_sky_transform;
	// 空用のモデル
	Model m_sky_model;
public:
	// フィールドオブジェクト
	TitleField m_field_1;
	// フィールドオブジェクト
	Field m_field_2;
	// プレイヤーのオブジェクト
	CharacterBase* m_player;
	enum CharacterNum
	{
		hero,
		bot,
	};
	// キャラクター番号
	int m_character_num = hero;
};