#include "CEnemyManager.h"
#include "..//..//CCharacter/Player/CPlayer.h"
#include <algorithm> // std::remove_if に必要

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
			// ✅ 各敵の生存時間を更新 (CEnemyBase::m_fLifetime が存在することを前提)
			enemy->UpdateLifetime();
			enemy->Update();
		}
	}

	// ✅ 敵の破棄チェックを毎フレーム実行
	CheckAndRemoveEnemies();
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

// ✅ CheckAndRemoveEnemies() の実装を修正
void CEnemyManager::CheckAndRemoveEnemies()
{
	// 5秒以上経過した敵を削除する
	m_Enemies.erase(
		std::remove_if(m_Enemies.begin(), m_Enemies.end(),
			[](const std::unique_ptr<CEnemyBase>& enemy) {
				return enemy->GetLifetime() >= 5.0f;
			}),
		m_Enemies.end());
}