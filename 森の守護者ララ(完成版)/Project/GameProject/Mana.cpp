#include "Mana.h"

#define CHIP_SIZE_W 211		// 1コマのサイズ(幅)
#define CHIP_SIZE_H 211     // 1コマのサイズ(高さ)
#define CENTER_POS CVector2D(0.0f, 50.0f)	// 中心座標

// マナのアニメーションデータ
TexAnimData Mana::ANIM_DATA[(int)EAnimType::Num] =
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
Mana::Mana(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// マナの画像を読み込み
	std::string imagePath;
	if (m_type == 0) imagePath = "mana.png";
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

Mana::~Mana()
{
	// 画像データを削除
	delete mp_image;
}

void Mana::Death()
{
}

void Mana::Update()
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

void Mana::Render()
{
	mp_image->Draw();
}

float Mana::GetChipSizeX()
{
	float aX = (CHIP_SIZE_W * 0.05);
	return aX;
}

float Mana::GetChipSizeY()
{
	float aY = (CHIP_SIZE_H * 0.6);
	return aY;
}

float Mana::GetChipSizeZ()
{
	float aZ = (CHIP_SIZE_H * 0.1);
	return aZ;
}

void Mana::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

void Mana::StateIdle()
{
	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

void Mana::StateAction()
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

void Mana::Alpha0()
{
	mp_image->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
}
