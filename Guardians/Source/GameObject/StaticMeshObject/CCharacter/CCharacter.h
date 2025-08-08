#pragma once

#include "GameObject/StaticMeshObject/CStaticMeshObject.h"

/**************************************************
*   �L�����N�^�[�N���X
**/
class CCharacter
	: public CStaticMeshObject
{
public:
	CCharacter();
	virtual ~CCharacter();

	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	//�e���΂��������m�F
	bool IsShot() const { return m_Shot; }

protected:
	bool	m_Shot;	//�e���΂��t���O
};