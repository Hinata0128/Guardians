#pragma once

#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"

/**************************************************
*	プレイヤークラス.
**/
class CPlayer
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	CPlayer();
	virtual ~CPlayer() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	void HandleGroundCollision(CStaticMeshObject* pGroundObject);

protected:
	float		m_MoveSpeed;	//移動速度.
	float       m_TurnSpeed;    // 回転速度を追加
};