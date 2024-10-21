#pragma once


//! @fule Animation.h
//! @brief アニメーション用のクラス


//! アニメーション用のクラス

class Animation
{
private:
	//! ブレンド率が一フレームで変化する分
	static constexpr float BLEND_VALUE = 1.0f / 5.0f;
	//! ブレンド率の最大値
	static constexpr float BLEND_VALUE_MAX = 1.0f;
	//! ブレンド率の最低値
	static constexpr float BLEND_VALUE_MIN = 0.0f;
public:

	//!@brief コンストラクタ
	Animation();
	//!@brief デストラクタ
	virtual ~Animation();

	//=========================================================================
	// 構造体の定義
	//=========================================================================

	//! アニメーションのデータ用の構造体
	struct AnimData
	{
		int handle = -1;       //! アニメーションの入れ物
		int index = -1;        //! アニメションのインデックス
		float play_speed = -1; //! アニメーションの再生速度
	};
	std::vector<AnimData*> anim_handle;

	//! 再生中のアニメーションの情報構造体
	struct Information
	{
		bool  is_playing = false;         //!< 再生中かどうか
		bool  is_loop = false;              //!< ループ再生かどうか
		int   animation_index = -1;         //!< 現在の再生中の番号
		int   animation_attach_index = -1;  //!< アタッチされたスロット番号
		float m_blend_ratio = 0.0f;         //!< ブレンド比(0.0f～1.0f)
		float animation_total_time = 0.0;   //!< 総再生時間
		float play_time = 0.0f;             //!< 現在再生中の時間
	};
	//! 構造体はアニメーションブレンドのため2系統を持つ
	//! [0]現在のアニメーション
	//! [1]前のアニメーション
	Information m_contexts[2];


	//=========================================================================
	// 関数の定義
	//=========================================================================

	//! @brief アニメーションの情報の初期化関数
	//! @param どちらの構造体を初期化するかの番号
	void Information_Init(int num);

	//! @brief アニメーションの初期設定
	//! @param アニメーションの数
	//! @param デフォルト設定したいアニメーションの番号
	//! @detail デフォルトアニメーションのセット、アニメーションの入れ物の確保
	void Init_Animation(int anim_max, int default_anim);

	//! @brief読み込み用関数
	//! @param ファイルのパス
	//! @param アニメーション番号
	void Load_Animation(const char file_path[256], int anim_no, int anim_index, float anim_play_speed);

	//! @briefアニメーションをセットする関数
	//! @param ベースのモデル
	//! @param アッタチしたいモデル番号
	//! @param ループ再生させるかどうか
	void Init_Attach_Animation(Model* model, int anim_num, bool loop);

	//! @briefアニメーションをセットする関数
	//! @param ベースのモデル
	//! @param アッタチしたいモデル番号
	//! @param ループ再生させるかどうか
	void Attach_Animation(Model* model, int anim_num, bool loop);

	//! @briefついているアニメーションを取り外すようの関数
	//! @param ベースのモデル
	//! @param デタッチしたいモデル番号
	void Detach_Animation(Model* model);

	//! @brief ブレンド率の変更関数
	void Change_Blend();

	//! @brief アニメーションの変更を行っていい以下を判断する関数
	bool Change_Flag(bool flag1);

	//! @briefアニメーションの切り替え用関数
	//! @param ベースのモデル
	//! @param 次にセットしたいアニメーション番号
	//! @param アニメーションをループさせるかどうか
	void Change_Animation(Model* model, int anim_num, bool loop);

	//! @brief アニメーションの切り替え用関数
	//! @param ベースのモデル
	//! @param 次にセットしたいアニメーション番号
	//! @param アニメーションをループさせるかどう
	//! @param コンボフラグ
	//! @detail アニメーションが終わるタイミングで切り替えるでChange_Animationを呼びたい時用の関数
	void Action_Change_Animation(Model* model, int anim_num, bool loop, bool* combo_flag);

	//! @briefアニメーションの再生
	//! @param ベースのモデル
	//! @param コンボフラグ
	void Play_Animation(Model* model, bool combo_flag);

	//! @briefアニメーションをループさせる
	void Loop_Animation();

	//! @briefループさせない場合
	//! @param ベースのモデル
	void Not_Loop(Model* model);

private:
	//! アニメーションの最大数
	int m_anim_max = 0;
	//! デフォルトでセットしたアニメーション番号用の変数
	int m_default_anim = -1;
	//! 二つアニメーションが付いているかどうか
	bool m_attached = false;
	//! デタッチ済みかどうかを調べる
	bool m_detach_flag = false;
	//! どのアニメーションが入っているかを保存するもの
	int m_anim_num = -1;

public:
	//! アニメーションの切り替えを行っていいかのフラグ
	//! フラグの上げは書くキャラクラスで行う
	//! フラグの下げはアニメーション変更関数で下げる
	//! このフラグをを上げるタイミングが大事
	bool m_anim_change_flag = true;

	bool m_playing_anim = false;
};