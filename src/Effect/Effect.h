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
	//! @param エフェクトを入れておく用の変数
	//! @param エフェクトを使う時に使うハンドル用の変数
	void EffectNew(int Effect_MAX, int* effect_container, int* m_effect_handle);


	//! @brief エフェクト用の変数delete用の関数
	//! @param エフェクトを入れておく用の変数
	//! @param エフェクトを使う時に使うハンドル用の変数
	void EffectDelete(int* effect_container, int* effect_handle);

	//! @brief エフェクトを再生
	//! @param 再生したいエフェクト番号
	//! @param エフェクトの座標
	//! @param エフェクトの向き

	//! @brief エフェクトの座標を設定
	//! @param 設定したい座標

	//! @brief エフェクトの座標(ずらしたい量)を設定
	//! @param ずらしたい分の座標
	
	//! @brief エフェクトの向きの設定
	
	//! @brief 確保したエフェクトを開放する関数

private:
	//! エフェクトを保存する用の変数
	std::vector<int>m_effect_handle;
};
