#include "CPlayerAttack.h"
#include "..//..//..//..//StaticMash/CStaticMeshManager.h"

CPlayerAttack::CPlayerAttack(const D3DXVECTOR3& startPos, const D3DXVECTOR3& dir, float speed, const std::string& name)
	: CAllAttackBase(name)
	, m_vDir(dir)
	, m_fSpeed(speed)
	, m_fLifeTime(3.f)	//3�b�̎���
{
	AttachMesh(*CStaticMeshManager::GetInstance()->GetMeshInstance(CStaticMeshManager::CMeshList::PAttack));

	m_vPosition = startPos;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

CPlayerAttack::~CPlayerAttack()
{
}

void CPlayerAttack::Update()
{
	//�e�̔��ˑ��x����
	m_vPosition += m_vDir * m_fSpeed;
	m_fLifeTime -= 1.f / 60.f;	//60FPS�w��
}

void CPlayerAttack::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CAllAttackBase::Draw(View, Proj, Light, Camera);
}

bool CPlayerAttack::IsDead() const
{
	return m_fLifeTime <= 0.f;
}
