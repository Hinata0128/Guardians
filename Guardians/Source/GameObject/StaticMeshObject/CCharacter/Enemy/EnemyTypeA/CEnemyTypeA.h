#pragma once
#include "GameObject//StaticMeshObject//CCharacter//Enemy//CEnemyBase.h"

//前方宣言
class CPlayer;

/********************************************************
*	敵タイプA(子クラス)
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