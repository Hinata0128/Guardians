#include "CSceneBase.h"

CSceneBase::CSceneBase(CDirectX11* pDx11)
	: m_pMesh	(nullptr)
	, m_pDx11	(pDx11)
	, m_pDx9	()
	, m_hWnd	()
	, m_Score	(0)
	, m_Count	(0)
{
}
