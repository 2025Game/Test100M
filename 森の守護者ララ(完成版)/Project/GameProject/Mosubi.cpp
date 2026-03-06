#include "Mosubi.h"

#define CHIP_SIZE_W 400		// 1コマのサイズ(幅)
#define CHIP_SIZE_H 400     // 1コマのサイズ(高さ)
//#define CENTER_POS CVector2D(128.0f, 340.0f)	// 中心座標
#define CENTER_POS CVector2D(0.0f, 300.0f)	// 中心座標

// モスビのアニメーションデータ
TexAnimData Mosubi::ANIM_DATA[(int)EAnimType::Num] =
{
	{
		new TexAnim[4]
		{
			{ 0, 10}, { 1, 10},
			{ 2, 10}, { 3, 10},
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
Mosubi::Mosubi(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// モスビの画像を読み込み
	std::string imagePath;
	if (m_type == 0) imagePath = "mosubi.png";
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

// デストラクタ
Mosubi::~Mosubi()
{
	// 画像データを削除
	delete mp_image;
}

// 当たった処理
void Mosubi::Death()
{
	// 当たった状態へ移行
	ChangeState(EState::Action);
}

// 現在の状態を切り替え
void Mosubi::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 待機時の更新処理
void Mosubi::StateIdle()
{
	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

// 当たった時の更新処理
void Mosubi::StateAction()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：当たったアニメーションを再生
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

// 更新処理
void Mosubi::Update()
{
	if (PUSH(CInput::eMouseR))
	{
		ChangeState(EState::Action);
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

// 描画処理
void Mosubi::Render()
{
	mp_image->Draw();
}

float Mosubi::GetChipSizeX()
{
	float aX = (CHIP_SIZE_W * 0.5);
	return aX;
}

float Mosubi::GetChipSizeY()
{
	float aY = (CHIP_SIZE_H * 0.4);
	return aY;
}

float Mosubi::GetChipSizeZ()
{
	float aZ = (CHIP_SIZE_H * 0.3);
	return aZ;
}