#pragma once
#include "EnemyBase.h"

class Mana : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">スライムの種類</param>
	/// <param name="pos">初期位置</param>
	Mana(int type, const CVector3D& pos);
	// デストラクタ
	~Mana();

	// 踏まれた処理
	virtual void Death() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	//　キャラのサイズをもとに当たり判定のサイズを返す
	float GetChipSizeX()override; //　横幅
	float GetChipSizeY()override; //　高さ
	float GetChipSizeZ()override; //　奥行き

	void Alpha0();

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
};