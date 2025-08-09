#include "CEnemyTypeA.h"
#include "StaticMash//CStaticMeshManager.h"
#include "..//..//Player/CPlayer.h"

CEnemyTypeA::CEnemyTypeA()
	: m_pPlayer		(nullptr)
	, m_MoveSpeed	(0.05f)
{
	AttachMesh(*CStaticMeshManager::GetInstance()->GetMeshInstance(CStaticMeshManager::CMeshList::Enemy));
}

CEnemyTypeA::~CEnemyTypeA()
{
}

void CEnemyTypeA::Update()
{
	//敵Aの動作(プレイヤー)の方に向かってくる処理を作成しないといけない
	if (m_pPlayer == nullptr)
	{
		return;
	}// プレイヤー未設定なら処理しない

	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
	D3DXVECTOR3 dir = playerPos - m_vPosition;  // 自分からプレイヤーへのベクトル

	if (D3DXVec3LengthSq(&dir) > 0.0001f)
	{
		D3DXVec3Normalize(&dir, &dir);
		m_vPosition += dir * m_MoveSpeed;
	}

	// 必要に応じて回転などもここで制御可能

	// 基底クラスのUpdateも呼ぶ
	CEnemyBase::Update();
}

void CEnemyTypeA::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{

	CEnemyBase::Draw(View, Proj, Light, Camera);
}

void CEnemyTypeA::SetPlayer(CPlayer* player)
{
	m_pPlayer = player;
}
