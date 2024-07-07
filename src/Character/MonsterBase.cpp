#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"
#include "src/System/Spotlight.h"
#include "src/Collision/BoxCollision.h"
#include "src/System/TargetMove.h"

#include "src/Character/MonsterBase.h"


// モンスター専用のキャラベースクラス

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
MonsterBase::MonsterBase()
{

}

//---------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------
MonsterBase::~MonsterBase()
{

}

void MonsterBase::BaseInit()
{
}

//---------------------------------------------------------------------------
// 移動用のあたり判定
//---------------------------------------------------------------------------
//void MonsterBase::MoveHitUpdate(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
//{
//	move.Move_Hit(player_pos, before_pos, hit_size, box);
//}
