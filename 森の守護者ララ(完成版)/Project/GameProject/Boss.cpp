#include "Boss.h"

#define CHIP_SIZE_W 1750	// 1コマのサイズ(幅)
#define CHIP_SIZE_H 950     // 1コマのサイズ(高さ)

#define CENTER_POS CVector2D(0.0f, 300.0f)	// 中心座標
#define MOVE_SPEED_X 5.0f	// 横方向の移動速度
#define GRAVITY -0.23f		// 重力

// プレイヤーのアニメーションデータの前宣言
TexAnimData Boss::ANIM_DATA[(int)EAnimType::Num] =
{
	// ダッシュアニメーション
	{
		new TexAnim[5]
		{
			{0, 6}, {1, 6}, {2, 6},
			{3, 6}, {4, 6}
		},
		5
	},
	// ジャンプアニメーション
	{
		new TexAnim[4]
		{
			{27, 2}, {28, 2}, {29, 2},
			{30, 2}
		},
		4
	},
	// 死亡アニメーション
	{
		new TexAnim[2]
		{
			{45, 30},
			{46, 30},
		},
		2
	},
	// 攻撃アニメーション
	{
		new TexAnim[2]
		{
			{18, 20},
			{19, 20},
		},
		2
	},
	// 被ダメアニメーション
	{
		new TexAnim[3]
		{
			{5, 20},
			{6, 20},
			{7, 20}
		},
		3
	},
};


// コンストラクタ
Boss::Boss(const CVector3D& pos)
	: CharaBase(pos)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
	, m_isInvincible(false)
	, m_invincibleTime(0.0f)
{
	m_hp = 20; // UI に合わせて初期HPを3に
	s_isMove = false;
	// HPを共有
	sHp = m_hp;
	//

	// プレイヤーの画像を読み込み
	mp_image = CImage::CreateImage
	(
		"dragon.png",	// 画像ファイルのパス
		ANIM_DATA,		// アニメーションのデータ
		CHIP_SIZE_W, CHIP_SIZE_H	// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation((int)EAnimType::Dash);
	mp_image->SetCenter(CENTER_POS);
}

// デストラクタ
Boss::~Boss()
{
	// 画像データを削除
	delete mp_image;
}

// 現在の状態を切り替え
void Boss::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 移動処理の更新
bool Boss::UpdateMove()
{
	// 右方向へ移動
	m_pos.x += MOVE_SPEED_X;
	mp_image->SetFlipH(false);
	return true;
}

// ダッシュ時の更新処理
void Boss::StateIdle()
{
	// 移動処理
	bool isMove = UpdateMove();

	// ダッシュアニメーションに変更
	mp_image->ChangeAnimation((int)EAnimType::Dash);
}

void Boss::StateJump()
{
}

void Boss::StateAttack()
{
}

// 死亡時の更新処理
void Boss::StateDeath()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：被ダメアニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);
		m_stateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 被ダメアニメーションが終了したら、死亡状態へ移行
		if (mp_image->CheckAnimationEnd())
		{
			m_stateStep++;
		}
		break;
		// ステップ0：死亡アニメーションに切り替え
	case 2:
		printf("Hello, World!\n");
		mp_image->ChangeAnimation((int)EAnimType::Death, false);
		m_stateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 3:
		// 被ダメアニメーションが終了したら、ゲームオーバー画面へ移行
		if (mp_image->CheckAnimationEnd())
		{
			//Death();
		}
		break;
	}
}

// 被ダメ中の更新処理
void Boss::StateDamage()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：被ダメアニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);
		m_stateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 被ダメアニメーションが終了したら、待機状態へ移行
		if (mp_image->CheckAnimationEnd())
		{
			ChangeState(EState::Idle);
		}
		break;
	}
}

// ダメージを受ける処理
void Boss::TakeDamage(int damage)
{
	if (m_isInvincible) return; // 無敵中なら何もしない

	if (m_hp > damage)
	{
		m_hp -= damage;					// HPを減らす

		// HPを共有
		sHp = m_hp;
		m_isInvincible = true;			// 無敵状態に設定
		m_invincibleTime = 3.0f;		// 無敵時間を3秒に設定
		ChangeState(EState::Damage);	// 被ダメ状態へ移行
	}
	else
	{
		// HPを0にして、被ダメアニメーション+死亡アニメーションに切り替え
		m_hp = 0;
		sHp = m_hp; // HPを共有
		ChangeState(EState::Death); // 死亡状態へ移行
	}
}

// HPを取得
int Boss::sHp = 0; // HP


int Boss::Hp()
{
	return sHp;
}

bool Boss::s_isMove = false;
bool Boss::IsMove()
{
	return s_isMove;
}

extern float testScrollX;

// 更新処理
void Boss::Update()
{
	// 無敵状態の時間をカウントダウン
	if (m_isInvincible)
	{
		m_invincibleTime -= 1.0f / 60.0f; // 1フレームごとに時間を減少 (60FPS)
		if (m_invincibleTime <= 0.0f)
		{
			m_isInvincible = false; // 無敵解除
		}
	}

	// 現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Jump:		StateJump();	break;
	case EState::Attack:	StateAttack();	break;
	case EState::Death:		StateDeath();	break;
	case EState::Damage:    StateDamage();  break;
	}

	m_pos.x = testScrollX;

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// 描画処理
void Boss::Render()
{
	mp_image->Draw();
}