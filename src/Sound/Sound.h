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

	//! @brief サウンドが再生中かどうかを返す
	//! @return true：再生中、false：停止中
    bool PlayingSound();

	//! 指定のサウンドが再生中かどうかを返す
	//! @param 再生中か調べたいSE番号
	//! @return true：再生中、false：停止中
	bool PickPlayingSound(int num);

	//! @brief サウンドのボリューム調整用の関数
	//! @param 設定したいボリューム
	void SetSoundVolume(int volume);

	//! @brief 再生中のサウンドを止める
	void StopSound();

private:
	//! サウンドの入れ物
	std::vector<int>m_sound_handle;
	//! サウンドの最大数を保存する
	int m_sound_max;
	//! 現在再生中のサウンド番号
	int m_now_playing_se_num;

public:
	//! 現在再生可能状態化を判断するためのフラグ
	bool m_playing_flag = true;
};