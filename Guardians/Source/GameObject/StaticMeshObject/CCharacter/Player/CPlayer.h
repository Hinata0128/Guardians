#pragma once

#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"

/**************************************************
*	�v���C���[�N���X.
**/
class CPlayer
	: public CCharacter	//�L�����N�^�[�N���X���p��.
{
public:
	//�ړ���ԗ񋓌^.
	enum enMoveState
	{
		stop = 0,	//��~.
		Forward,	//�O�i.
		Backward,	//���.
		TurnLeft,	//���ړ�.
		TurnRight,	//�E�ړ�.
	};
public:
	CPlayer();
	virtual ~CPlayer() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//���W�R������.
	void RadioControl();

	void HandleGroundCollision(CStaticMeshObject* pGroundObject);

protected:
	float		m_TurnSpeed;	//��]���x.
	float		m_MoveSpeed;	//�ړ����x.
	enMoveState m_MoveState;	//�ړ����.
};