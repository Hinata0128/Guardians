#pragma once

#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"

#include "..//..//CAllAttackBase/CPlayerAttack/CPlayerAttack.h"

/**************************************************
*	プレイヤークラス.
**/
class CPlayer final
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	CPlayer();
	~CPlayer() override;

	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	void HandleGroundCollision(CStaticMeshObject* pGroundObject);

protected:
	float m_MoveSpeed;    //移動速度.
	float m_TurnSpeed;    //回転速度.
	bool  m_Shot;

	std::vector<std::unique_ptr<CPlayerAttack>> m_Attacks; // 弾の管理
	float m_AttackCooldown; // クールタイム秒数
	float m_AttackTimer;    // クールタイム残り時間
};