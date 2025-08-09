#pragma once

#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"

#include "..//..//CAllAttackBase/CPlayerAttack/CPlayerAttack.h"

/**************************************************
*	�v���C���[�N���X.
**/
class CPlayer final
	: public CCharacter	//�L�����N�^�[�N���X���p��.
{
public:
	CPlayer();
	~CPlayer() override;

	void Update() override;
	void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	void HandleGroundCollision(CStaticMeshObject* pGroundObject);

protected:
	float m_MoveSpeed;    //�ړ����x.
	float m_TurnSpeed;    //��]���x.
	bool  m_Shot;

	std::vector<std::unique_ptr<CPlayerAttack>> m_Attacks; // �e�̊Ǘ�
	float m_AttackCooldown; // �N�[���^�C���b��
	float m_AttackTimer;    // �N�[���^�C���c�莞��
};