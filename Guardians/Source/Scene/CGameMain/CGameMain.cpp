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
{
	m_pDx11 = pDx11;
	m_pDx9 = pDx9;

	//スタティックメッシュマネージャーの構築
	CStaticMeshManager::GetInstance()->Create(*pDx9, *pDx11);

	//カメラの位置を変更できるところ.
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

	// ★変更: 複数の壁オブジェクトを解放
	for (auto it = m_Walls.rbegin(); it != m_Walls.rend(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_Walls.clear(); // vector の要素をクリア

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
	m_Camera.vPosition = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_Camera.vLook = D3DXVECTOR3(0.0f, 2.0f, 5.0f);
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

	// ★変更: 複数の壁オブジェクトを生成し、vectorに追加
	const int NUM_WALLS = 4; // 四方に壁を置くので4つ
	m_Walls.reserve(NUM_WALLS); // 事前にメモリ確保 (任意)
	for (int i = 0; i < NUM_WALLS; ++i)
	{
		m_Walls.push_back(new CStaticMeshObject());
	}

	const float STAGE_HALF_SIZE_X = 20.0f;
	const float STAGE_HALF_SIZE_Z = 20.0f;
	const float WALL_HEIGHT = 2.0f;
	const float WALL_THICKNESS = 0.5f;

	// 各壁の設定
	// 1. 奥の壁 (+Z方向)
	m_Walls[0]->AttachMesh(*m_pStaticMeshWall);
	m_Walls[0]->SetScale(STAGE_HALF_SIZE_X * 2.0f + WALL_THICKNESS * 2.0f, WALL_HEIGHT, WALL_THICKNESS);
	m_Walls[0]->SetPosition(0.0f, WALL_HEIGHT / 2.0f, STAGE_HALF_SIZE_Z + WALL_THICKNESS / 2.0f);
	m_Walls[0]->CreateBBoxForMesh(*m_pStaticMeshWall); // BoundingBoxも作成

	// 2. 手前の壁 (-Z方向)
	m_Walls[1]->AttachMesh(*m_pStaticMeshWall);
	m_Walls[1]->SetScale(STAGE_HALF_SIZE_X * 2.0f + WALL_THICKNESS * 2.0f, WALL_HEIGHT, WALL_THICKNESS);
	m_Walls[1]->SetPosition(0.0f, WALL_HEIGHT / 2.0f, -STAGE_HALF_SIZE_Z - WALL_THICKNESS / 2.0f);
	m_Walls[1]->CreateBBoxForMesh(*m_pStaticMeshWall); // BoundingBoxも作成

	// 3. 左の壁 (-X方向)
	m_Walls[2]->AttachMesh(*m_pStaticMeshWall);
	m_Walls[2]->SetScale(WALL_THICKNESS, WALL_HEIGHT, STAGE_HALF_SIZE_Z * 2.0f);
	m_Walls[2]->SetPosition(-STAGE_HALF_SIZE_X - WALL_THICKNESS / 2.0f, WALL_HEIGHT / 2.0f, 0.0f);
	m_Walls[2]->CreateBBoxForMesh(*m_pStaticMeshWall); // BoundingBoxも作成

	// 4. 右の壁 (+X方向)
	m_Walls[3]->AttachMesh(*m_pStaticMeshWall);
	m_Walls[3]->SetScale(WALL_THICKNESS, WALL_HEIGHT, STAGE_HALF_SIZE_Z * 2.0f);
	m_Walls[3]->SetPosition(STAGE_HALF_SIZE_X + WALL_THICKNESS / 2.0f, WALL_HEIGHT / 2.0f, 0.0f);
	m_Walls[3]->CreateBBoxForMesh(*m_pStaticMeshWall); // BoundingBoxも作成

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
}

void CGameMain::Update()
{
	//地面を更新し、そのBoundingBoxも更新
	m_pGround->Update();
	m_pGround->UpdateBSpherePos(); // 地面のBBoxを更新 (衝突判定前に必要)

	//プレイヤーを更新し、そのBoundingBoxとBSphereも更新
	m_pPlayer->Update();
	m_pPlayer->UpdateBSpherePos(); // プレイヤーのBBoxとBSphereを更新 (衝突判定前に必要)

	//三人称カメラ.
	// CGame::ThirdPersonCamera(
	//     &m_Camera,
	//     m_pPlayer->GetPosition(),
	//     m_pPlayer->GetRotation().y);

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

	// ★変更: 複数の壁オブジェクトの更新とプレイヤーとのAABB衝突判定
	for (auto& wall : m_Walls) {
		wall->Update(); // 各壁オブジェクトを更新
		wall->UpdateBSpherePos(); // ★追加: 壁のBBoxも更新 (衝突判定前に必要)
	}

	if (playerBBox) // プレイヤーのBBoxが有効な場合のみ壁との判定を行う
	{
		for (auto& wall : m_Walls)
		{
			BoundingBox* wallBBox = wall->GetBBox();

			if (wallBBox && playerBBox->IsHit(*wallBBox))
			{
				OutputDebugStringA("Player hit A Wall!\n");

				D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();

				const D3DXVECTOR3& playerMin = playerBBox->GetMinPosition();
				const D3DXVECTOR3& playerMax = playerBBox->GetMaxPosition();
				const D3DXVECTOR3& wallMin = wallBBox->GetMinPosition();
				const D3DXVECTOR3& wallMax = wallBBox->GetMaxPosition();

				float overlapX = 0.0f;
				float overlapY = 0.0f;
				float overlapZ = 0.0f;

				if (playerMin.x < wallMax.x && playerMax.x > wallMin.x) {
					overlapX = min(playerMax.x - wallMin.x, wallMax.x - playerMin.x);
				}
				if (playerMin.y < wallMax.y && playerMax.y > wallMin.y) {
					overlapY = min(playerMax.y - wallMin.y, wallMax.y - playerMin.y);
				}
				if (playerMin.z < wallMax.z && playerMax.z > wallMin.z) {
					overlapZ = min(playerMax.z - wallMin.z, wallMax.z - playerMin.z);
				}

				if (overlapX > 0 && (overlapX <= overlapZ || overlapZ == 0)) {
					if (playerPos.x < wallBBox->GetCenter().x) {
						playerPos.x = wallMin.x - (playerBBox->GetSize().x / 2.0f);
					}
					else {
						playerPos.x = wallMax.x + (playerBBox->GetSize().x / 2.0f);
					}
				}
				else if (overlapZ > 0 && (overlapZ < overlapX || overlapX == 0)) {
					if (playerPos.z < wallBBox->GetCenter().z) {
						playerPos.z = wallMin.z - (playerBBox->GetSize().z / 2.0f);
					}
					else {
						playerPos.z = wallMax.z + (playerBBox->GetSize().z / 2.0f);
					}
				}
				m_pPlayer->SetPosition(playerPos.x, playerPos.y, playerPos.z);
				break; // 最初の壁との衝突応答でループを抜ける
			}
		}
	}



	// --- ボスの当たり判定コード修正終わり ---

	//三人称カメラ
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

	// ★変更: 複数の壁を描画
	for (auto& wall : m_Walls)
	{
		wall->Draw(m_mView, m_mProj, m_Light, m_Camera);
	}

	// ★変更: BoundingBox/Sphere位置の更新はUpdate()に移動したのでここからは削除

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

void CGameMain::ThirdPersonCamera(CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY)
{
	//Z軸ベクトル（Z+方向の単位ベクトル）
	D3DXVECTOR3 vecAxisZ(0.f, 0.f, 1.f);

	//Y方向の回転行列
	D3DXMATRIX mRotationY;
	//Y軸回転行列を作成
	D3DXMatrixRotationY(
		&mRotationY,	//(out)行列
		TargetRotY);	//対象のY方向の回転値

	//Y軸回転行列を使ってZ軸ベクトルを座標変換する
	D3DXVec3TransformCoord(
		&vecAxisZ,		//(out)Z軸ベクトル
		&vecAxisZ,		//(in)Z軸ベクトル
		&mRotationY);	//Y軸回転行列

	//カメラの位置、注視位置を対象にそろえる
	pCamera->vPosition = TargetPos;
	pCamera->vLook = TargetPos;

	//カメラの位置、注視位置をZ軸ベクトルを用いて調整
	pCamera->vPosition -= vecAxisZ * 4.f;	//対象の背中側
	pCamera->vLook += vecAxisZ * 3.f;	//対象を挟んで向こう側

	//カメラの位置、注視位置の高さをそれぞれ微調整
	pCamera->vPosition.y += 2.f;
	pCamera->vLook.y += 0.5f;
}