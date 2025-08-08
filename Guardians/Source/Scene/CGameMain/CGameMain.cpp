#include "CGameMain.h"
#include "Sound/CSoundManager.h"
#include "StaticMash/CStaticMeshManager.h"
#include "Collision/BoundingBox/BoundingBox.h" // BoundingBoxクラスを使用するために必要
#include <algorithm> // std::min のために必要

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
	, m_vCameraTargetLookAt(0.0f, 0.0f, 0.0f)   // カメラターゲット注視点の初期化
	, m_fCameraSmoothSpeed(0.05f)               // カメラ追従速度の初期化
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
	//地面の破棄.
	SAFE_DELETE(m_pGround);

	//プレイヤーの破棄.
	SAFE_DELETE(m_pPlayer);

	//スタティックメッシュオブジェクトの破棄 (未使用だが一応)
	SAFE_DELETE(m_pStcMeshObj);

	//スタティックメッシュの破棄
	SAFE_DELETE(m_pStaticMeshFighter); // ★追加: Fighterメッシュの解放
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
		_T("Data\\Mesh\\Static\\Ground\\ground.x"));
	//バウンディングスフィア(当たり判定用).
	m_pStaticMeshBSphere->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Sphere.x"));
	//バウンディングボックス(当たり判定用).
	m_pStaticMeshBBox->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Box.x")); // BBox用メッシュの読み込み
	//壁メッシュの読み込み.
	m_pStaticMeshWall->Init(*m_pDx9, *m_pDx11,
		_T("Data\\Collision\\Box.x")); // Wallメッシュの読み込み

	const float STAGE_HALF_SIZE_X = 20.0f;
	const float STAGE_HALF_SIZE_Z = 20.0f;
	const float WALL_HEIGHT = 2.0f;
	const float WALL_THICKNESS = 0.5f;

	//スタティックメッシュを設定.
	m_pStcMeshObj->AttachMesh(*m_pStaticMeshFighter); // 未使用だが一応設定
	//プレイヤーを設定.
	m_pPlayer->AttachMesh(*m_pStaticMeshFighter);
	//地面スプライトを設定.
	m_pGround->AttachMesh(*m_pStaticMeshGround);

	//バウンディングスフィアの作成.
	m_pPlayer->CreateBSphereForMesh(*m_pStaticMeshBSphere);
	// ★追加: プレイヤーのBoundingBoxも作成 (地面とのAABB判定用)
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

	// ★追加: ゲーム開始時にカメラの位置を即座に設定 (遅延をなくすため)
	// ThirdPersonCameraのロジックをここで直接適用し、補間なしで初期化する
	float cameraHeightOffset = 15.0f; // プレイヤーのY座標からの高さオフセット
	float cameraBackOffset = 8.0f;   // プレイヤーの背後へのオフセット

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

	// 地面とプレイヤーの当たり判定.
	BoundingBox* playerBBox = m_pPlayer->GetBBox();
	BoundingBox* groundBBox = m_pGround->GetBBox(); // 地面のBBoxはここで取得

	if (playerBBox && groundBBox)
	{
		if (playerBBox->IsHit(*groundBBox))
		{
			// CPlayer::HandleGroundCollision が適切に実装されているなら、そちらを呼び出す方がカプセル化されています。
			// プレイヤーのY座標を地面の最大Y座標に設定し、地面の上に立っているようにする
			D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
			playerPos.y = groundBBox->GetMaxPosition().y + (playerBBox->GetSize().y / 2.0f);
			m_pPlayer->SetPosition(playerPos.x, playerPos.y, playerPos.z);
			// 必要に応じて、プレイヤーのY方向の速度を0にするなどの処理も追加
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
	float cameraBackOffset = 8.0f; // プレイヤーの背後へのオフセット (変更)

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
