#pragma once

#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"

/**************************************************
*	プレイヤークラス.
**/
class CPlayer
	: public CCharacter	//キャラクタークラスを継承.
{
public:
	//移動状態列挙型.
	enum enMoveState
	{
		stop = 0,	//停止.
		Forward,	//前進.
		Backward,	//後退.
		TurnLeft,	//左移動.
		TurnRight,	//右移動.
	};
public:
	CPlayer();
	virtual ~CPlayer() override;

	virtual void Update() override;
	virtual void Draw(
		D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//ラジコン操作.
	void RadioControl();

	void HandleGroundCollision(CStaticMeshObject* pGroundObject);

protected:
	float		m_TurnSpeed;	//回転速度.
	float		m_MoveSpeed;	//移動速度.
	enMoveState m_MoveState;	//移動状態.
};