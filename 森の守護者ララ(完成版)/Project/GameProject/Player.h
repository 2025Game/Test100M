#pragma once
#include "CharaBase.h"
#include "UI.h" // UIアイコンを変更するため

class Player : public CharaBase
{
public:
	// コンストラクタ// UI を参照で受け取る
	Player(const CVector3D& pos);
	// デストラクタ
	~Player();

	void Attack();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	void TakeDamage(int damage) override; // TakeDamage をオーバーライド
	void TakeHeal(int amount);

	//　キャラのサイズをもとに当たり判定のサイズを返す
	float GetChipSizeX()override; //　横幅
	float GetChipSizeY()override; //　高さ
	float GetChipSizeZ()override; //　奥行き

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
		Sliding,// スライディング
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
	// スライディング時の更新処理
	void StateSliding();
	// 被ダメ時の更新処理
	void StateDamage();

	EState m_state;
	int m_stateStep;

	// アニメーションの種類
	enum class EAnimType
	{
		Dash,	// ダッシュ
		Jump,	// ジャンプ
		Death,	// 死亡
		Attack,	// 攻撃
		Sliding,// スライディング
		Damage, // 被ダメ
		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// プレイヤーの画像

	float m_moveSpeedY;	// Y軸（高さ）の移動速度

	bool m_isInvincible;          // 無敵状態かどうか
	float m_invincibleTime;       // 無敵時間の残り（秒）

	bool m_isHeal;          // 回復可能状態かどうか                // 3/17変更（西川）
	float m_notHealTime;    // 回復不能時間の残り（秒）                // 3/17変更（西川）

	bool m_isFloat;          // 滞空状態かどうか                // 3/20変更（西川）
	float m_floatTime;    // 滞空時間の残り（秒）                // 3/20変更（西川）

	// HP(共有用)
	static int sHp;
	//
	static bool s_isMove;
};