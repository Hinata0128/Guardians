#pragma once

#include "GameObject/StaticMeshObject/CStaticMeshObject.h"

/**************************************************
*   キャラクタークラス
**/
class CCharacter
	: public CStaticMeshObject
{
public:
	CCharacter();
	virtual ~CCharacter();

	virtual void Update() override;
	virtual void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	//弾を飛ばしたいか確認
	bool IsShot() const { return m_Shot; }

protected:
	bool	m_Shot;	//弾を飛ばすフラグ
};