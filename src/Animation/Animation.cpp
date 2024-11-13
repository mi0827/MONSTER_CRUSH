#include "src/WinMain.h"
#include "src/Model/Model.h"
#include "Animation.h"
using namespace std;

//-----------------------------------------------
// コンストラクタ
//-----------------------------------------------
Animation::Animation()
{
	m_contexts[0].is_playing = false;
}

//-----------------------------------------------
// デストラクタ
//-----------------------------------------------
Animation::~Animation()
{
	anim_handle.clear();

	// 下の二つのdeleteはエラーが出る
	//delete[] (m_contexts);
	//delete m_contexts;
}

//-----------------------------------------------
// アニメーションの情報の初期化関数
//-----------------------------------------------
void Animation::InformationInit(int num)
{
	//アニメーションの情報をすべて初期化しておく
	m_contexts[num].is_playing = false;
	m_contexts[num].is_loop = false;
	m_contexts[num].animation_index = -1;
	m_contexts[num].animation_attach_index = -1;
	m_contexts[num].m_blend_ratio = 0.0f;
	m_contexts[num].animation_total_time = 0.0;
	m_contexts[num].play_time = 0.0f;
}

//-----------------------------------------------
// アニメーションの初期設定
//-----------------------------------------------
void Animation::InitAnimation(int anim_max, int default_anim)
{
	// デフォルトアニメーションの設定
	m_default_anim = default_anim;
	// アニメーションの入れ物を配列で確保する
	for (int i = 0; i < anim_max; i++)
	{
		AnimData* anim = new AnimData;
		anim_handle.push_back(anim);
	}
}

//-----------------------------------------------
// 読み込み
//-----------------------------------------------
void Animation::LoadAnimation(const char file_path[256], int anim_no, int anim_index, float anim_play_speed)
{
	// 読み込み
	anim_handle[anim_no]->handle = MV1LoadModel(file_path);
	// アニメションインデックス番号の保存
	anim_handle[anim_no]->index = anim_index;
	// アニメーションの再生速度の設定
	anim_handle[anim_no]->play_speed = anim_play_speed;
}

//-----------------------------------------------
// 最初の初期化の時だけ同じアニメーションをセットする
//-----------------------------------------------
void Animation::InitAttachAnimation(Model* model, int anim_num, bool loop)
{
	for (int i = 0; i < 2; i++) {
		// 入れ物を初期化しておく
		InformationInit(i);

		// アニメションのセット
		m_contexts[i].animation_attach_index = MV1AttachAnim
		(
			model->m_model,
			anim_handle[anim_num]->index,
			anim_handle[anim_num]->handle
		);

		// アニメーション総再生フレームの取得
		m_contexts[i].animation_total_time = MV1GetAttachAnimTotalTime
		(
			model->m_model,
			m_contexts[i].animation_attach_index
		);

		// ブレンド率の取得
		m_contexts[i].m_blend_ratio = MV1GetAttachAnimBlendRate
		(
			model->m_model,
			m_contexts[i].animation_attach_index
		);

		if (i == 0) {
			m_contexts[0].m_blend_ratio = 1.0f;
		}
		else
		{
			m_contexts[1].m_blend_ratio = 0.0f;
		}

		// ループするかどうか
		m_contexts[i].is_loop = loop;
	}
	m_detach_flag = true;
	// どのアニメーションかを保存
	m_anim_num = anim_num;
}

//-----------------------------------------------
// アニメーションのセット
//-----------------------------------------------
void Animation::AttachAnimation(Model* model, int anim_num, bool loop)
{
	// 入れ物を初期化しておく
	InformationInit(0);

	// アニメションのセット
	m_contexts[0].animation_attach_index = MV1AttachAnim
	(
		model->m_model,
		anim_handle[anim_num]->index,
		anim_handle[anim_num]->handle
	);

	// アニメーション総再生フレームの取得
	m_contexts[0].animation_total_time = MV1GetAttachAnimTotalTime
	(
		model->m_model,
		m_contexts[0].animation_attach_index
	);

	// ブレンド率の取得
	m_contexts[0].m_blend_ratio = MV1GetAttachAnimBlendRate
	(
		model->m_model,
		m_contexts[0].animation_attach_index
	);
	// ブレンド率をゼロからスタートする
	m_contexts[0].m_blend_ratio = 0.2;
	// ループするかどうか
	m_contexts[0].is_loop = loop;

	// アニメーションを再生中にする
	//m_contexts[0].is_playering = true;

	// どのアニメーションかを保存
	m_anim_num = anim_num;
}

// アニメーションの取り外し
void Animation::DetachAnimation(Model* model)
{
	// アニメーションをの取り外し
	m_contexts[1].animation_attach_index = MV1DetachAnim
	(model->m_model,
		m_contexts[1].animation_attach_index);
	// 取り外したアニメーションの情報をすべて初期化しておく
	InformationInit(1);
	// デタッチされたからフラグを下げる
	m_detach_flag = false;
}

//-----------------------------------------------
// ブレンド率の変更をする関数
//-----------------------------------------------
void Animation::ChangeBlend()
{
	bool blend_flag1 = false;
	bool blend_flag2 = false;
	// 次のアニメーションのブレンド率を上げる
	m_contexts[0].m_blend_ratio += BLEND_VALUE;
	// 上限の設定
	if (m_contexts[0].m_blend_ratio >= BLEND_VALUE_MAX)
	{
		m_contexts[0].m_blend_ratio = BLEND_VALUE_MAX;
		blend_flag1 = true;
	}
	// もともとのアニメーションのブレンド率を下げる
	m_contexts[1].m_blend_ratio -= BLEND_VALUE;
	// 下限の設定
	if (m_contexts[1].m_blend_ratio <= BLEND_VALUE_MIN)
	{
		m_contexts[1].m_blend_ratio = BLEND_VALUE_MIN;
		blend_flag2 = true;

	}

	// ブレンドフラグが両方立っていたら
	if (blend_flag1 && blend_flag2)
	{
		// ブレンド率が下がったのでフラグを下げる
		m_attached = false;
	}
}

//-----------------------------------------------
// アニメーションの変更をしていいかを判断する関数
//-----------------------------------------------
bool Animation::ChangeFlag(bool flag1)
{
	// キャラクターの各フラグと
	// アニメーション変更フラグが両方上がっていたら
	if (flag1 == true && m_anim_change_flag == true)
	{
		// 変更の許可を出す
		return true;
	}
	else // どちらか一方でもフラグが立っていなかったら
	{
		// アニメーションの変更はさせない
		return false;
	}

}

//-----------------------------------------------
// アニメーション切り替え用関数
//-----------------------------------------------
void Animation::ChangeAnimation(Model* model, int anim_num, bool loop)
{
	// アニメーション変更フラグを下げる
	m_anim_change_flag = false;
	// アニメーションを入れ替える前にもともとついていアニメーションを取り外す
	DetachAnimation(model);
	// 中にはいている情報を入れ替える
	m_contexts[1] = m_contexts[0];
	// 二つ付入れているからフラグを上げる
	m_attached = true;
	// 二つ目のアニメーションが付いているから
	m_detach_flag = true;
	// 次に行いたいアニメーションをつける
	AttachAnimation(model, anim_num, loop);
}

//-----------------------------------------------
// アニメーション切り替え用関数（アニメーションが終わるタイミングで切り替える）
//-----------------------------------------------
void Animation::ActionComboChangeAnimation(Model* model, int anim_num, bool loop, bool* combo_flag)
{
	// アニメーションが最後のフレームに差し掛かったら
	if (combo_flag)
	{
		if (m_contexts[0].animation_total_time <= m_contexts[0].play_time)
		{
			/*if (anim_num== 7)
			{
				Change_Animation(model, anim_num, loop);
			}*/
			ChangeAnimation(model, anim_num, loop);
			// 次のアニメーションがついたからコンボフラグを下す
			*combo_flag = false;
		}
	}
	
}

//-----------------------------------------------
// アニメーションを再生させるための関数
//-----------------------------------------------
void Animation::PlayAnimation(Model* model, bool combo_flag)
{
	m_playing_anim = true;
	// アニメーションを再生中にしておく
	if (m_contexts[0].play_time <= m_contexts[0].animation_total_time)
	{
		m_contexts[0].is_playing = true;
	}
	// アニメーションの再生フレームを進める
	m_contexts[0].play_time += anim_handle[m_anim_num]->play_speed;
	// ループするかどうかによって処理の変更

	if (m_contexts[0].is_loop)
	{
		LoopAnimation();
	}
	else // ループしない場合
	{
		if (combo_flag == false)
		{
			NotLoop(model);
		}
	}


	// ブレンド率の変更
	ChangeBlend();

	// 二つ付いているときとそうでないときで処理を変える
	if (m_attached)
	{
		for (int i = 0; i < 2; i++)
		{
			// それぞれにアニメーションの割合分再生します
			// ブレンド率の設定
			MV1SetAttachAnimBlendRate(
				model->m_model,
				m_contexts[i].animation_attach_index,
				m_contexts[i].m_blend_ratio);
			// アニメーションの再生
			MV1SetAttachAnimTime
			(
				model->m_model,
				m_contexts[i].animation_attach_index,
				m_contexts[i].play_time
			);
		}
	}
	else
	{
		if (m_detach_flag)
		{
			// デタッチされてなっかたら
			// デタッチする
			DetachAnimation(model);
		}
		// アニメーションの再生
		MV1SetAttachAnimTime
		(
			model->m_model,
			m_contexts[0].animation_attach_index,
			m_contexts[0].play_time
		);
	}
}

//-----------------------------------------------
// アニメーションをループさせる
//-----------------------------------------------
void Animation::LoopAnimation()
{
	// アニメーションフレームが最後まで回ったら
	if (m_contexts[0].play_time >= m_contexts[0].animation_total_time)
	{
		// 最初にリセットする
		m_contexts[0].play_time = 0.0f;
		// アニメーション再生外にする
		m_contexts[0].is_playing = false;
	}
}


//-----------------------------------------------
// アニメーションをループさせ無い場合
//-----------------------------------------------
void Animation::NotLoop(Model* model)
{
	// アニメーションフレームが最後まで回ったら
	if (m_contexts[0].play_time >= m_contexts[0].animation_total_time)
	{
		//m_playing_anim = false;
		// アニメーション再生外にする
		m_contexts[0].is_playing = false;
		// 最初にリセットする
		m_contexts[0].play_time = 0.0f;

		// デフォルトのアニメーションに設定
		ChangeAnimation(model, m_default_anim, true);
	}

}


