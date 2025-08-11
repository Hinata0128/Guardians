#include "CEnemyManager.h"
#include "..//..//CCharacter/Player/CPlayer.h"
CEnemyManager::CEnemyManager()
{
}

CEnemyManager::~CEnemyManager()
{
}

void CEnemyManager::CreateEnemy(EnemyType a_Type, float a_X, float a_Y, float a_Z, CPlayer* a_pPlayer)
{
	switch (a_Type)
	{
	case EnemyType::EnemyA:
		// 敵を生成
		m_Enemies.push_back(std::make_unique<CEnemyTypeA>(a_X, a_Y, a_Z));
		// 生成した敵にプレイヤーのポインタをセット
		m_Enemies.back()->SetPlayer(a_pPlayer);
		break;
	case EnemyType::max:
		break;
	}
}

void CEnemyManager::Update()
{
	for (const auto& enemy : m_Enemies)
	{
		if (enemy)
		{
			enemy->Update();
		}
	}
}

void CEnemyManager::Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	for (const auto& enemy : m_Enemies)
	{
		if (enemy)
		{
			enemy->Draw(View, Proj, Light, Camera);
		}
	}
}