#include "CAllAttackBase.h"

CAllAttackBase::CAllAttackBase(const std::string& name)
	: m_strName (name) 
{
}

CAllAttackBase::~CAllAttackBase()
{
}

void CAllAttackBase::Update()
{
}

void CAllAttackBase::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CStaticMeshObject::Draw(View, Proj, Light, Camera);
}
