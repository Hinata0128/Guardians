#pragma once
#include "..//..//StaticMeshObject/CStaticMeshObject.h"

#include <string>

/*************************************************************************
*	プレイヤー・敵の攻撃の基底クラス.
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
	std::string m_strName; // 攻撃の名前やリソースID
};