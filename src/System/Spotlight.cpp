#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Spotlight.h"

//---------------------------------------
// コンストラクタ(初期化)
//---------------------------------------
Spotlight::Spotlight()
{
	
}

//---------------------------------------
// デストラクター
//---------------------------------------
Spotlight::~Spotlight()
{
	// ライトの削除
	DeleteLightHandle(m_light_handle);
}

void Spotlight::InitSetLight(Vector3 pos, Vector3 spot_pos, float outAngle, float inangle, float range, float atten0, float atten1, float atten2)
{
	// ライトの作成

	m_light_handle = CreateSpotLightHandle(
		pos.VGet(),
		spot_pos.VGet(),
		TO_RADIAN(outAngle), TO_RADIAN(inangle), range, atten0, atten1, atten2);
}

//---------------------------------------
// ライトの座標設定用関数
//---------------------------------------
void Spotlight::SetLightPos(Vector3* pos)
{
	// ライトの座標設定
	SetLightPositionHandle(m_light_handle, pos->VGet());
}

//---------------------------------------
// ライトの照らす方向設定用関数
//---------------------------------------
void Spotlight::SetLightSpotPos(Vector3* spot_pos)
{
	// ライトの向きの設定
	SetLightDirectionHandle(m_light_handle, spot_pos->VGet());
}

//---------------------------------------
// ライトの座標関連の更新処理
//---------------------------------------
void Spotlight::UpdateTransform(Vector3* pos, Vector3* spot_pos)
{
	// 座標の設定
	SetLightPos(pos);
	// ライトの向きの設定
	SetLightSpotPos(spot_pos);

}
