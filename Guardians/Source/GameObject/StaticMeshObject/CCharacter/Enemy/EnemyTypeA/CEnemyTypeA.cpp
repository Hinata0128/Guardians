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
	//�GA�̓���(�v���C���[)�̕��Ɍ������Ă��鏈�����쐬���Ȃ��Ƃ����Ȃ�
	if (m_pPlayer == nullptr)
	{
		return;
	}// �v���C���[���ݒ�Ȃ珈�����Ȃ�

	D3DXVECTOR3 playerPos = m_pPlayer->GetPosition();
	D3DXVECTOR3 dir = playerPos - m_vPosition;  // ��������v���C���[�ւ̃x�N�g��

	if (D3DXVec3LengthSq(&dir) > 0.0001f)
	{
		D3DXVec3Normalize(&dir, &dir);
		m_vPosition += dir * m_MoveSpeed;
	}

	// �K�v�ɉ����ĉ�]�Ȃǂ������Ő���\

	// ���N���X��Update���Ă�
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
