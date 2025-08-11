#pragma once
#include <vector>
#include <memory>
#include "..//Enemy/CEnemyBase.h"
#include "..//Enemy/EnemyTypeA/CEnemyTypeA.h"

class CPlayer;

/****************************************************
*	�G�}�l�[�W���[�N���X
**/

class CEnemyManager
{
public:
	enum class EnemyType
	{
		EnemyA,

		max,
	};
public:
	CEnemyManager();
	~CEnemyManager();

	// ������3�ix, y, z�j�ɏC��
	void CreateEnemy(EnemyType a_Type, float a_X, float a_Y, float a_Z, CPlayer* a_pPlayer);
	void Update();
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

private:
	std::vector<std::unique_ptr<CEnemyBase>> m_Enemies;

};