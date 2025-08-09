#include "CSceneManager.h"
#include "Sound/CSoundManager.h"

CSceneManager::CSceneManager()
	: m_pScene	( nullptr )
	, m_hWnd	()
	, m_pDx11	()
	, m_pDx9	()
{

}

CSceneManager::~CSceneManager()
{
}

HRESULT CSceneManager::Create(HWND hWnd)
{
	m_hWnd = hWnd;
	LoadScene(List::GameMain);
	return S_OK;
}

void CSceneManager::Update()
{
	m_pScene->Update();
}

void CSceneManager::Draw()
{
	m_pScene->Draw();
}

void CSceneManager::LoadScene(List Scene)
{
	m_pScene.reset();

	//シーン作成.
	MakeScene(Scene);
	m_pScene->Create();
}

HWND CSceneManager::GetHWND() const
{
	return m_hWnd;
}

//シーン作成.
void CSceneManager::MakeScene(List Scene)
{
	switch (Scene)
	{
	//case CSceneManager::Title:
	//	m_pScene = std::make_unique<CTitle>(m_pDx11);
	//	break;
	case CSceneManager::GameMain:
		m_pScene = std::make_unique<CGameMain>(m_pDx9, m_pDx11);
		break;
	//case CSceneManager::GameOver:
	//	m_pScene = std::make_unique<CGameOver>(m_pDx11);
	//	break;
	//case CSceneManager::Ending:
	//	m_pScene = std::make_unique<CEnding>(m_pDx11);
	//	break;
	default:
		break;
	}
}
