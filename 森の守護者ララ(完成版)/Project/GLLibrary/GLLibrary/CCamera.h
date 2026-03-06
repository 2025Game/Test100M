/*!
 *	@brief		カメラクラス
 *	5個のカメラが存在する。
 *　カレントカメラに設定したビュー行列でモデルが表示される。
 */
#pragma once

#include "CVector.h"
#include "CMatrix.h"
#include "CRect.h"
class CCamera {
public:

	enum {
		eMainCamera,
		eSkyBox,
		eShadowCamera,
		eOsser_1,
		eMax
	};
private:
	CVector3D m_pos;	//カメラの位置
	CVector3D m_at;		//注視点
	CVector3D m_up;		//Upベクトル
	CVector3D m_dir;	//向き（Frontベクトル）
	CMatrix m_mView;	//カメラ行列
	CMatrix m_mProj;	//射影行列
	CMatrix m_m2D;		//2D射影行列
	CMatrix m_mViewport;	//ビューポート行列
	CRect m_viewportRect;	//ビューポート矩形
	float	m_whidth;	//カメラの幅
	float	m_height;	//　〃　　高さ
	static CCamera m_camera[eMax];
	static CCamera *m_pCurrent;
//	CRect	m_screen;
public:

	/*!
		@brief	カメラオブジェクトを取得
		@param	num			[in] 取得するカメラの番号
		@retval	カメラオブジェクトのアドレス
	**/
	static CCamera* GetCamera(int num=0);
	
	/*!
		@brief	現在使用しているカメラオブジェクトを取得
		@retval	カレントカメラオブジェクトのアドレス
	**/
	static CCamera* GetCurrent();

	/*!
		@brief	使用するカメラに設定
		@param	num			[in] 使用するカメラの番号
		@retval　無し
	**/

	static void SetCurrent(int num);
	/*!
		@brief	使用するカメラに設定
		@param	p			[in] 使用するカメラのアドレス
		@retval　無し
	**/
	static void SetCurrent(CCamera *p);
	/*!
		@brief	コンストラクタ
		@retval　無し
	**/
	CCamera();
	/*!
		@brief	カメラの幅を取得
		@retval　カメラの幅　
	**/
	float GetWhidth() const;
	/*!
		@brief	カメラの高さを取得
		@retval　カメラの高さ　
	**/
	float GetHeight() const;
	/*!
		@brief	カメラの幅と高さ設定
		@param	w			[in] 幅
		@param	h			[in] 高さ
		@retval　無し　
	**/
	void SetSize(float w,float h);

	/*!
		@brief	注視カメラでビュー行列
		@param	eye			[in] 位置
		@param	at			[in] 注視点
		@param	up			[in] 上ベクトル
		@retval　無し　
	**/
	void LookAt(const CVector3D &eye, const CVector3D &at, const CVector3D &up);

	/*!
		@brief	位置と回転値でビュー行列を設定
		@param	pos			[in] 位置
		@param	rot			[in] 回転値
		@retval　無し　
	**/
	void SetTranseRot(const CVector3D &pos, const CVector3D &rot);
	/*!
		@brief	モデル行列からビュー行列を設定
		@param	mtx			[in] モデル行列
		@retval　無し　
	**/
	void SetTranseRot(const CMatrix &mtx);

	/*!
		@brief	ビュー行列を直接設定
		@param	m			[in] ビュー行列
		@retval　無し　
	**/
	void SetViewMatrix(const CMatrix &m);

	/*!
		@brief	ビュー行列を取得
		@retval　無し　
	**/
	const CMatrix& GetViewMatrix() const {

		return m_mView;
	}

	
	/*!
		@brief	ビュー行列を取得
		@retval　無し　
	**/
	CMatrix GetBuilbordMatrix() const;
	
	/*!
		@brief	カメラの位置を取得
		@retval　カメラの位置
	**/
	CVector3D GetPos() const;

	/*!
		@brief	カメラの向きを取得
		@retval　カメラの向き
	**/
	CVector3D GetDir() const;
	/*!
	@brief	カメラの向きを取得
	@retval　カメラの向き
	**/
	CVector3D GetUp() const;
	/*!
	@brief	透視投影行列用
	@param	fovy			[in] 視野
	@param	aspect			[in] アスペクト比
	@param	zNear			[in] 前面距離
	@param	zFar			[in] 背面距離
	@retval　
	**/
	void Perspective(float fovy, float aspect, float zNear, float zFar);
	void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	/*!
		@brief	射影行列の設定
		@param	m			[in] 射影行列
	**/
	void SetProjectionMatrix(const CMatrix &m);
	/*!
		@brief	2次元描画用射影行列の設定
		@param	m			[in] 射影行列
	**/
	void SetProjectionMatrix2D(const CMatrix& m);
	/*!
		@brief	 射影行列を取得
		@retval　射影行列
	**/
	const CMatrix& GetProjectionMatrix() const{
		return m_mProj;
	}

	/*void SetScreenRect(const CRect& r) {
		m_screen = r;
	}
	CRect GetScreenRect() {
		return m_screen;
	}*/
	/*!
	@brief	透視投影行列用
	@param	x			[in] X位置
	@param	y			[in] Y位置
	@param	w			[in] 幅
	@param	h			[in] 高さ
	*/
	void Viewport(float x, float y, float w, float h);
	const CMatrix& GetViewportMatrix() const {
		return m_mViewport;
	}
	const CRect& GetViewportRect() const {
		return m_viewportRect;
	}

	void Update2DProjectionMatrix();
	const CMatrix& Get2DProjectionMatrix() const {
		return m_m2D;
	}

	// カメラ追従(追加)
	void CCamera::UpdateCameraFollowPlayer(CCamera* camera, const CVector3D& playerPos) {
		if (!camera) return;

		// 現在のカメラ位置と注視点
		CVector3D camPos = camera->GetPos();
		CVector3D camAt = camera->GetPos() + camera->GetDir(); // 注視点

		// 追従速度（プレイヤーの動きに応じて変化可能）
		float followSpeed = 0.15f;  // 速すぎるなら 0.1f に調整

		// カメラの位置を補間してスムーズに移動
		CVector3D targetCamPos = {
			playerPos.x,         // プレイヤーと同じ X
			camPos.y,            // カメラのY座標はそのまま
			camPos.z             // カメラのZ座標もそのまま
		};

		// 線形補間 (Lerp) で X座標だけを更新
		camPos.x += (targetCamPos.x - camPos.x) * followSpeed;

		// 注視点の補間
		CVector3D targetCamAt = {
			playerPos.x,        // プレイヤーのX座標に注視点を合わせる
			camAt.y,            // 注視点のY座標はそのまま
			camAt.z             // 注視点のZ座標はそのまま
		};

		camAt.x += (targetCamAt.x - camAt.x) * followSpeed;

		// カメラのビュー行列を更新
		camera->LookAt(camPos, camAt, camera->GetUp());
	}
	//	
};