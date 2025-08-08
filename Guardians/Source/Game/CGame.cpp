#include "CGame.h"
#include "Sound/CSoundManager.h"
#include <algorithm>    // std::min, std::max �̂��߂ɕK�v (�Փˉ�����overlap�v�Z�Ŏg�p)
#include "StaticMash/CStaticMeshManager.h"

//�R���X�g���N�^.
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
	//�X�^�e�B�b�N���b�V���}�l�[�W���[�̍\�z
	CStaticMeshManager::GetInstance()->Create(pDx9, pDx11);
}


//�f�X�g���N�^.
CGame::~CGame()
{
}

//�\�z.
void CGame::Create()
{
	CStaticMeshManager::GetInstance()->Create(*m_pDx9, *m_pDx11);

	CSceneManager::GetInstance()->SetDx11(m_pDx11);
	CSceneManager::GetInstance()->SetDx9(m_pDx9);

}

//���[�h�f�[�^�֐�.
HRESULT CGame::LoadData()
{
	//�T�E���h�f�[�^�̓ǂݍ���
	if (CSoundManager::GetInstance()->Load(m_hWnd) == false) {
		return E_FAIL;
	}

	CSceneManager::GetInstance()->Create(m_hWnd);


	return S_OK;
}

//����֐�.
void CGame::Release()
{
	// CGame::~CGame() �ňꊇ���čs���Ă��邽�߁A�����ł͕s�v�B
}


//�X�V����.
void CGame::Update()
{
	CSceneManager::GetInstance()->Update();

}

void CGame::Draw()
{
	CSceneManager::GetInstance()->Drae();

}

//�J�����֐�.
void CGame::Camera()
{
}
//�v���W�F�N�V�����֐�.
void CGame::Projection()
{
}
//�O�l�̃J����.
void CGame::ThirdPersonCamera(
	CAMERA* pCamera, const D3DXVECTOR3& TagetPos, float TagetRotY)
{
}