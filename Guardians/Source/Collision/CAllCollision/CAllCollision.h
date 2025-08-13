#pragma once
#include "StaticMash/CStaticMesh.h"
#include "..//..//GameObject/StaticMeshObject/CCharacter/EnemyManager/CEnemyManager.h"
#include "..//..//GameObject/StaticMeshObject/CCharacter/Player/CPlayer.h"

/**********************************************************************************
*	“–‚½‚è”»’èƒNƒ‰ƒX
* Œp³ŠÖŒW‚Í‚³‚¹‚È‚¢
**/

class CAllCollision final
{
public:
	CAllCollision();
	~CAllCollision();

	//‚·‚×‚Ä‚Ì“–‚½‚è”»’è‚ğÀ‘•‚·‚é.
	void AllCollision();

	void Update();

private:
	CEnemyManager* m_pEnemy;
	CPlayer* m_pPlayer;
};