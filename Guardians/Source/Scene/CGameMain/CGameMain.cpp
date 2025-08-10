#include "CGameMain.h"
#include "Sound/CSoundManager.h" // CSoundManagerは使用されていないが、残しておく
#include "StaticMash/CStaticMeshManager.h"
#include "Collision/BoundingBox/BoundingBox.h" // BoundingBoxクラスを使用するために必要
#include <algorithm> // std::min, std::max, std::abs のために必要

// SAFE_DELETEマクロが定義されていない場合のために、ここで仮定義 (実際は共通ヘッダにあるはず)
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=nullptr; } }
#endif


// コンストラクタ.
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
	, m_vCameraTargetPosition(0.0f, 0.0f, 0.0f) // カメラターゲット位置の初期化
	, m_vCameraTargetLookAt(0.0f, 0.0f, 0.0f)	// カメラターゲット注視点の初期化
	, m_fCameraSmoothSpeed(0.05f)	// カメラ追従速度の初期化
	, m_pEnemyA(nullptr)
	, m_pWalls() // ★変更: std::vectorをデフォルトコンストラクトで初期化
{
	m_pDx11 = pDx11;
	m_pDx9 = pDx9;

	//スタティックメッシュマネージャーの構築
	CStaticMeshManager::GetInstance()->Create(*pDx9, *pDx11);

	//カメラの位置を変更できるところ.
	// 初期カメラ位置はInitialize()で設定されるため、ここではデフォルト値のままにします。
	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	//カメラを見ているところを変更できるところ.
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);

	//ライト情報
	m_Light.vDirection = D3DXVECTOR3(1.5f, 1.f, -1.f);		//ライト方向.

	Initialize();
}

// デストラクタ.
CGameMain::~CGameMain()
{
	SAFE_DELETE(m_pEnemyA);

	//地面の破棄.
	SAFE_DELETE(m_pGround);

	//プレイヤーの破棄.
	SAFE_DELETE(m_pPlayer);

	// 壁の破棄 ★追加
	for (CStaticMeshObject* pWall : m_pWalls)
	{
		SAFE_DELETE(pWall);
	}
	m_pWalls.clear(); // ベクターの内容をクリア

	//スタティックメッシュオブジェクトの破棄 (未使用だが一応)
	SAFE_DELETE(m_pStcMeshObj);

	//スタティックメッシュの破棄
	SAFE_DELETE(m_pStaticMeshFighter);
	SAFE_DELETE(m_pStaticMeshGround);
	SAFE_DELETE(m_pStaticMeshBullet); // 未使用だがデストラクタに残しておく
	SAFE_DELETE(m_pStaticMeshRoboA);// 未使用だがデストラクタに残しておく
	SAFE_DELETE(m_pStaticMeshRoboB);// 未使用だがデストラクタに残しておく
	SAFE_DELETE(m_pStaticMeshBSphere);
	SAFE_DELETE(m_pStaticMeshBBox);
	SAFE_DELETE(m_pStaticMeshWall);

	//スプライトの解放 (未使用だがデストラクタに残しておく)
	SAFE_DELETE(m_pSp3D);
	SAFE_DELETE(m_pSpriteGround);
	SAFE_DELETE(m_pSpriteExplosion);
	SAFE_DELETE(m_pGameMain);
	SAFE_DELETE(m_pSpriteTitle);

	//デバッグテキストの破棄
	SAFE_DELETE(m_pDbgText);

	//外部で作成しているので、ここでは破棄しない
	// m_hWnd = nullptr; // CSceneBaseのメンバであるため、ここでnullptrにしない
	m_pDx11 = nullptr;
	m_pDx9 = nullptr;
}

void CGameMain::Initialize()
{
	// カメラの位置を初期化
	// ThirdPersonCameraで制御するため、ここでは特に設定しません
	// m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	// m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);

	// カメラの初期ターゲット位置と注視点を設定
	// ここでの初期設定はCreate()で上書きされるため、デフォルト値のままでも問題ありません。
	m_vCameraTargetPosition = D3DXVECTOR3(0.0f, 15.0f, -10.0f);
	m_vCameraTargetLookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Camera.vPosition = m_vCameraTargetPosition;
	m_Camera.vLook = m_vCameraTargetLookAt;
}

void CGameMain::Create()
{
	//デバックテキストのインスタンス生成.
	m_pDbgText = new CDebugText();

	//スタティックメッシュのインスタンス生成.
	m_pStaticMeshFighter = new CStaticMesh();
	m_pStaticMeshGround = new CStaticMesh();
	m_pStaticMeshBullet = new CStaticMesh();
	m_pStaticMeshBSphere = new CStaticMesh(); // BSphere用のメッシュインスタンスをここで生成
	m_pStaticMeshBBox = new CStaticMesh();
	m_pStaticMeshWall = new CStaticMesh();

	//スタティックメッシュオブジェクトクラスのインスタンス生成.
	m_pStcMeshObj = new CStaticMeshObject(); // 未使用だが一応生成

	//キャラクタークラスのインスタンス生成.
	m_pPlayer = new CPlayer();

	//地面クラスのインスタンス生成.
	m_pGround = new CGround();

	//デバックテキストの読み込み.
	if (FAILED(m_pDbgText->Init(*m_pDx11)))
	{
		// エラー処理
		return;
	}

	//スタティックメッシュの読み込み.
	m_pStaticMeshFighter->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Mesh\\Static\\Fighter\\Fighter.x"));
	//地面の読み込み.
	m_pStaticMeshGround->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Mesh\\Static\\Ground\\box.x"));
	//バウンディングスフィア(当たり判定用).
	m_pStaticMeshBSphere->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Sphere.x"));
	//バウンディングボックス(当たり判定用).
	m_pStaticMeshBBox->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Box.x")); // BBox用メッシュの読み込み
	//壁メッシュの読み込み.
	m_pStaticMeshWall->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Box.x")); // Wallメッシュの読み込み

	// ステージと壁のサイズ定義
	const float STAGE_HALF_SIZE_X = 20.0f;
	const float STAGE_HALF_SIZE_Z = 20.0f;
	const float WALL_HEIGHT = 5.0f; // 壁の高さを調整
	const float WALL_THICKNESS = 0.5f;

	//スタティックメッシュを設定.
	m_pStcMeshObj->AttachMesh(*m_pStaticMeshFighter); // 未使用だが一応設定
	//プレイヤーを設定.
	m_pPlayer->AttachMesh(*m_pStaticMeshFighter);
	//地面スプライトを設定.
	m_pGround->AttachMesh(*m_pStaticMeshGround);

	//バウンディングスフィアの作成.
	m_pPlayer->CreateBSphereForMesh(*m_pStaticMeshBSphere);
	// プレイヤーのBoundingBoxも作成 (地面とのAABB判定用)
	m_pPlayer->CreateBBoxForMesh(*m_pStaticMeshBBox);


	//地面のBoundingBoxの作成 (CGame::LoadData()でやったように、地面のスケールも設定する)
	const float STAGE_FULL_SIZE_X = STAGE_HALF_SIZE_X * 2.0f;
	const float STAGE_FULL_SIZE_Z = STAGE_HALF_SIZE_Z * 2.0f;
	const float GROUND_THICKNESS = 0.5f; // 地面の厚み (表示用)
	m_pGround->SetScale(STAGE_FULL_SIZE_X, GROUND_THICKNESS, STAGE_FULL_SIZE_Z);
	m_pGround->SetPosition(0.0f, -GROUND_THICKNESS / 2.0f, 0.0f); // 地面がY=0に接するように設定
	m_pGround->CreateBBoxForMesh(*m_pStaticMeshGround);


	//キャラクターの初期座標を決定.
	m_pPlayer->SetPosition(0.f, 1.f, 6.f);

	m_pEnemyA = new CEnemyTypeA();
	m_pEnemyA->SetPosition(0.f, 1.f, 5.f);
	m_pEnemyA->SetPlayer(dynamic_cast<CPlayer*>(m_pPlayer));

	// ★修正: 4つの壁を作成し、リストに追加 - Y座標を1.0fに設定
	// 壁のY座標の基準点を1.0fに設定
	const float WALL_BASE_Y = 1.0f;

	// 北の壁 (-Z方向)
	CStaticMeshObject* pWallNorth = new CStaticMeshObject();
	pWallNorth->AttachMesh(*m_pStaticMeshWall);
	pWallNorth->SetScale(STAGE_FULL_SIZE_X + WALL_THICKNESS * 2.0f, WALL_HEIGHT, WALL_THICKNESS);
	pWallNorth->SetPosition(0.0f, -10.0f, -38.5f); // Y座標を1.0fに設定
	pWallNorth->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallNorth);

	// 南の壁 (+Z方向)
	CStaticMeshObject* pWallSouth = new CStaticMeshObject();
	pWallSouth->AttachMesh(*m_pStaticMeshWall);
	pWallSouth->SetScale(STAGE_FULL_SIZE_X + WALL_THICKNESS * 2.0f, WALL_HEIGHT, WALL_THICKNESS);
	pWallSouth->SetPosition(0.0f, -10.0f, 38.5f); // Y座標を1.0fに設定
	pWallSouth->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallSouth);

	// 東の壁 (+X方向)
	CStaticMeshObject* pWallEast = new CStaticMeshObject();
	pWallEast->AttachMesh(*m_pStaticMeshWall);
	pWallEast->SetScale(WALL_THICKNESS, WALL_HEIGHT, STAGE_FULL_SIZE_Z); // 角は東西の壁がカバーするように調整
	pWallEast->SetPosition(38.5f, -10.f, 0.0f); // Y座標を1.0fに設定
	pWallEast->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallEast);

	// 西の壁 (-X方向)
	CStaticMeshObject* pWallWest = new CStaticMeshObject();
	pWallWest->AttachMesh(*m_pStaticMeshWall);
	pWallWest->SetScale(WALL_THICKNESS, WALL_HEIGHT, STAGE_FULL_SIZE_Z); // 角は東西の壁がカバーするように調整
	pWallWest->SetPosition(-38.5f,-10.f, 0.0f); // Y座標を1.0fに設定
	pWallWest->CreateBBoxForMesh(*m_pStaticMeshWall);
	m_pWalls.push_back(pWallWest);


	// ゲーム開始時にカメラの位置を即座に設定 (遅延をなくすため)
	// ThirdPersonCameraのロジックをここで直接適用し、補間なしで初期化する
	float cameraHeightOffset = 15.0f; // プレイヤーのY座標からの高さオフセット
	float cameraBackOffset = 8.0f;	// プレイヤーの背後へのオフセット

	// カメラの初期位置を設定
	m_Camera.vPosition.x = m_pPlayer->GetPosition().x;
	m_Camera.vPosition.y = m_pPlayer->GetPosition().y + cameraHeightOffset;
	m_Camera.vPosition.z = m_pPlayer->GetPosition().z - cameraBackOffset;

	// カメラの初期注視点をプレイヤーの中心に設定
	m_Camera.vLook = m_pPlayer->GetPosition();

	// カメラの目標位置と注視点も初期位置に合わせて更新しておく
	m_vCameraTargetPosition = m_Camera.vPosition;
	m_vCameraTargetLookAt = m_Camera.vLook;


}

void CGameMain::Update()
{
	//地面を更新し、そのBoundingBoxも更新
	m_pGround->Update();
	m_pGround->UpdateBSpherePos(); // 地面のBBoxを更新 (衝突判定前に必要)

	//プレイヤーを更新し、そのBoundingBoxとBSphereも更新
	m_pPlayer->Update();
	m_pPlayer->UpdateBSpherePos(); // プレイヤーのBBoxとBSphereを更新 (衝突判定前に必要)

	m_pEnemyA->Update();

	// 地面とプレイヤーの当たり判定.
	BoundingBox* playerBBox = m_pPlayer->GetBBox();
	BoundingBox* groundBBox = m_pGround->GetBBox(); // 地面のBBoxはここで取得

	if (playerBBox && groundBBox)
	{
		if (playerBBox->IsHit(*groundBBox))
		{
			// プレイヤーのY座標を地面の最大Y座標に設定し、地面の上に立っているようにする
			D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
			playerPos.y = groundBBox->GetMaxPosition().y + (playerBBox->GetSize().y / 2.0f);
			m_pPlayer->SetPosition(playerPos.x, playerPos.y, playerPos.z);
			// 必要に応じて、プレイヤーのY方向の速度を0にするなどの処理も追加
		}
	}

	// 壁とプレイヤーの当たり判定
	if (playerBBox)
	{
		D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();

		for (CStaticMeshObject* pWall : m_pWalls)
		{
			pWall->Update(); // 壁オブジェクトの更新 (ワールド行列、BoundingBoxの位置など)
			pWall->UpdateBSpherePos(); // BoundingBoxの位置も更新されることを期待

			BoundingBox* wallBBox = pWall->GetBBox();

			if (wallBBox && playerBBox->IsHit(*wallBBox))
			{
				// プレイヤーと壁の重なりを計算し、プレイヤーを押し戻す
				D3DXVECTOR3 playerMin = playerBBox->GetMinPosition();
				D3DXVECTOR3 playerMax = playerBBox->GetMaxPosition();
				D3DXVECTOR3 wallMin = wallBBox->GetMinPosition();
				D3DXVECTOR3 wallMax = wallBBox->GetMaxPosition();

				// X軸方向の重なり
				float overlapX1 = playerMax.x - wallMin.x; // プレイヤーの右端が壁の左端を越えた量
				float overlapX2 = wallMax.x - playerMin.x; // 壁の右端がプレイヤーの左端を越えた量

				// Z軸方向の重なり
				float overlapZ1 = playerMax.z - wallMin.z; // プレイヤーの手前側が壁の奥側を越えた量
				float overlapZ2 = wallMax.z - playerMin.z; // 壁の手前側がプレイヤーの奥側を越えた量

				float pushX = 0.0f;
				// プレイヤーが壁の左側から接触した場合 (playerMax.x > wallMin.x, playerMin.x < wallMin.x)
				// もしくはプレイヤーが壁の右側から接触した場合 (playerMin.x < wallMax.x, playerMax.x > wallMax.x)
				if (overlapX1 < overlapX2) { // プレイヤーが壁の左側から深く入り込んでいる
					pushX = overlapX1; // 壁の左側から押し戻す量
				}
				else { // プレイヤーが壁の右側から深く入り込んでいる
					pushX = -overlapX2; // 壁の右側から押し戻す量
				}

				float pushZ = 0.0f;
				if (overlapZ1 < overlapZ2) { // プレイヤーが壁の奥側から深く入り込んでいる
					pushZ = overlapZ1; // 壁の奥側から押し戻す量
				}
				else { // プレイヤーが壁の手前側から深く入り込んでいる
					pushZ = -overlapZ2; // 壁の手前側から押し戻す量
				}

				// 重なりが小さい方の軸に沿って押し戻す (最小貫通軸)
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


	//三人称カメラを更新
	// プレイヤーを上空から見下ろすようにカメラを調整します。
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

	// 壁の描画
	for (CStaticMeshObject* pWall : m_pWalls)
	{
		pWall->Draw(m_mView, m_mProj, m_Light, m_Camera);
	}


	// デバッグテキストの描画 (必要であれば)
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

	m_pEnemyA->Draw(m_mView, m_mProj, m_Light, m_Camera);
}

HRESULT CGameMain::LoadData()
{
	// ここで実際のロード処理を行うか、Create()に集約されているのであればE_NOTIMPLのままでもよい
	// 現在のCGameMainではCreate()でほとんどのInit/Load処理が行われているようです
	return S_OK; // 成功を返す
}

void CGameMain::Camera()
{
	D3DXVECTOR3 cam_pos = m_Camera.vPosition;
	D3DXVECTOR3 cam_look = m_Camera.vLook;
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);	//上方（ベクトル）.

	//ビュー（カメラ）変換.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&cam_pos, &cam_look, &vUpVec);
}

void CGameMain::Projection()
{
	//y方向の視野角。数値を大きくしたら視野が狭くなる.
	float fov_y = static_cast<FLOAT>(D3DXToRadian(45.0));	//ラジアン値.
	//アスペクト（幅÷高さ）.
	float aspect = static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H);
	float near_z = 0.1f;
	float far_z = 100.0f;

	//プロジェクション（射影）変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		fov_y,		//視野角（FOV：Field of View）.
		aspect,		//アスペクト.
		near_z,		//近いビュー平面のz値.
		far_z);	//遠いビュー平面のz値.
}

// プレイヤーを上空から見下ろすようにカメラを調整する関数
void CGameMain::ThirdPersonCamera(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY)
{
	// カメラの目標位置を計算
	// プレイヤーの真上ではなく、少しプレイヤーの背後から見下ろすような位置
	float cameraHeightOffset = 15.0f; // プレイヤーのY座標からの高さオフセット (変更)
	float cameraBackOffset = 8.0f;	// プレイヤーの背後へのオフセット (変更)

	// カメラの目標位置を設定
	m_vCameraTargetPosition.x = TargetPos.x;
	m_vCameraTargetPosition.y = TargetPos.y + cameraHeightOffset;
	m_vCameraTargetPosition.z = TargetPos.z - cameraBackOffset; // プレイヤーの背後

	// カメラの目標注視点をプレイヤーの中心に設定
	m_vCameraTargetLookAt = TargetPos;

	// 現在のカメラ位置と目標位置の間を滑らかに補間
	pCamera->vPosition.x = pCamera->vPosition.x + (m_vCameraTargetPosition.x - pCamera->vPosition.x) * m_fCameraSmoothSpeed;
	pCamera->vPosition.y = pCamera->vPosition.y + (m_vCameraTargetPosition.y - pCamera->vPosition.y) * m_fCameraSmoothSpeed;
	pCamera->vPosition.z = pCamera->vPosition.z + (m_vCameraTargetPosition.z - pCamera->vPosition.z) * m_fCameraSmoothSpeed;

	// 現在のカメラ注視点と目標注視点の間を滑らかに補間
	pCamera->vLook.x = pCamera->vLook.x + (m_vCameraTargetLookAt.x - pCamera->vLook.x) * m_fCameraSmoothSpeed;
	pCamera->vLook.y = pCamera->vLook.y + (m_vCameraTargetLookAt.y - pCamera->vLook.y) * m_fCameraSmoothSpeed;
	pCamera->vLook.z = pCamera->vLook.z + (m_vCameraTargetLookAt.z - pCamera->vLook.z) * m_fCameraSmoothSpeed;
}
