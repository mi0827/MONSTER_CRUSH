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

//---------------------------------------------------------------------------
// ベースクラスの初期処理
//---------------------------------------------------------------------------
void MonsterBase::BaseInit(Vector3 target_pos,float m_target_hit_r)
{

	//引数の型が合わないから一つ一つ引数で渡してsetInfoの中で構造体の中に入れる
	// 引数を受け取るところは考え中
	//

	move.SetInfo(m_info, &targetinfo);
}

//---------------------------------------------------------------------------
// ベースクラスの更新処理
//---------------------------------------------------------------------------
void MonsterBase::BaseUpdate()
{
	// 移動処理
	move.Update();
}

//---------------------------------------------------------------------------
// 移動用のあたり判定
//---------------------------------------------------------------------------
//void MonsterBase::MoveHitUpdate(Vector3* player_pos, Vector3* before_pos, Vector3* hit_size, BoxCollision* box)
//{
//	move.Move_Hit(player_pos, before_pos, hit_size, box);
//}
