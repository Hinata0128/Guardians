// CPlayer.cpp （Updateにクールタイム処理追加）
#include "CPlayer.h"
#include "Sound/CSoundManager.h"
#include "Collision/BoundingBox/BoundingBox.h"
#include "StaticMash/CStaticMeshManager.h"
#include <cmath>

CPlayer::CPlayer()
	: m_MoveSpeed(0.1f)
	, m_TurnSpeed(D3DXToRadian(5.0f))
	, m_Shot(false)
	, m_AttackCooldown(0.5f) // クールタイム3秒
	, m_AttackTimer(0.0f)
{
	AttachMesh(*CStaticMeshManager::GetInstance()->GetMeshInstance(CStaticMeshManager::CMeshList::Player));
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
	D3DXVECTOR3 moveDirection(0.0f, 0.0f, 0.0f);

	if (GetAsyncKeyState(VK_UP) & 0x8000) { moveDirection.z += 1.0f; }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { moveDirection.z -= 1.0f; }
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { moveDirection.x += 1.0f; }
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) { moveDirection.x -= 1.0f; }

	if (D3DXVec3LengthSq(&moveDirection) > 0)
	{
		D3DXVec3Normalize(&moveDirection, &moveDirection);

		m_vPosition.x += moveDirection.x * m_MoveSpeed;
		m_vPosition.z += moveDirection.z * m_MoveSpeed;

		float targetRotationY = atan2f(moveDirection.x, moveDirection.z);
		float diff = targetRotationY - m_vRotation.y;
		if (diff > D3DX_PI) diff -= 2 * D3DX_PI;
		if (diff < -D3DX_PI) diff += 2 * D3DX_PI;
		m_vRotation.y += diff * m_TurnSpeed;
		m_vRotation.y = fmodf(m_vRotation.y, 2 * D3DX_PI);
		if (m_vRotation.y > D3DX_PI) m_vRotation.y -= 2 * D3DX_PI;
		if (m_vRotation.y < -D3DX_PI) m_vRotation.y += 2 * D3DX_PI;
	}

	// クールタイム減算（60FPS想定）
	if (m_AttackTimer > 0.0f)
	{
		m_AttackTimer -= 1.f / 60.f;
		if (m_AttackTimer < 0.0f)
			m_AttackTimer = 0.0f;
	}

	m_Shot = false;
	if ((GetAsyncKeyState('Z') & 0x8000) && m_AttackTimer <= 0.0f)
	{
		m_Shot = true;
		m_AttackTimer = m_AttackCooldown; // クールタイムリセット

		// 弾の発射処理
		D3DXVECTOR3 dir(0.0f, 0.0f, 1.0f);
		D3DXMATRIX rotY;
		D3DXMatrixRotationY(&rotY, m_vRotation.y);
		D3DXVec3TransformNormal(&dir, &dir, &rotY);

		m_Attacks.push_back(std::make_unique<CPlayerAttack>(
			m_vPosition, dir, 0.3f, "PlayerBullet"
		));
	}

	// 弾の更新・削除処理
	for (auto it = m_Attacks.begin(); it != m_Attacks.end(); )
	{
		(*it)->Update();
		if ((*it)->IsDead())
		{
			it = m_Attacks.erase(it);
		}
		else
		{
			++it;
		}
	}

	CCharacter::Update();
}

void CPlayer::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw(View, Proj, Light, Camera);

	for (auto& atk : m_Attacks)
	{
		atk->Draw(View, Proj, Light, Camera);
	}
}

void CPlayer::HandleGroundCollision(CStaticMeshObject* pGroundObject)
{
	BoundingBox* playerBBox = this->GetBBox();
	BoundingBox* groundBBox = pGroundObject->GetBBox();

	if (playerBBox == nullptr || groundBBox == nullptr)
		return;

	if (playerBBox->GetMinPosition().y < groundBBox->GetMaxPosition().y)
	{
		float playerHalfHeight = (playerBBox->GetMaxPosition().y - playerBBox->GetMinPosition().y) / 2.0f;
		this->SetPosition(m_vPosition.x, groundBBox->GetMaxPosition().y + playerHalfHeight, m_vPosition.z);
	}
}
