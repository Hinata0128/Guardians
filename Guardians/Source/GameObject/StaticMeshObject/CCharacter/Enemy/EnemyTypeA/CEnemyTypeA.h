#pragma once
#include "GameObject//StaticMeshObject//CCharacter//Enemy//CEnemyBase.h"

//�O���錾
class CPlayer;

/********************************************************
*	�G�^�C�vA(�q�N���X)
**/

class CEnemyTypeA final
	: public CEnemyBase
{
public:
	CEnemyTypeA();
	~CEnemyTypeA() override;

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	void SetPlayer(CPlayer* player);

private:
	CPlayer* m_pPlayer;

	float m_MoveSpeed;
};