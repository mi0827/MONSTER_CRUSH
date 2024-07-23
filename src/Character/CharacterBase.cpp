#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"
#include "src/System/Spotlight.h"

#include "src/Model/Model.h"

#include "src/Collision/BoxCollision.h"
#include "src/Collision/CapsuleCollision.h"
#include "src/System/Move.h"

#include "CharacterBase.h"

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
CharacterBase::CharacterBase()
{

}

//---------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------
CharacterBase::~CharacterBase()
{

}

//---------------------------------------------------------------------------
// 移動用のあたり判定
//---------------------------------------------------------------------------
void CharacterBase::MoveHitUpdate(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
{
	m_move_hit_box.CreateBox(*player_pos, *hit_size);
	m_move.Move_Hit(player_pos, before_pos, hit_size, box);
}

//---------------------------------------------------------------------------
// ベースクラスでの初期処理
//---------------------------------------------------------------------------
void CharacterBase::BaseInit(float r)
{
	// 座標、向き、回転の初期化
	m_transform.pos.clear();
	// 当たり判定の半径の設定
	m_hit_r = r;
}

//---------------------------------------------------------------------------
//  ベースクラスでの更新処理
//---------------------------------------------------------------------------
void CharacterBase::BaseUpdate(bool* m_check_move, Vector3* camera_rot, const float* mov_speed)
{
	// 移動の処理
	m_move.Update(m_check_move, camera_rot, &m_transform.rot, mov_speed, &m_transform.pos, m_pad_no, m_mov);
}
