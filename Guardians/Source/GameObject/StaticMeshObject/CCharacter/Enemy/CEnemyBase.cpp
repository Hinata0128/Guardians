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
