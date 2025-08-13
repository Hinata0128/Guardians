#pragma once
#include "Scene/CSceneBase.h"
#include "GameObject/CSpriteObject/CSpriteObject.h"
#include "SceneManager/CSceneManager.h"

#include "GameObject/StaticMeshObject/CCharacter/Ground/CGround.h"
#include "GameObject/StaticMeshObject/CCharacter/Player/CPlayer.h"
#include "DebugText/CDebugText.h"
#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"
#include "GameObject/StaticMeshObject/CStaticMeshObject.h"
#include "StaticMash/CStaticMesh.h"

#include "..//..//GameObject/StaticMeshObject/CCharacter/EnemyManager/CEnemyManager.h"

#include <vector>
#include <random>

/*********************************************
*	ゲームメイン画像クラス.
**/

class CGameMain
	: public CSceneBase
{
public:
	CGameMain(CDirectX9* pDx9, CDirectX11* pDx11);
	~CGameMain() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	HRESULT LoadData();

	void Camera();
	void Projection();

	void ThirdPersonCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);


private:
	CSprite3D* m_pSp3D;
	CSprite3D* m_pSpriteGround;
	CSprite3D* m_pSpriteExplosion;

	CStaticMesh* m_pStaticMeshGround;
	CStaticMesh* m_pStaticMeshFighter;
	CStaticMesh* m_pStaticMeshRoboA;
	CStaticMesh* m_pStaticMeshRoboB;
	CStaticMesh* m_pStaticMeshBullet;
	CStaticMesh* m_pStaticMeshBSphere;
	CStaticMesh* m_pStaticMeshBBox;
	CStaticMesh* m_pStaticMeshWall;

	CStaticMesh* m_pGameMain;
	CSprite2D* m_pSpriteTitle;

	CStaticMeshObject* m_pStcMeshObj;

	CCharacter* m_pPlayer;

	CGround* m_pGround;
	CDebugText* m_pDbgText;

	D3DXVECTOR3 m_vCameraTargetPosition;
	D3DXVECTOR3 m_vCameraTargetLookAt;
	float m_fCameraSmoothSpeed;

	CEnemyManager* m_pEnemyManager;
	std::vector<CStaticMeshObject*> m_pWalls;

	// 時間経過で敵を出現させるためのメンバー変数
	int m_EnemySpawnTimer;
	const int m_EnemySpawnInterval = 180; // 3秒
	/*const int m_MaxEnemyCount = 3;
	int m_CurrentEnemyCount;*/
};