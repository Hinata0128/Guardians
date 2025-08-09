#pragma once
#include "..//..//StaticMeshObject/CStaticMeshObject.h"

#include <string>

/*************************************************************************
*	�v���C���[�E�G�̍U���̊��N���X.
**/

class CAllAttackBase
	: public CStaticMeshObject
{
public:
	CAllAttackBase(const std::string& name);
	virtual ~CAllAttackBase() override;

	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

protected:
	std::string m_strName; // �U���̖��O�⃊�\�[�XID
};