#pragma once
#include "Scene/CSceneBase.h"
#include "GameObject/CSpriteObject/CSpriteObject.h" // CSpriteObjectを直接使用している箇所はないが、残しておく
#include "SceneManager/CSceneManager.h" // CSceneManagerを使用していないが、残しておく

#include "GameObject/StaticMeshObject/CCharacter/Ground/CGround.h"
#include "GameObject/StaticMeshObject/CCharacter/Player/CPlayer.h"
#include "DebugText/CDebugText.h"
#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h" // CCharacterを使用するために必要
#include "GameObject/StaticMeshObject/CStaticMeshObject.h" // CStaticMeshObjectを使用するために必要
#include "StaticMash/CStaticMesh.h" // CStaticMeshを使用するために必要

#include "..//..//GameObject/StaticMeshObject/CCharacter/Enemy/EnemyTypeA/CEnemyTypeA.h" // CEnemyTypeAを使用するために必要

#include <vector> // std::vectorを使用するために必要


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

	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Projection();

	//三人称カメラ
	// プレイヤーを上空から見下ろすようにカメラを調整します。
	void ThirdPersonCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);


private:

	//ゲームで扱うスプライトデータ(使いまわす資源).
	CSprite3D* m_pSp3D; // このメンバは現在使用されていないようです
	CSprite3D* m_pSpriteGround; // 現在のコードでは使用されていませんが、残しておきます
	CSprite3D* m_pSpriteExplosion; // 現在のコードでは使用されていませんが、残しておきます

	//スタティックメッシュ(使いまわす資源).
	CStaticMesh* m_pStaticMeshGround;	//地面
	CStaticMesh* m_pStaticMeshFighter;	//プレイヤーのメッシュ
	CStaticMesh* m_pStaticMeshRoboA;		//ロボA (現在使用されていません)
	CStaticMesh* m_pStaticMeshRoboB;		//ロボB (現在使用されていません)
	CStaticMesh* m_pStaticMeshBullet;	//弾 (現在使用されていません)
	CStaticMesh* m_pStaticMeshBSphere;	//バウンディングスフィア(当たり判定用)
	CStaticMesh* m_pStaticMeshBBox;		//バウンディングボックス(当たり判定用)
	CStaticMesh* m_pStaticMeshWall;		//壁

	// m_pGameMain と m_pSpriteTitle は現在使用されていませんが、残しておきます
	CStaticMesh* m_pGameMain;
	CSprite2D* m_pSpriteTitle;

	//スタティックメッシュオブジェクトクラス.
	CStaticMeshObject* m_pStcMeshObj; // このメンバは現在使用されていません

	//キャラクタークラス
	CCharacter* m_pPlayer;

	//地面クラス.
	CGround* m_pGround;
	CDebugText* m_pDbgText;	//デバックテキスト.

	// カメラの滑らかな追従のための変数
	D3DXVECTOR3 m_vCameraTargetPosition;
	D3DXVECTOR3 m_vCameraTargetLookAt;
	float m_fCameraSmoothSpeed;

	CEnemyTypeA* m_pEnemyA;

	std::vector<CStaticMeshObject*> m_pWalls; // ★追加: 壁オブジェクトのリスト
};