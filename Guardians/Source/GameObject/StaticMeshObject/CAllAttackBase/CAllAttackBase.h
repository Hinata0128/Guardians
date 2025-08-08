#pragma once
#include "..//..//StaticMeshObject/CStaticMeshObject.h"

/*************************************************************************
*	�v���C���[�E�G�̍U���̊��N���X.
**/

class CAllAttackBase
	: public CStaticMeshObject
{
public:
	CAllAttackBase();
	virtual ~CAllAttackBase() override;

	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

protected:

};