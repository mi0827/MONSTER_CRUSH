#include "src/WinMain.h"

#include "src/Model/Model.h"

#include "src/Collision/CapsuleCollision.h"
#include "src/Collision/BoxCollision.h"
#include "src/Hit/Hit.h"
#include "src/Animation/Animation.h"
#include "src/Action/Attack.h"


bool Attack::HitAttack(CapsuleCollision capsule1, CapsuleCollision capsule2, Animation anim)
{

	// 攻撃アニメーションが終わる寸前でhit フラグを下げる
// 本来は寸前でなく終わったらのほうが良いがanimationクラスの書き方が悪く
// アニメーションの終わる瞬間が取れないのでこのような形になっている
// -5なのは物によってはアニメーションの進むスピードが違うので今のところこれになっている
// 上記の書き方でもうまくいっていないこのif分に入るときと入らないときがある
// コンボをつなぐと入る
// 本来はanim.m_contexts[0].is_playingで判定をとりたいがうまくいかない
// プログラミングの順番的にはうまくいくはずなのにうまくいかない
	if (anim.m_contexts[0].play_time >= anim.m_contexts[0].animation_total_time -1 /*!anim.m_contexts[0].is_playing*/)
	{
		// 当たり判定がないときはfalseにしておく
		hit = false;
	}
	// 指定のアニメーションの再生中に
	if (anim.m_contexts[0].play_time < anim.m_contexts[0].animation_total_time)
	{
		// 指定のカプセル同士の当たり判定があったとき
		if (CheckCapsuleHit(capsule1, capsule2))
		{
			if (!hit)
			{
				hit = true;
				// 当たり判定があってなおかつ初めてこのif分に入ったときだけ
				// trueを返してダメージ計算をさせる
				return true;
			}
			return false;
		}
	}
}
