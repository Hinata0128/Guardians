#pragma once
#include "..//..//StaticMeshObject/CStaticMeshObject.h"

/*************************************************************************
*	プレイヤー・敵の攻撃の基底クラス.
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