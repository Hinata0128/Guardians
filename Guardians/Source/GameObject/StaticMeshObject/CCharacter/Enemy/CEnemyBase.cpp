#include "CEnemyBase.h"

CEnemyBase::CEnemyBase()
{
}

CEnemyBase::~CEnemyBase()
{
}

void CEnemyBase::Update()
{
}

void CEnemyBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw(View, Proj, Light, Camera);
}

void CEnemyBase::SetPlayer(CPlayer* player)
{
	m_pPlayer = player;
}

void CEnemyBase::UpdateLifetime()
{
	m_fLifetime += 1.0f / 60.0f; // 60FPS‚ð‘z’è
}
