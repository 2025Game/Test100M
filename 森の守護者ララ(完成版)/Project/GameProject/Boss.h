#pragma once
#include "CharaBase.h"
#include "UI.h"			// UIアイコンを変更するため

class Boss : public CharaBase
{
public:
	// コンストラクタ// UI を参照で受け取る
	Boss(const CVector3D& pos);
	// デストラクタ
	~Boss();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	void TakeDamage(int damage) override; // TakeDamage をオーバーライド

	// HPを取得(追加)
	static int Hp();

	// isMoveの取得
	static bool IsMove();


private:
	// 状態
	enum class EState
	{
		Idle,	// 待機状態
		Jump,	// ジャンプ中
		Attack,	// 攻撃中
		Death,	// 死亡
		Damage, // 被ダメ
	};
	void ChangeState(EState state);

	// 移動処理の更新
	bool UpdateMove();

	// 待機時の更新処理
	void StateIdle();
	// ジャンプ中の更新処理
	void StateJump();
	// 攻撃中の更新処理
	void StateAttack();
	// 死亡時の更新処理
	void StateDeath();
	// 被ダメ時の更新処理
	void StateDamage();

	//--------------------------------------------------------
	bool m_isInvincible;          // 無敵状態かどうか
	float m_invincibleTime;       // 無敵時間の残り（秒）
	//--------------------------------------------------------

	EState m_state;
	int m_stateStep;

	// アニメーションの種類
	enum class EAnimType
	{
		Dash,	// ダッシュ
		Jump,	// ジャンプ
		Death,	// 死亡
		Attack,	// 攻撃
		Damage, // 被ダメ
		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// Bossドラゴンの画像

	// HP(共有用)
	static int sHp;
	//
	static bool s_isMove;
};