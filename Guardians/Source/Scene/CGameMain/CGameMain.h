#pragma once
#include "Scene/CSceneBase.h"
#include "GameObject/CSpriteObject/CSpriteObject.h"
#include "SceneManager/CSceneManager.h"

#include "GameObject/StaticMeshObject/CCharacter/Ground/CGround.h"
#include "GameObject/StaticMeshObject/CCharacter/Player/CPlayer.h"
#include "DebugText/CDebugText.h"
#include "GameObject/StaticMeshObject/CCharacter/CCharacter.h"
#include "GameObject/StaticMeshObject/CStaticMeshObject.h" // CStaticMeshObject���g�p���邽�߂ɕK�v
#include "StaticMash/CStaticMesh.h" // CStaticMesh���g�p���邽�߂ɕK�v

#include <vector>


/*********************************************
*	�Q�[�����C���摜�N���X.
**/

class CGameMain
	: public CSceneBase
{
public:
	CGameMain(CDirectX9* pDx9, CDirectX11* pDx11);
	~CGameMain() override;

	void Initialize() override;
	void Create() override;
	void Update() override;
	void Draw() override;

	HRESULT LoadData();

	//�J�����֐�.
	void Camera();
	//�v���W�F�N�V�����֐�.
	void Projection();

	//�O�l�̃J����
	void ThirdPersonCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TargetPos, float TargetRotY);


private:

	//�Q�[���ň����X�v���C�g�f�[�^(�g���܂킷����).
	CSprite3D* m_pSp3D; // ���̃����o�͌��ݎg�p����Ă��Ȃ��悤�ł�
	CSprite3D* m_pSpriteGround; // ���݂̃R�[�h�ł͎g�p����Ă��܂��񂪁A�c���Ă����܂�
	CSprite3D* m_pSpriteExplosion; // ���݂̃R�[�h�ł͎g�p����Ă��܂��񂪁A�c���Ă����܂�

	//�X�^�e�B�b�N���b�V��(�g���܂킷����).
	CStaticMesh* m_pStaticMeshGround;	//�n��
	CStaticMesh* m_pStaticMeshFighter;	//�v���C���[�̃��b�V��
	CStaticMesh* m_pStaticMeshRoboA;		//���{A (���ݎg�p����Ă��܂���)
	CStaticMesh* m_pStaticMeshRoboB;		//���{B (���ݎg�p����Ă��܂���)
	CStaticMesh* m_pStaticMeshBullet;	//�e (���ݎg�p����Ă��܂���)
	CStaticMesh* m_pStaticMeshBSphere;	//�o�E���f�B���O�X�t�B�A(�����蔻��p)
	CStaticMesh* m_pStaticMeshBBox;		//�o�E���f�B���O�{�b�N�X(�����蔻��p)
	CStaticMesh* m_pStaticMeshWall;     //��

	// m_pGameMain �� m_pSpriteTitle �͌��ݎg�p����Ă��܂��񂪁A�c���Ă����܂�
	CStaticMesh* m_pGameMain;
	CSprite2D* m_pSpriteTitle;

	//�X�^�e�B�b�N���b�V���I�u�W�F�N�g�N���X.
	CStaticMeshObject* m_pStcMeshObj; // ���̃����o�͌��ݎg�p����Ă��܂���

	std::vector<CStaticMeshObject*> m_Walls; // ���ύX: m_Walls �� std::vector �Ő錾


	//�L�����N�^�[�N���X
	CCharacter* m_pPlayer; 

	//�n�ʃN���X.
	CGround* m_pGround;
	CDebugText* m_pDbgText;	//�f�o�b�N�e�L�X�g.


};