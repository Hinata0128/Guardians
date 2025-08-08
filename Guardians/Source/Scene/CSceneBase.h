#pragma once
#include "StaticMash/CStaticMesh.h"
#include "DirectX/CDirectX11.h"
#include "Sprite2D/CSprite2D.h"
#include "GameObject/UIObject/CUIObject.h"


/**********************************************************
*	基底クラス.
**/

class CSceneBase	
{
public:
	CSceneBase(CDirectX11* pDx11);
	virtual ~CSceneBase() {}

	virtual void Initialize() = 0;
	virtual void Create() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//メッシュを接続する.
	void AttachMesh(CStaticMesh& pMesh) {
		m_pMesh = &pMesh;
	}

protected:
	CStaticMesh* m_pMesh;
	CDirectX9* m_pDx9;
	CDirectX11* m_pDx11;

	//ウィンドウハンドル.
	HWND				m_hWnd;

	//カメラ情報.
	CAMERA				m_Camera;
	//ライト情報
	LIGHT				m_Light;

	//行列.
	D3DXMATRIX			m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX			m_mProj;	//射影（プロジェクション）行列.

	int m_Score;

	int m_Count;


};