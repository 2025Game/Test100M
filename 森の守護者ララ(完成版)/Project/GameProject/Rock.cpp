#include "Rock.h"

#define CHIP_SIZE_W 500		// 1コマのサイズ(幅)
#define CHIP_SIZE_H 600     // 1コマのサイズ(高さ)
#define CENTER_POS CVector2D(0.0f, 300.0f)	// 中心座標

// スライムのアニメーションデータ
TexAnimData Rock::ANIM_DATA[(int)EAnimType::Num] =
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

// コンストラクタ
Rock::Rock(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// スライムの画像を読み込み
	std::string imagePath;
	if (m_type == 0) imagePath = "wall.png";
	else if (m_type == 1) imagePath = "slime_b.png";
	else if (m_type == 2) imagePath = "slime_c.png";
	mp_image = CImage::CreateImage
	(
		imagePath.c_str(),	// 画像ファイルのパス
		ANIM_DATA,			// アニメーションのデータ
		CHIP_SIZE_W, CHIP_SIZE_H		// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation(0);
	mp_image->SetCenter(CENTER_POS);
	mp_image->SetFlipH(true);
}

Rock::~Rock()
{
	// 画像データを削除
	delete mp_image;
}

void Rock::Death()
{
}

void Rock::Update()
{
	if (PUSH(CInput::eButton5))
	{
		//ChangeState(EState::Action);
	}

	// 状態に合わせて、更新処理を切り替える
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	case EState::Action:	StateAction();	break;
	}

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

void Rock::Render()
{
	mp_image->Draw();
}

float Rock::GetChipSizeX()
{
	float aX = (CHIP_SIZE_W * 0.05);
	return aX;
}

float Rock::GetChipSizeY()
{
	float aY = (CHIP_SIZE_H * 0.6);
	return aY;
}

float Rock::GetChipSizeZ()
{
	float aZ = (CHIP_SIZE_H * 0.1);
	return aZ;
}

void Rock::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

void Rock::StateIdle()
{
	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

void Rock::StateAction()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：踏まれたアニメーションを再生
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Action, false);
		m_stateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		/* 一時的に削除
		// アニメーションが終了したら、削除
		if (mp_image->CheckAnimationEnd())
		{
			// 削除フラグを立てる
			Kill();
		}*/
		break;
	}
}


