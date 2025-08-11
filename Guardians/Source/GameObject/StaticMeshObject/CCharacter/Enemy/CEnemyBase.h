#pragma once
#include "GameObject//StaticMeshObject//CCharacter//CCharacter.h"

class CPlayer;

/********************************************************
*	�G�N���X(���N���X).
* ���N���X�Ȃ̂Ŋ�{�����ɂ͉��������Ȃ�
* ����̃Q�[���́A���Z�̂��ߓG1�̂ō쐬���Ă����܂�.
**/

class CEnemyBase
	: public CCharacter
{
public:
	CEnemyBase();
	~CEnemyBase() override;

	void Update() override;
	void Draw(D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera) override;

	// �v���C���[�̃|�C���^��ݒ肷��֐�
	void SetPlayer(CPlayer* player);

protected:
	CPlayer* m_pPlayer;
};