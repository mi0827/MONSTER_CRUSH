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
	// エフェクトの配列を確保
	m_effect_info.resize(effect_max);
}


//---------------------------------------------------------------------------
//  エフェクト用変数のdelete用関数
//---------------------------------------------------------------------------
void Effect::EffectDelete()
{
	// 確保した配列を開放する
	m_effect_info.clear();
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
	// エフェクトのハンドル
	int effect_handle = m_effect_info[effect_num].effect_handle;

	// エフェクトの再生
	// 現在再生中のエフェクトに設定しておく
	int b = PlayEffekseer3DEffect(effect_handle);
	// エフェクトの再生スピードの設定
	int a =SetSpeedPlayingEffekseer3DEffect(effect_handle, m_effect_info[effect_num].effect_play_speed);

	// エフェクトの生成座標の設定
	SetPosPlayingEffekseer3DEffect(effect_handle, pos.x, pos.y, pos.z);
}

//---------------------------------------------------------------------------
// エフェクトの座標を設定
//---------------------------------------------------------------------------
void Effect::SetEffectPos(int effect_num, Vector3 pos, Vector3 pos2)
{
	// 設定したい座標
	Vector3 pos_ = pos + pos2;
	// エフェクトの設定
	SetPosPlayingEffekseer3DEffect(m_effect_info[effect_num].effect_handle, pos_.x, pos_.y, pos_.z);
}

//---------------------------------------------------------------------------
//  エフェクトの座標をモデルの向きに合わせてずらす関数
//---------------------------------------------------------------------------
void Effect::SetEffectRotPos(int effect_num, Vector3 pos, Vector3 pos2, Vector3 rot)
{
	// 設定したいモデルの向きに合わせて座標をセットする
	Vector3 pos_ = pos;
	pos_.x += pos2.x + sinf(TO_RADIAN(rot.y));
	pos_.y += pos2.y;
	pos_.z += pos2.z + cosf(TO_RADIAN(rot.y));
	// エフェクトの座標を設定
	SetPosPlayingEffekseer3DEffect(m_effect_info[effect_num].effect_handle, pos_.x, pos_.y, pos_.z);
}


//---------------------------------------------------------------------------
//  エフェクトの再生
//---------------------------------------------------------------------------
void Effect::SetEffectRot(int effect_num, Vector3 rot)
{
	// 設定したい向きのを出す
	Vector3 rot_;
	rot_.x = TO_RADIAN(rot.x);
	rot_.y = TO_RADIAN(rot.y);
	rot_.z = TO_RADIAN(rot.z);
	// エフェクトの向きを設定
	SetRotationPlayingEffekseer3DEffect(m_effect_info[effect_num].effect_handle, rot_.x, rot_.y, rot_.z);
}

//---------------------------------------------------------------------------
//  エフェクトの細かいサイズの設定
//---------------------------------------------------------------------------
void Effect::SetEffectSize(int effect_num, Vector3 size)
{
	// エフェクトのサイズを設定
	SetScalePlayingEffekseer3DEffect(m_effect_info[effect_num].effect_handle, size.x, size.y, size.z);
}

//---------------------------------------------------------------------------
//  エフェクトの再生速度の設定
//---------------------------------------------------------------------------
void Effect::SetEffectPlaySpeed(int effect_num, float speed)
{
	SetSpeedPlayingEffekseer3DEffect(m_effect_info[effect_num].effect_handle, speed);
}

//---------------------------------------------------------------------------
//  エフェクトの色の設定
//---------------------------------------------------------------------------
void Effect::SetEffectColor(int effect_num, COLOR_F color)
{
	// エフェクトの色の設定
	SetColorPlayingEffekseer3DEffect(m_effect_info[effect_num].effect_handle, color.r, color.g, color.b, color.a);
}