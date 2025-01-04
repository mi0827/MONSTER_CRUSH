#include "src/WinMain.h"

#include "src/System/Vector3.h"
#include "Effect.h"

Effect::Effect()
{

}
Effect::~Effect()
{

}
//---------------------------------------------------------------
// エフェクト用の配列確保関数
//---------------------------------------------------------------
void Effect::NewArraySecure(int effect_max)
{
	// エフェクトの最大数を確保しておく
	//m_effect_max = effect_max;
	// エフェクトの配列を確保
	m_effect_info.resize(effect_max);
	// 再生中のエフェクトのを保存する変数
	m_playing_effect.resize(effect_max);
}


//---------------------------------------------------------------------------
//  エフェクト用変数のdelete用関数
//---------------------------------------------------------------------------
void Effect::EffectDelete()
{
	// 確保した配列を開放する
	m_effect_info.clear();
	m_playing_effect.clear();
}

//---------------------------------------------------------------------------
//  エフェクトの読み込み
//---------------------------------------------------------------------------
void Effect::LoadEffect(const char file_path[256], int effect_no, float  effect_play_speed)
{
	// エフェクトの読み込み
	m_effect_info[effect_no].effect_handle = LoadEffekseerEffect(file_path);
	// エフェクトの再生速度の設定
	m_effect_info[effect_no].effect_play_speed = effect_play_speed;
}

//---------------------------------------------------------------------------
//  エフェクトの再生
//---------------------------------------------------------------------------
void Effect::PlayEffect(int effect_num, Vector3 pos)
{
	// 再生中のエフェクト番号を保存する
	m_playing_effect_num = effect_num;
	// エフェクトの座標を指定の座標にセット
	m_model_pos = pos;
	// エフェクトが再生中していない再生させる
	// エフェクトが重複しないように
	m_playing_effect_flag = IsPlayingEffect();
	if (m_playing_effect_flag == -1)
	{
		// エフェクトの再生
		m_playing_effect[m_playing_effect_num] = PlayEffekseer3DEffect(m_effect_info[m_playing_effect_num].effect_handle);
	}
	// 現在再生中のエフェクトに設定しておく
	// エフェクトの再生スピードの設定
	SetSpeedPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], m_effect_info[effect_num].effect_play_speed);
	// エフェクトの生成座標の設定
	SetPosPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], pos.x, pos.y, pos.z);
}

//---------------------------------------------------------------------------
// 再生中のエフェクトの座標を設定
//---------------------------------------------------------------------------
void Effect::SetEffectPos(Vector3 pos)
{
	// エフェクトが再生中なら
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// 設定したい座標
		Vector3 pos_ = m_model_pos + pos;
		// エフェクトの設定
		SetPosPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], pos_.x, pos_.y, pos_.z);

	}
}

//---------------------------------------------------------------------------
//  再生中のエフェクトの座標をモデルの向きに合わせてずらす関数
//---------------------------------------------------------------------------
void Effect::SetEffectRotPos(Vector3 character_pos, Vector3 pos2, Vector3 rot)
{
	// エフェクトが再生中なら
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// 設定したいモデルの向きに合わせて座標をセットする
		Vector3 pos_ = m_model_pos;
		pos_.x += pos2.x * sinf(TO_RADIAN(rot.y));
		pos_.y += pos2.y;
		pos_.z += pos2.z * cosf(TO_RADIAN(rot.y));
		// エフェクトの座標を設定
		SetPosPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], pos_.x, pos_.y, pos_.z);
	}
}


//---------------------------------------------------------------------------
//  再生中のエフェクトの向きのを設定
//---------------------------------------------------------------------------
void Effect::SetEffectRot(float rot_x, float rot_y, float rot_z)
{
	// エフェクトが再生中なら
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// 設定したい向きのを出す
		Vector3 rot_;
		rot_.x = TO_RADIAN(rot_x);
		rot_.y = TO_RADIAN(rot_y);
		rot_.z = TO_RADIAN(rot_z);
		// エフェクトの向きを設定
		SetRotationPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], rot_.x, rot_.y, rot_.z);
	}
}

//---------------------------------------------------------------------------
// 再生中エフェクトの細かいサイズの設定
//---------------------------------------------------------------------------
void Effect::SetEffectSize(Vector3 size)
{
	// エフェクトが再生中なら
	int  a = IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]);
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// エフェクトのサイズを設定
		SetScalePlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], size.x, size.y, size.z);
	}
}

//---------------------------------------------------------------------------
//  再生中エフェクトの再生速度の設定
//---------------------------------------------------------------------------
void Effect::SetEffectPlaySpeed(float speed)
{
	// エフェクトが再生中なら
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		SetSpeedPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], speed);
	}
}

//---------------------------------------------------------------------------
//  エフェクトの色の設定
//---------------------------------------------------------------------------
void Effect::SetEffectColor(COLOR_F color)
{
	// エフェクトが再生中なら
	if (IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]) == 0)
	{
		// エフェクトの色の設定
		SetColorPlayingEffekseer3DEffect(m_playing_effect[m_playing_effect_num], color.r, color.g, color.b, color.a);
	}
}

//---------------------------------------------------------------------------
//  エフェクトが再生中かどうかを調べる(現在再生中のエフェクト)
//---------------------------------------------------------------------------
int Effect::IsPlayingEffect()
{
	return IsEffekseer3DEffectPlaying(m_playing_effect[m_playing_effect_num]);
}

//---------------------------------------------------------------------------
//  再生中のエフェクトを停止する
//---------------------------------------------------------------------------
void Effect::StopEffect()
{
	// エフェクトが再生中の時だけ停止する
	if(IsPlayingEffect() == 0)
	{
		StopEffekseer3DEffect(m_playing_effect[m_playing_effect_num]);
	}
}

//---------------------------------------------------------------------------
//  エフェクトの描画
//---------------------------------------------------------------------------
void Effect::EffectDraw()
{
	DrawEffekseer3D();
}