#pragma once
#include "GameObject//StaticMeshObject//CCharacter//Enemy//CEnemyBase.h"


/********************************************************
*	敵タイプA(子クラス)
**/

class CEnemyTypeA final
	: public CEnemyBase
{
public:
	// x, y, z 座標を受け取るコンストラクタを追加
	CEnemyTypeA(float a_X, float a_Y, float a_Z);

	// これまでの引数なしのコンストラクタも残しておきます
	CEnemyTypeA();

	~CEnemyTypeA() override;

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;
private:
	float m_MoveSpeed;
};