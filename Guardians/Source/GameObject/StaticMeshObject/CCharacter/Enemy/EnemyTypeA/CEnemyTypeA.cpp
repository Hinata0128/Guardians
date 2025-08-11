#include "CEnemyTypeA.h"
#include "StaticMash//CStaticMeshManager.h"
#include "..//..//Player/CPlayer.h"

// ��������R���X�g���N�^�̎���
// �󂯎�������W���A�e�N���X��SetPosition�֐��ŏ����ʒu�Ƃ��Đݒ肵�܂�

// ����܂ł̈����Ȃ��R���X�g���N�^�̎���
CEnemyTypeA::CEnemyTypeA(float a_X, float a_Y, float a_Z)
	: m_MoveSpeed(0.05f)
{
	AttachMesh(*CStaticMeshManager::GetInstance()->GetMeshInstance(CStaticMeshManager::CMeshList::Enemy));
	SetPosition(a_X, a_Y, a_Z);
}

CEnemyTypeA::CEnemyTypeA()
	: m_MoveSpeed(0.05f)
{
	AttachMesh(*CStaticMeshManager::GetInstance()->GetMeshInstance(CStaticMeshManager::CMeshList::Enemy));
}

CEnemyTypeA::~CEnemyTypeA()
{
}

void CEnemyTypeA::Update()
{
	if (m_pPlayer == nullptr)
	{
		return;
	}

	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
	D3DXVECTOR3 dir = playerPos - m_vPosition;

	if (D3DXVec3LengthSq(&dir) > 0.0001f)
	{
		D3DXVec3Normalize(&dir, &dir);
		m_vPosition += dir * m_MoveSpeed;
	}

	// ���N���X��Update���Ă�
	CEnemyBase::Update();
}

void CEnemyTypeA::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CEnemyBase::Draw(View, Proj, Light, Camera);
}
