#pragma once
//! @file Sound.h
//! @brief サウンド関連のクラス
class Sound
{
public:
	//! @brief コンストラクタ
	Sound();
	//! @brief デストラクタ
	~Sound();

	//! @brief サウンド分の配列を作る関数
	//! @param サウンドの数の最大数
	void NewArraySecureSound(int sound_max);

	//! @brief サウンドの読み込み
	//! @param サウンドデータのパス
	//! @param サウンドの番号
	void LoadSound(const char path[256], int no);

	//! @brief サウンドの再生
	//! @param サウンドの番号
	//! @param 再生するタイプ
	//! DX_PLAYTYPE_NORMAL　:　ノーマル再生
    //! DX_PLAYTYPE_BACK　　 : バックグラウンド再生
    //! DX_PLAYTYPE_LOOP　　 : ループ再生
	//! @param 再生をループするかどうか
	void PlaySound_(int no, int type, bool loop);

	//! @brief サウンドが再生中がどうかを返す
	//! @param 調べたいサウンド番号
	bool PlayingSound(int no);

	//! @brief サウンドのボリューム調整用の関数
	//! @param 設定したいボリューム
	void SetSoundVolume(int volume);

private:
	//! サウンドの入れ物
	std::vector<int>m_sound_handle;
	//! サウンドの最大数を保存する
	int m_sound_max;
};