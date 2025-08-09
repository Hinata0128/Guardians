#pragma once

#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"

#include "..//..//CAllAttackBase/CPlayerAttack/CPlayerAttack.h"

/**************************************************
*	�v���C���[�N���X.
**/
class CPlayer
	: public CCharacter	//�L�����N�^�[�N���X���p��.
{
public:
	CPlayer();
	virtual ~CPlayer() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	void HandleGroundCollision(CStaticMeshObject* pGroundObject);

protected:
	float		m_MoveSpeed;	//�ړ����x.
	float       m_TurnSpeed;    // ��]���x��ǉ�

	std::vector<std::unique_ptr<CPlayerAttack>> m_Attacks;
	float m_ShotCoolTime;	//���ˊ��o�̎���.
};