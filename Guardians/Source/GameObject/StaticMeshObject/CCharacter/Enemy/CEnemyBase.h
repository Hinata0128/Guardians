#pragma once
#include "GameObject//StaticMeshObject//CCharacter//CCharacter.h"

/********************************************************
*	敵クラス(基底クラス).
* 基底クラスなので基本ここには何も書かない
* 今回のゲームは、時短のため敵1体で作成していきます.
**/

class CEnemyBase
	: public CCharacter
{
public:
	CEnemyBase();
	~CEnemyBase() override;

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

protected:

};