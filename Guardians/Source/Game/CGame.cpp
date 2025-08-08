#include "CGame.h"
#include "Sound/CSoundManager.h"
#include <algorithm>    // std::min, std::max のために必要 (衝突応答のoverlap計算で使用)
#include "StaticMash/CStaticMeshManager.h"

//コンストラクタ.
CGame::CGame(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd)
	: m_pDx9(&pDx9)
	, m_pDx11(&pDx11)
	, m_hWnd(hWnd)
	, m_mView()
	, m_mProj()

	, m_Camera()
	, m_Light()

	, m_pSpriteGround(nullptr)
	, m_pSpritePlayer(nullptr)


{
	//スタティックメッシュマネージャーの構築
	CStaticMeshManager::GetInstance()->Create(pDx9, pDx11);
}


//デストラクタ.
CGame::~CGame()
{
}

//構築.
void CGame::Create()
{
	CStaticMeshManager::GetInstance()->Create(*m_pDx9, *m_pDx11);

	CSceneManager::GetInstance()->SetDx11(m_pDx11);
	CSceneManager::GetInstance()->SetDx9(m_pDx9);

}

//ロードデータ関数.
HRESULT CGame::LoadData()
{
	//サウンドデータの読み込み
	if (CSoundManager::GetInstance()->Load(m_hWnd) == false) {
		return E_FAIL;
	}

	CSceneManager::GetInstance()->Create(m_hWnd);


	return S_OK;
}

//解放関数.
void CGame::Release()
{
	// CGame::~CGame() で一括して行われているため、ここでは不要。
}


//更新処理.
void CGame::Update()
{
	CSceneManager::GetInstance()->Update();

}

void CGame::Draw()
{
	CSceneManager::GetInstance()->Drae();

}

//カメラ関数.
void CGame::Camera()
{
}
//プロジェクション関数.
void CGame::Projection()
{
}
//三人称カメラ.
void CGame::ThirdPersonCamera(
	CAMERA* pCamera, const D3DXVECTOR3& TagetPos, float TagetRotY)
{
}