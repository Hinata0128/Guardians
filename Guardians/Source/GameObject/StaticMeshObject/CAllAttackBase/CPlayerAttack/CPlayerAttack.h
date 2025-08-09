#pragma once
#include "..//CAllAttackBase.h"

#include <vector>
#include <string>

/****************************************************************************
*	プレイヤー攻撃クラス.
**/
class CPlayerAttack final
	: public CAllAttackBase
{
public:
	//引数ありのコンストラクタ.
	CPlayerAttack(const D3DXVECTOR3& startPos, const D3DXVECTOR3& dir, float speed, const std::string& name);
	~CPlayerAttack() override;

	void Update();
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	bool IsDead() const;

private:
	D3DXVECTOR3 m_vDir;
	float       m_fSpeed;
	float       m_fLifeTime;
};
