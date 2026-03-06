#include "Potion.h"

#define CHIP_SIZE_W 172		// 1コマのサイズ(幅)
#define CHIP_SIZE_H 173     // 1コマのサイズ(高さ)
#define CENTER_POS CVector2D(0.0f, 50.0f)	// 中心座標

// ポーションのアニメーションデータ
TexAnimData Potion::ANIM_DATA[(int)EAnimType::Num] =
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
Potion::Potion(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// ポーションの画像を読み込み
	std::string imagePath;
	if (m_type == 0) imagePath = "potion1.png";
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

Potion::~Potion()
{
	// 画像データを削除
	delete mp_image;
}

void Potion::Death()
{
}

void Potion::Update()
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

void Potion::Render()
{
	mp_image->Draw();
}

float Potion::GetChipSizeX()
{
	float aX = (CHIP_SIZE_W * 0.20);
	return aX;
}

float Potion::GetChipSizeY()
{
	float aY = (CHIP_SIZE_H * 1.0);
	return aY;
}

float Potion::GetChipSizeZ()
{
	float aZ = (CHIP_SIZE_H * 1.0);
	return aZ;
}

void Potion::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

void Potion::StateIdle()
{
	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

void Potion::StateAction()
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

void Potion::Alpha0()
{
	mp_image->SetColor(1.0f,1.0f, 1.0f,0.0f);
}
