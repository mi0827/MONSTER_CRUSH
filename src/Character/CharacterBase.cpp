#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"
#include "src/System/Spotlight.h"
#include "src/Collision/BoxCollision.h"
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

void CharacterBase::MoveHitUpdate(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
{
	move.Move_Hit(player_pos, before_pos, hit_size, box);
}

//---------------------------------------------------------------------------
// ベースクラスでの初期処理
//---------------------------------------------------------------------------
void CharacterBase::BaseInit()
{
	// 座標、向き、回転の初期化
	transform.pos.clear();
}

//---------------------------------------------------------------------------
//  ベースクラスでの更新処理
//---------------------------------------------------------------------------
void CharacterBase::BaseUpdate(bool* m_check_move, Vector3* camera_rot, const float* mov_speed)
{
	// 移動の処理
	move.Update(m_check_move, camera_rot, &transform.rot, mov_speed, &transform.pos, m_pad_no, m_mov);
}
