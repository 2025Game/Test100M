#pragma once
#include "ObjectBase.h"

class BGFog : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">スライムの種類</param>
	/// <param name="pos">初期位置</param>
	BGFog(int type, const CVector3D& pos);
	// デストラクタ
	~BGFog();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	CVector2D CalcScreenPos(bool grounded = false) const override;

private:
	// 状態
	enum class EState
	{
		Idle,	// 待機状態
		Action,	// 踏まれた
	};
	void ChangeState(EState state);

	// 待機時の更新処理
	void StateIdle();
	// 踏まれた時の更新処理
	void StateAction();

	void ReSetPos();

	EState m_state;
	int m_stateStep;

	// アニメーションの種類
	enum class EAnimType
	{
		Idle,	// 待機
		Action,	// 踏まれた

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// 画像
	int m_type;			// スライムの種類

	int loopN4;  // ← メンバ変数に変更
};