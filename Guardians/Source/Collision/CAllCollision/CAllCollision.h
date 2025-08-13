#pragma once
#include "StaticMash/CStaticMesh.h"
#include "..//..//GameObject/StaticMeshObject/CCharacter/EnemyManager/CEnemyManager.h"
#include "..//..//GameObject/StaticMeshObject/CCharacter/Player/CPlayer.h"

/**********************************************************************************
*	�����蔻��N���X
* �p���֌W�͂����Ȃ�
**/

class CAllCollision final
{
public:
	CAllCollision();
	~CAllCollision();

	//���ׂĂ̓����蔻�����������.
	void AllCollision();

	void Update();

private:
	CEnemyManager* m_pEnemy;
	CPlayer* m_pPlayer;
};