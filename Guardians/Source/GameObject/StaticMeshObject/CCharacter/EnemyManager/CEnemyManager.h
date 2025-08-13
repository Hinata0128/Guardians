#pragma once
#include <vector>
#include <memory>
#include "..//Enemy/CEnemyBase.h"
#include "..//Enemy/EnemyTypeA/CEnemyTypeA.h"

class CPlayer;

/****************************************************
*	敵マネージャークラス
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

	// 引数を4つ(x, y, z, Playerのポジションを手に入れるために必要な引数).
	void CreateEnemy(EnemyType a_Type, float a_X, float a_Y, float a_Z, CPlayer* a_pPlayer);
	void Update(); // ✅ CheckAndRemoveEnemies() の呼び出しをここで行います
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera);

private:
	std::vector<std::unique_ptr<CEnemyBase>> m_Enemies;
	// float m_fLifetime = 0.0f; // ✅ この行は敵個体のものではないため、削除または使用しない
	void CheckAndRemoveEnemies(); // ✅ プライベートメソッドに変更
};