#include "CGameMain.h"
#include "Sound/CSoundManager.h"
#include "StaticMash/CStaticMeshManager.h"
#include "Collision/BoundingBox/BoundingBox.h"
#include <algorithm>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=nullptr; } }
#endif


CGameMain::CGameMain(CDirectX9* pDx9, CDirectX11* pDx11)
	: CSceneBase(pDx11)
	, m_pStcMeshObj(nullptr)
	, m_pStaticMeshGround(nullptr)
	, m_pSp3D(nullptr)
	, m_pSpriteGround(nullptr)
	, m_pSpriteExplosion(nullptr)
	, m_pStaticMeshFighter(nullptr)
	, m_pStaticMeshRoboA(nullptr)
	, m_pStaticMeshRoboB(nullptr)
	, m_pStaticMeshBullet(nullptr)
	, m_pStaticMeshBSphere(nullptr)
	, m_pStaticMeshBBox(nullptr)
	, m_pStaticMeshWall(nullptr)
	, m_pGameMain(nullptr)
	, m_pSpriteTitle(nullptr)
	, m_pPlayer(nullptr)
	, m_pGround(nullptr)
	, m_pDbgText(nullptr)
	, m_vCameraTargetPosition(0.0f, 0.0f, 0.0f)
	, m_vCameraTargetLookAt(0.0f, 0.0f, 0.0f)
	, m_fCameraSmoothSpeed(0.05f)
	, m_pEnemyManager(nullptr)
	, m_pWalls()
	, m_EnemySpawnTimer(0) // タイマーを初期化
	, m_CurrentEnemyCount(0) // カウントを初期化
{
	m_pDx11 = pDx11;
	m_pDx9 = pDx9;

	CStaticMeshManager::GetInstance()->Create(*pDx9, *pDx11);

	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);

	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);

	Initialize();
}

CGameMain::~CGameMain()
{
	SAFE_DELETE(m_pEnemyManager);
	SAFE_DELETE(m_pGround);
	SAFE_DELETE(m_pPlayer);
	for (CStaticMeshObject* pWall : m_pWalls)
	{
		SAFE_DELETE(pWall);
	}
	m_pWalls.clear();
	SAFE_DELETE(m_pStcMeshObj);
	SAFE_DELETE(m_pStaticMeshFighter);
	SAFE_DELETE(m_pStaticMeshGround);
	SAFE_DELETE(m_pStaticMeshBullet);
	SAFE_DELETE(m_pStaticMeshRoboA);
	SAFE_DELETE(m_pStaticMeshRoboB);
	SAFE_DELETE(m_pStaticMeshBSphere);
	SAFE_DELETE(m_pStaticMeshBBox);
	SAFE_DELETE(m_pStaticMeshWall);
	SAFE_DELETE(m_pSp3D);
	SAFE_DELETE(m_pSpriteGround);
	SAFE_DELETE(m_pSpriteExplosion);
	SAFE_DELETE(m_pGameMain);
	SAFE_DELETE(m_pSpriteTitle);
	SAFE_DELETE(m_pDbgText);

	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void CGameMain::Initialize()
{
	m_vCameraTargetPosition = D3DXVECTOR3(0.0f, 15.0f, -10.0f);
	m_vCameraTargetLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Camera.vPosition = m_vCameraTargetPosition;
	m_Camera.vLook = m_vCameraTargetLookAt;
}

void CGameMain::Create()
{
	m_pDbgText = new CDebugText();
	m_pStaticMeshFighter = new CStaticMesh();
	m_pStaticMeshGround = new CStaticMesh();
	m_pStaticMeshBullet = new CStaticMesh();
	m_pStaticMeshBSphere = new CStaticMesh();
	m_pStaticMeshBBox = new CStaticMesh();
	m_pStaticMeshWall = new CStaticMesh();
	m_pStcMeshObj = new CStaticMeshObject();
	m_pPlayer = new CPlayer();
	m_pGround = new CGround();
	m_pEnemyManager = new CEnemyManager();

	if (FAILED(m_pDbgText->Init(*m_pDx11)))
	{
		return;
	}

	m_pStaticMeshFighter->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Mesh\\Static\\Fighter\\Fighter.x"));
	m_pStaticMeshGround->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Mesh\\Static\\Ground\\box.x"));
	m_pStaticMeshBSphere->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Sphere.x"));
	m_pStaticMeshBBox->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Box.x"));
	m_pStaticMeshWall->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Box.x"));

	const float STAGE_HALF_SIZE_X = 20.0f;
	const float STAGE_HALF_SIZE_Z = 20.0f;
	const float WALL_HEIGHT = 5.0f;
	const float WALL_THICKNESS = 0.5f;

	m_pStcMeshObj->AttachMesh(*m_pStaticMeshFighter);
	m_pPlayer->AttachMesh(*m_pStaticMeshFighter);
	m_pGround->AttachMesh(*m_pStaticMeshGround);

	m_pPlayer->CreateBSphereForMesh(*m_pStaticMeshBSphere);
	m_pPlayer->CreateBBoxForMesh(*m_pStaticMeshBBox);

	const float STAGE_FULL_SIZE_X = STAGE_HALF_SIZE_X * 2.0f;
	const float STAGE_FULL_SIZE_Z = STAGE_HALF_SIZE_Z * 2.0f;
	const float GROUND_THICKNESS = 0.5f;
	m_pGround->SetScale(STAGE_FULL_SIZE_X, GROUND_THICKNESS, STAGE_FULL_SIZE_Z);
	m_pGround->SetPosition(0.0f, -GROUND_THICKNESS / 2.0f, 0.0f);
	m_pGround->CreateBBoxForMesh(*m_pStaticMeshGround);

	m_pPlayer->SetPosition(0.f, 1.f, 6.f);

	const float WALL_BASE_Y = 1.0f;

	CStaticMeshObject* pWallNorth = new CStaticMeshObject();
	pWallNorth->AttachMesh(*m_pStaticMeshWall);
	pWallNorth->SetScale(STAGE_FULL_SIZE_X + WALL_THICKNESS * 2.0f, WALL_HEIGHT, WALL_THICKNESS);
	pWallNorth->SetPosition(0.0f, -10.0f, -38.5f);
	pWallNorth->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallNorth);

	CStaticMeshObject* pWallSouth = new CStaticMeshObject();
	pWallSouth->AttachMesh(*m_pStaticMeshWall);
	pWallSouth->SetScale(STAGE_FULL_SIZE_X + WALL_THICKNESS * 2.0f, WALL_HEIGHT, WALL_THICKNESS);
	pWallSouth->SetPosition(0.0f, -10.0f, 38.5f);
	pWallSouth->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallSouth);

	CStaticMeshObject* pWallEast = new CStaticMeshObject();
	pWallEast->AttachMesh(*m_pStaticMeshWall);
	pWallEast->SetScale(WALL_THICKNESS, WALL_HEIGHT, STAGE_FULL_SIZE_Z);
	pWallEast->SetPosition(38.5f, -10.f, 0.0f);
	pWallEast->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallEast);

	CStaticMeshObject* pWallWest = new CStaticMeshObject();
	pWallWest->AttachMesh(*m_pStaticMeshWall);
	pWallWest->SetScale(WALL_THICKNESS, WALL_HEIGHT, STAGE_FULL_SIZE_Z);
	pWallWest->SetPosition(-38.5f, -10.f, 0.0f);
	pWallWest->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallWest);

	float cameraHeightOffset = 15.0f;
	float cameraBackOffset = 8.0f;

	m_Camera.vPosition.x = m_pPlayer->GetPosition().x;
	m_Camera.vPosition.y = m_pPlayer->GetPosition().y + cameraHeightOffset;
	m_Camera.vPosition.z = m_pPlayer->GetPosition().z - cameraBackOffset;
	m_Camera.vLook = m_pPlayer->GetPosition();
	m_vCameraTargetPosition = m_Camera.vPosition;
	m_vCameraTargetLookAt = m_Camera.vLook;
}

void CGameMain::Update()
{
	m_pGround->Update();
	m_pGround->UpdateBSpherePos();
	m_pPlayer->Update();
	m_pPlayer->UpdateBSpherePos();

	m_pEnemyManager->Update();

	// 時間経過で敵を出現させるロジック
	if (m_CurrentEnemyCount < m_MaxEnemyCount)
	{
		m_EnemySpawnTimer++;

		if (m_EnemySpawnTimer >= m_EnemySpawnInterval)
		{
			const float STAGE_HALF_SIZE_X = 20.0f;
			const float STAGE_HALF_SIZE_Z = 20.0f;
			const float ENEMY_Y_POS = 1.0f;

			std::random_device rnd;
			std::mt19937 mt(rnd());
			std::uniform_real_distribution<> randX(-STAGE_HALF_SIZE_X, STAGE_HALF_SIZE_X);
			std::uniform_real_distribution<> randZ(-STAGE_HALF_SIZE_Z, STAGE_HALF_SIZE_Z);

			m_pEnemyManager->CreateEnemy(
				CEnemyManager::EnemyType::EnemyA,
				static_cast<float>(randX(mt)),
				ENEMY_Y_POS,
				static_cast<float>(randZ(mt)),
				dynamic_cast<CPlayer*>(m_pPlayer)
			);

			m_EnemySpawnTimer = 0;
			m_CurrentEnemyCount++;
		}
	}

	BoundingBox* playerBBox = m_pPlayer->GetBBox();
	BoundingBox* groundBBox = m_pGround->GetBBox();

	if (playerBBox && groundBBox)
	{
		if (playerBBox->IsHit(*groundBBox))
		{
			D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
			playerPos.y = groundBBox->GetMaxPosition().y + (playerBBox->GetSize().y / 2.0f);
			m_pPlayer->SetPosition(playerPos.x, playerPos.y, playerPos.z);
		}
	}

	if (playerBBox)
	{
		D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
		for (CStaticMeshObject* pWall : m_pWalls)
		{
			pWall->Update();
			pWall->UpdateBSpherePos();
			BoundingBox* wallBBox = pWall->GetBBox();
			if (wallBBox && playerBBox->IsHit(*wallBBox))
			{
				D3DXVECTOR3 playerMin = playerBBox->GetMinPosition();
				D3DXVECTOR3 playerMax = playerBBox->GetMaxPosition();
				D3DXVECTOR3 wallMin = wallBBox->GetMinPosition();
				D3DXVECTOR3 wallMax = wallBBox->GetMaxPosition();
				float overlapX1 = playerMax.x - wallMin.x;
				float overlapX2 = wallMax.x - playerMin.x;
				float overlapZ1 = playerMax.z - wallMin.z;
				float overlapZ2 = wallMax.z - playerMin.z;
				float pushX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;
				float pushZ = (overlapZ1 < overlapZ2) ? overlapZ1 : -overlapZ2;

				if (std::abs(pushX) < std::abs(pushZ)) {
					playerPos.x -= pushX;
				}
				else {
					playerPos.z -= pushZ;
				}
				m_pPlayer->SetPosition(playerPos.x, playerPos.y, playerPos.z);
			}
		}
	}

	ThirdPersonCamera(
		&m_Camera,
		m_pPlayer->GetPosition(),
		m_pPlayer->GetRotation().y);
}


void CGameMain::Draw()
{
	Camera();
	Projection();

	m_pGround->Draw(m_mView, m_mProj, m_Light, m_Camera);
	m_pPlayer->Draw(m_mView, m_mProj, m_Light, m_Camera);

	for (CStaticMeshObject* pWall : m_pWalls)
	{
		pWall->Draw(m_mView, m_mProj, m_Light, m_Camera);
	}

	m_pEnemyManager->Draw(m_mView, m_mProj, m_Light, m_Camera);


	TCHAR dbgText[256];
	_stprintf_s(dbgText, _T("Player Pos: %.2f, %.2f, %.2f"),
		m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y, m_pPlayer->GetPosition().z);
	m_pDbgText->SetColor(1.0f, 1.0f, 0.0f);
	m_pDbgText->Render(dbgText, 10, 10);

	BoundingBox* playerBBox = m_pPlayer->GetBBox();
	if (playerBBox) {
		_stprintf_s(dbgText, _T("Player BBox Min: %.2f, %.2f, %.2f"),
			playerBBox->GetMinPosition().x, playerBBox->GetMinPosition().y, playerBBox->GetMinPosition().z);
		m_pDbgText->Render(dbgText, 10, 30);
		_stprintf_s(dbgText, _T("Player BBox Max: %.2f, %.2f, %.2f"),
			playerBBox->GetMaxPosition().x, playerBBox->GetMaxPosition().y, playerBBox->GetMaxPosition().z);
		m_pDbgText->Render(dbgText, 10, 50);
	}

	BoundingBox* groundBBox = m_pGround->GetBBox();
	if (groundBBox) {
		_stprintf_s(dbgText, _T("Ground BBox Min: %.2f, %.2f, %.2f"),
			groundBBox->GetMinPosition().x, groundBBox->GetMinPosition().y, groundBBox->GetMinPosition().z);
		m_pDbgText->Render(dbgText, 10, 70);
		_stprintf_s(dbgText, _T("Ground BBox Max: %.2f, %.2f, %.2f"),
			groundBBox->GetMaxPosition().x, groundBBox->GetMaxPosition().y, groundBBox->GetMaxPosition().z);
		m_pDbgText->Render(dbgText, 10, 90);
	}
}

HRESULT CGameMain::LoadData()
{
	return S_OK;
}

void CGameMain::Camera()
{
	D3DXVECTOR3 cam_pos = m_Camera.vPosition;
	D3DXVECTOR3 cam_look = m_Camera.vLook;
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(
		&m_mView,
		&cam_pos, &cam_look, &vUpVec);
}

void CGameMain::Projection()
{
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 100.0f;

	D3DXMatrixPerspectiveFovLH(
		&m_mProj,
		fov_y,
		aspect,
		near_z,
		far_z);
}

void CGameMain::ThirdPersonCamera(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY)
{
	float cameraHeightOffset = 15.0f;
	float cameraBackOffset = 8.0f;

	m_vCameraTargetPosition.x = TargetPos.x;
	m_vCameraTargetPosition.y = TargetPos.y + cameraHeightOffset;
	m_vCameraTargetPosition.z = TargetPos.z - cameraBackOffset;

	m_vCameraTargetLookAt = TargetPos;

	pCamera->vPosition.x = pCamera->vPosition.x + (m_vCameraTargetPosition.x - pCamera->vPosition.x) * m_fCameraSmoothSpeed;
	pCamera->vPosition.y = pCamera->vPosition.y + (m_vCameraTargetPosition.y - pCamera->vPosition.y) * m_fCameraSmoothSpeed;
	pCamera->vPosition.z = pCamera->vPosition.z + (m_vCameraTargetPosition.z - pCamera->vPosition.z) * m_fCameraSmoothSpeed;

	pCamera->vLook.x = pCamera->vLook.x + (m_vCameraTargetLookAt.x - pCamera->vLook.x) * m_fCameraSmoothSpeed;
	pCamera->vLook.y = pCamera->vLook.y + (m_vCameraTargetLookAt.y - pCamera->vLook.y) * m_fCameraSmoothSpeed;
	pCamera->vLook.z = pCamera->vLook.z + (m_vCameraTargetLookAt.z - pCamera->vLook.z) * m_fCameraSmoothSpeed;
}