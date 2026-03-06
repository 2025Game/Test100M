#pragma once
#include "EnemyBase.h"

class Mosubi : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">スライムの種類</param>
	/// <param name="pos">初期位置</param>
	Mosubi(int type, const CVector3D& pos);
	// デストラクタ
	~Mosubi();

	// 当たった処理
	virtual void Death() override;

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	//　キャラのサイズをもとに当たり判定のサイズを返す
	float GetChipSizeX()override; //　横幅
	float GetChipSizeY()override; //　高さ
	float GetChipSizeZ()override; //　奥行き

private:
	// 状態
	enum class EState
	{
		Idle,	// 待機状態
		Action,	// 当たった
	};
	void ChangeState(EState state);

	// 待機時の更新処理
	void StateIdle();
	// 当たった時の更新処理
	void StateAction();

	EState m_state;
	int m_stateStep;

	// アニメーションの種類
	enum class EAnimType
	{
		Idle,	// 待機
		Action,	// 当たった

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// 画像
	int m_type;			// モスビの種類
};