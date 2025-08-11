#pragma once
#include "GameObject//StaticMeshObject//CCharacter//Enemy//CEnemyBase.h"


/********************************************************
*	�G�^�C�vA(�q�N���X)
**/

class CEnemyTypeA final
	: public CEnemyBase
{
public:
	// x, y, z ���W���󂯎��R���X�g���N�^��ǉ�
	CEnemyTypeA(float a_X, float a_Y, float a_Z);

	// ����܂ł̈����Ȃ��̃R���X�g���N�^���c���Ă����܂�
	CEnemyTypeA();

	~CEnemyTypeA() override;

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;
private:
	float m_MoveSpeed;
};