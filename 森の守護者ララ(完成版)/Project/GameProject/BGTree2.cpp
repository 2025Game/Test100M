#include "BGTree2.h"

#define CHIP_SIZE_W 1920		// 横のサイズ
#define CHIP_SIZE_H 1080		// 縦のサイズ
#define CENTER_POS CVector2D(0.0f, 0.0f)	// 中心座標
#define FIELD_CENTER_Z (SCREEN_HEIGHT * 0.75f)

// スライムのアニメーションデータ
TexAnimData BGTree2::ANIM_DATA[(int)EAnimType::Num] =
{
	{
		new TexAnim[4]
		{
			{ 0, 10}, { 0, 10},
			{ 0, 10}, { 0, 10},
		},
		4
	},
	{
		new TexAnim[2]
		{
			{7, 10}, {8, 10},
		},
		2
	},
};

BGTree2::BGTree2(int type, const CVector3D& pos)
	: ObjectBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	std::string imagePath;
	if (m_type == 0) imagePath = "tree.png",	// 画像ファイルのパス
		// プレイヤーの画像を読み込み
		mp_image = CImage::CreateImage
		(
			imagePath.c_str(),	// 画像ファイルのパス
			ANIM_DATA,		// アニメーションのデータ
			CHIP_SIZE_W, CHIP_SIZE_H	// 1コマの幅と高さ
		);
	//mp_image->ChangeAnimation((int)EAnimType::Idle);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetFlipH(true);
}

BGTree2::~BGTree2()
{
	// 画像データを削除
	delete mp_image;
}

void BGTree2::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	}

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

void BGTree2::Render()
{
	mp_image->Draw();
}

extern float testScrollX;
//int loopN2 = 0;
CVector2D BGTree2::CalcScreenPos(bool grounded) const
{
	CVector2D ret;

	ret.x = ((m_pos.x + (3840 * loopN2)) - (testScrollX * 0.6f));

	// 通常座標を求める場合
	if (!grounded)
	{
		// Y座標は、3次元座標のY（高さ）とZ（奥行）を合わせる
		float posZ = FIELD_CENTER_Z + m_pos.z;
		ret.y = -m_pos.y + posZ;
		ret.y = -m_pos.y;
	}
	// 高さを考慮しない地面の位置を求める場合
	else
	{
		// Y座標は、3次元座標のZ（奥行）のみ反映する
		ret.y = FIELD_CENTER_Z + m_pos.z;
	}

	//if (ret.x < -3840.0f)
	if (ret.x < -1920.0f)
	{
		//loopN2++;
		// `loopN` をメンバ変数として更新
		const_cast<BGTree2*>(this)->loopN2++;
	}

	return ret;
}


void BGTree2::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

void BGTree2::StateIdle()
{
	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}