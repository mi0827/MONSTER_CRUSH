#pragma once
//! @file Effect.h
//! @brief エフェクト用のクラス
class Effect
{
public:
	//! @brief コンストラクタ
	Effect();
	//! @brief デストラクタ
	~Effect();

	//! @brief エフェクト用の配列変数の確保関数
	//! @param エフェクトの最大個数
	void NewArraySecure(int effect_max);

	//! @brief 確保したエフェクトを開放する関数
	void EffectDelete();

	//! @brief エフェクトの読み込み用関数
	//! @param エフェクト保存用番号
	//! @param エフェクトの再生速度
	void LoadEffect(const char file_path[256], int effect_noint ,float effect_play_speed);

	//! @brief エフェクトを再生
	//! @param 再生したいエフェクト番号
	//! @param エフェクトの座標
	
	void PlayEffect(int effect_num, Vector3 pos);

	//! @brief エフェクトの座標を設定
	//! @param ずらしたい座標
	void SetEffectPos( Vector3 pos = { 0.0f,0.0f,0.0f });

	//! @brief エフェクトの座標をモデルの向きに合わせてずらす関数
	//! @param ずらしたい座標
	//! @param 設定したいモデルの向き
	void SetEffectRotPos(Vector3 pos, Vector3 rot);

	//! @brief エフェクトの向きの設定
	//! @param エフェクトの向き
	void SetEffectRot( Vector3 rot);

	//! @brief エフェクトの細かいサイズの設定
	//! @param 各方向に対しての拡大率(Vector3)
	void SetEffectSize( Vector3 size);

	//! @brief エフェクトの再生速度の設定
	//! @param 再生速度
	void SetEffectPlaySpeed( float speed);

	//! @brief エフェクトの色の設定
	//! @param 設定したいカラー
	void SetEffectColor( COLOR_F color);

	//! @brief エフェクトの描画
	void EffectDraw();
private:
	//! エフェクトを保存する用の変数
	struct EffectInfo
	{
		//! エフェクトの入れ物
		int effect_handle;
		//! エフェクトの再生速度
		int effect_play_speed;
		
	};
	std::vector<EffectInfo>m_effect_info;
	//! 現在再生中のエフェクト番号を保存する変数
	int m_playing_effect_num;

private:
	//! 現在再生中のエフェクトのを保存する変数
	int  m_playing_effect;
	//! モデルの座標
	Vector3 m_model_pos;
};
