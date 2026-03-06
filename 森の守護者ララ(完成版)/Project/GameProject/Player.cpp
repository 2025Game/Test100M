#include "Player.h"

#define CHIP_SIZE 430		// 1コマのサイズ
//#define CENTER_POS CVector2D(1000.0f, 400.0f)	// 中心座標
#define CENTER_POS CVector2D(0.0f, 300.0f)	// 中心座標
#define MOVE_SPEED_X 5.0f	// 横方向の移動速度
#define MOVE_SPEED_Z 150.0f	// 奥方向の移動速度
#define JUMP_SPEED 15.0f	// ジャンプ速度
//#define GRAVITY -0.175f		// 重力
#define GRAVITY -0.45f		// 重力


bool m_slowDownAfterSliding = false;		// スライディング後にゆっくり移動するフラグ
float moveSpeed;
// 毎フレームでのスピードアップ量
float speedUpPerFrame = 10.0f / (300.0f * 60.0f); // 1フレームごとの増加量

// プレイヤーのアニメーションデータの前宣言
TexAnimData Player::ANIM_DATA[(int)EAnimType::Num] =
{
	// ダッシュアニメーション
	{
		new TexAnim[6]
		{
			{0, 6}, {1, 6}, {2, 6},
			{3, 6}, {4, 6}, {5, 6}
		},
		6
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
			{18, 10},
			{19, 10},
		},
		2
	},
	// スライディングアニメーション
	{
		new TexAnim[5]
		{
			{9, 6},
			{10, 10},
			{11, 60},
			{12, 20},
			{13, 6},
		},
		5
	},
	// 被ダメアニメーション
	{
		new TexAnim[2]
		{
			{36, 10},
			{37, 10},
		},
		2
	},
};


// コンストラクタ
Player::Player(const CVector3D& pos)
	: CharaBase(pos)
	, m_state(EState::Idle)
	, m_stateStep(0)
	, mp_image(nullptr)
	, m_isInvincible(false)
	, m_invincibleTime(0.0f)
	, m_isFloat(false)
	, m_floatTime(0.0f)
{
	m_hp = 3; // UI に合わせて初期HPを3に
	moveSpeed = MOVE_SPEED_X;
	s_isMove = false;
	// HPを共有
	sHp = m_hp;
	//

	// プレイヤーの画像を読み込み
	mp_image = CImage::CreateImage
	(
		"エルフアニメーション.png",	// 画像ファイルのパス
		ANIM_DATA,		// アニメーションのデータ
		CHIP_SIZE, CHIP_SIZE	// 1コマの幅と高さ
	);
	mp_image->ChangeAnimation((int)EAnimType::Dash);
	mp_image->SetCenter(CENTER_POS);

	// 追加
	// ジャンプSE
	SOUND("jump_se")->Load("jump.wav", 1, false);
	// 回復SE
	SOUND("kaifuku_se")->Load("kaifuku.wav", 1, false);
	// 被ダメSE
	SOUND("damage_se")->Load("damage.wav", 1, false);
	// 攻撃SE
	SOUND("attack_se")->Load("attack.wav", 1, false);
	// スライディングSE
	SOUND("sliding_se")->Load("sliding.wav", 1, false);
	// ジャンプSE
	SOUND("jump_se")->Volume(0.1f);  // 01%の音量
	// 回復SE
	SOUND("kaifuku_se")->Volume(0.1f);  // 01%の音量
	// 被ダメSE
	SOUND("damage_se")->Volume(0.1f);  // 01%の音量
	// 攻撃SE
	SOUND("attack_se")->Volume(0.1f);  // 01%の音量
	// スライディングSE
	SOUND("sliding_se")->Volume(0.1f);  // 01%の音量
}

// デストラクタ
Player::~Player()
{
	// 画像データを削除
	delete mp_image;
}

void Player::Attack()
{
	ChangeState(EState::Attack);
}

// 現在の状態を切り替え
void Player::ChangeState(EState state)
{
	if (m_state == state) return;

	m_state = state;
	m_stateStep = 0;
}

// 移動処理の更新
bool Player::UpdateMove()
{
	bool isMove = false;

	// ジャンプ中はレーン移動を無効化
	if (m_state == EState::Jump)
	{
		// 上キーを押した瞬間にZ座標を減らす（奥へ移動）
		if (PUSH(CInput::eUp) && m_pos.z > -230.0f)
		{
			m_pos.z -= MOVE_SPEED_Z;
			isMove = true;
		}
		// 下キーを押した瞬間にZ座標を増やす（手前へ移動）
		else if (PUSH(CInput::eDown) && m_pos.z < 70.0f)
		{
			m_pos.z += MOVE_SPEED_Z;
			isMove = true;
		}
	}
	else
	{
		// 上キーを押した瞬間にZ座標を減らす（奥へ移動）
		if (PUSH(CInput::eUp) && m_pos.z > -230.0f)
		{
			m_pos.z -= MOVE_SPEED_Z;
			isMove = true;
		}
		// 下キーを押した瞬間にZ座標を増やす（手前へ移動）
		else if (PUSH(CInput::eDown) && m_pos.z < 70.0f)
		{
			m_pos.z += MOVE_SPEED_Z;
			isMove = true;
		}
	}

	// スライディング中は速く移動
	if (m_state == EState::Sliding)
	{
		//m_pos.x += moveSpeed; // 速く移動
		isMove = true;
	}
	// スライディング後は移動速度をゆっくりにする
	else if (m_slowDownAfterSliding)
	{
		m_pos.x += moveSpeed; // スライディング後の移動速度
		isMove = true;
	}

	// 通常の右移動
	else
	{
		//m_pos.x += MOVE_SPEED_X;
		//isMove = true;

		// 右方向へ移動
		m_pos.x += moveSpeed;
		mp_image->SetFlipH(false);
		isMove = true;
	}

	//s_isMove = isMove;

	return isMove;
}

// ダッシュ時の更新処理
void Player::StateIdle()
{
	// 移動処理
	bool isMove = UpdateMove();

	// ダッシュアニメーションに変更
	mp_image->ChangeAnimation((int)EAnimType::Dash);

	// [SPACE]キーでジャンプ状態へ移行
	if (PUSH(CInput::eButton5))
	{
		ChangeState(EState::Jump);
	}
	// [X]キーで攻撃状態へ移行
	//else if (PUSH(CInput::eButton2))
	//{
		//ChangeState(EState::Attack);
	//}
	// [C]キーでスライディング状態へ移行
	else if (PUSH(CInput::eButton3))
	{
		ChangeState(EState::Sliding);
	}
}

// ジャンプ中の更新処理
void Player::StateJump()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：ジャンプ開始
	case 0:
		// Y軸（高さ）の移動速度にジャンプを速度を設定し、
		// 接地状態を解除する
		m_moveSpeedY = JUMP_SPEED;
		m_isGrounded = false;

		// ジャンプアニメーションに変更
		mp_image->ChangeAnimation((int)EAnimType::Jump, false);

		// 追加
		// ジャンプSE再生
		SOUND("jump_se")->Play(false);
		//

		m_stateStep++;
		break;
		// ステップ1：ジャンプ終了
	case 1:
		// 接地したら、待機状態へ移行
		if (m_isGrounded)
		{
			ChangeState(EState::Idle);
		}
		break;
	}

	// 移動処理
	bool isMove = UpdateMove();

	if (!isMove)
	{
		mp_image->ChangeAnimation((int)EAnimType::Jump);
	}

	// 以下だと、ジャンプのアニメーションが動かない為、上記に変更
	//mp_image->ChangeAnimation((int)EAnimType::Dash);
}

// 攻撃中の更新処理
void Player::StateAttack()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：攻撃アニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Attack, false);

		// 追加
		// 攻撃SE再生
		SOUND("attack_se")->Play(false);
		//

		m_stateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 攻撃アニメーションが終了したら、待機状態へ移行
		if (mp_image->CheckAnimationEnd())
		{
			ChangeState(EState::Idle);
		}
		break;
	}
	// 移動処理
	bool isMove = UpdateMove();
}

// 死亡時の更新処理
void Player::StateDeath()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：被ダメアニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);

		// 追加
		// 被ダメSE再生
		SOUND("damage_se")->Play(false);
		//

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
		//printf("Hello, World!\n");
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

// スライディング時の更新処理
void Player::StateSliding()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：スライディングアニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Sliding, false);

		// 追加
		// スライディングSE再生
		SOUND("sliding_se")->Play(false);
		//

		m_stateStep++;
		break;

		// ステップ1：アニメーション終了待ち
	case 1:
		// [SPACE]キーでジャンプ状態へ移行
		if (PUSH(CInput::eButton5))
		{
			ChangeState(EState::Jump);
		}

		// スライディング中の移動（速いスピードで右へ移動）
		m_pos.x += moveSpeed;

		// スライディングアニメーションが終了したら、ゆっくり移動を始める
		if (mp_image->CheckAnimationEnd())
		{
			// スライディング後は移動速度を遅くする
			//m_slowDownAfterSliding = true;
			ChangeState(EState::Idle);
		}
		break;
	}
	bool isMove = UpdateMove();
}

// 被ダメ中の更新処理
void Player::StateDamage()
{
	// ステップごとに処理を切り替え
	switch (m_stateStep)
	{
		// ステップ0：被ダメアニメーションに切り替え
	case 0:
		mp_image->ChangeAnimation((int)EAnimType::Damage, false);

		// 追加
		// 被ダメSE再生
		SOUND("damage_se")->Play(false);
		//

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
void Player::TakeDamage(int damage)
{
	if (m_isInvincible) return; // 無敵中なら何もしない

	if (m_hp > damage)
	{
		m_hp -= damage;					// HPを減らす

		// HPを共有
		sHp = m_hp;
		//

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

void Player::TakeHeal(int amount)
{
	if (m_isHeal) return; // 無敵中なら何もしない
	if (0 < m_hp && m_hp < 3)
	{
		m_hp += amount;					// HPを増やす

		// 追加
		// 回復SE再生
		SOUND("kaifuku_se")->Play(false);
		//

		// HPを共有
		sHp = m_hp;

		m_isHeal = true;			// 無敵状態に設定
		m_notHealTime = 1.0f;		// 無敵時間を3秒に設定
	}
	else if (m_hp <= 0)
	{

	}
	else
	{

	}
}


float Player::GetChipSizeX()
{
	float aX = (CHIP_SIZE * 0.4f);
	// CHIP_SIZE = 430
	return aX;
}

float Player::GetChipSizeY()
{
	float aY = 0.0f;

	if (m_state == EState::Sliding)
	{
		aY = CHIP_SIZE * 0.2f;
	}
	else
	{
		aY = CHIP_SIZE * 0.4f;
	}

	// CHIP_SIZE = 430
	return aY;
}

float Player::GetChipSizeZ()
{
	float aZ = (CHIP_SIZE * 0.2f);
	return aZ;
}

// HPを取得
int Player::sHp = 0; // HP


int Player::Hp()
{
	return sHp;
}

bool Player::s_isMove = false;
bool Player::IsMove()
{
	return s_isMove;
}

extern float testScrollX;

// 更新処理
void Player::Update()
{
	moveSpeed += speedUpPerFrame;
	s_isMove = !m_isInvincible;
	// 無敵状態の時間をカウントダウン
	if (m_isInvincible)
	{
		m_invincibleTime -= 1.0f / 60.0f; // 1フレームごとに時間を減少 (60FPS)
		if (m_invincibleTime <= 0.0f)
		{
			m_isInvincible = false; // 無敵解除
		}
	}

	// 無敵状態の時間をカウントダウン                // 3/17変更（西川）
	if (m_isHeal)
	{
		m_notHealTime -= 1.0f / 60.0f; // 1フレームごとに時間を減少 (60FPS)
		if (m_notHealTime <= 0.0f)
		{
			m_isHeal = false; // 無敵解除
		}
	}

	// 現在の状態に合わせて、処理を切り替える
	switch (m_state)
	{
	case EState::Idle:		StateIdle();	break;
	case EState::Jump:		StateJump();	break;
	case EState::Attack:	StateAttack();	break;
	case EState::Death:		StateDeath();	break;
	case EState::Sliding:	StateSliding();	break;
	case EState::Damage:    StateDamage();  break;
	}

	// Y軸（高さ）の移動を座標に反映
	m_pos.y += m_moveSpeedY;
	if (m_moveSpeedY > 0)
	{
		m_moveSpeedY += GRAVITY;	// Y軸の移動速度に重力を加算
	}

	else if (m_moveSpeedY <= 0 && !m_isGrounded && !m_isFloat)
	{
		m_floatTime = 0.8f; //　滞空時間の設定
		m_isFloat = true;
	}
	else if(m_floatTime <= 0 && !m_isGrounded)
	{
		m_moveSpeedY += GRAVITY;	// Y軸の移動速度に重力を加算
	}
	// 地面より下にいくと
	if (m_pos.y <= 0.0f)
	{
		// 地面の座標へ戻す
		m_pos.y = 0.0f;
		m_moveSpeedY = 0.0f;
		m_isGrounded = true;
		m_isFloat = false;
	}

	if (m_floatTime > 0)
	{
		m_floatTime -= 1.0f / 60.0f;
	}

	testScrollX = m_pos.x;

	// イメージに座標を設定して、アニメーションを更新
	mp_image->SetPos(CalcScreenPos());
	mp_image->UpdateAnimation();
}

// 描画処理
void Player::Render()
{
	mp_image->Draw();
}