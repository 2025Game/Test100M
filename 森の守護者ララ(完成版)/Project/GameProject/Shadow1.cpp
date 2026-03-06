#include "Shadow1.h"

#define CHIP_SIZE_W 259		// 1コマのサイズ(幅)
#define CHIP_SIZE_H 259     // 1コマのサイズ(高さ)
#define CENTER_POS CVector2D(-100.0f, 300.0f)	// 中心座標

// スライムのアニメーションデータ
TexAnimData Shadow1::ANIM_DATA[(int)EAnimType::Num] =
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
Shadow1::Shadow1(int type, const CVector3D& pos)
	: EnemyBase(pos)
	, mp_image(nullptr)
	, m_type(type)
{
	m_hp = 200;

	// スライムの画像を読み込み
	std::string imagePath;
	if (m_type == 0) imagePath = "shadow.png";
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

Shadow1::~Shadow1()
{
	// 画像データを削除
	delete mp_image;
}

void Shadow1::Death()
{
}

extern float testScrollX; // プレイヤー追従

void Shadow1::Update()
{
	// 上キーを押した瞬間にZ座標を減らす（奥へ移動）
	if (PUSH(CInput::eUp) && m_pos.z > -230.0f)
	{
		m_pos.z -= 150.0f;
	}
	// 下キーを押した瞬間にZ座標を増やす（手前へ移動）
	else if (PUSH(CInput::eDown) && m_pos.z < 0.0f)
	{
		m_pos.z += 150.0f;
	}

	// 状態に合わせて、更新処理を切り替える
	switch (m_state)
	{
	case EState::Idle:	StateIdle();	break;
	case EState::Action:	StateAction();	break;
	}

	m_pos.x = testScrollX; // プレイヤーの位置によって影移動

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

void Shadow1::Render()
{
	mp_image->Draw();
}

void Shadow1::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

void Shadow1::StateIdle()
{
	// 待機アニメーションを再生
	mp_image->ChangeAnimation((int)EAnimType::Idle);
}

void Shadow1::StateAction()
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


