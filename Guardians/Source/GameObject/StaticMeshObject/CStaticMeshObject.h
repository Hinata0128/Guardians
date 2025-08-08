#pragma once

#include "GameObject/CGameObject.h"
#include "StaticMash/CStaticMesh.h"
#include "Collision/CBoundingSphere/CBoundingSphere.h"
#include "Collision/BoundingBox/BoundingBox.h"

/*************************************
*
**/
class CStaticMeshObject
	: public CGameObject
{
public:
	CStaticMeshObject();
	virtual ~CStaticMeshObject() override;

	//CGameObject�ŏ������z�֐��̐錾������Ă�̂ł�����Œ�`������.
	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//���b�V����ڑ�����.
	void AttachMesh( CStaticMesh& pMesh ){
		m_pMesh = &pMesh;
	}
	//���b�V����؂藣��.
	void DetachMesh(){
		m_pMesh = nullptr;
	}
	//�o�E���f�B���O�X�t�B�A�擾
	CBoundingSphere* GetBSphere() const {
		return m_pBSphere;
	}
	//���f���ɍ��킹���o�E���f�B���O�X�t�B�A�쐬�̃��b�p�[�֐�
	HRESULT CreateBSphereForMesh(const CStaticMesh& pMesh) {
		return m_pBSphere->CreateSphereForMesh(pMesh);
	}
	//�o�E���f�B���O�X�t�B�A���I�u�W�F�N�g�ʒu�ɍ��킹��
	//�����f���̌��_�����S�̏ꍇ��z��
	void UpdateBSpherePos() {
		m_pBSphere->SetPosition(m_vPosition);
	}

	//�o�E���f�B���O�{�b�N�X�擾.
	BoundingBox* GetBBox() const
	{
		return m_pBBox;
	}
	//���f���ɍ��킹���o�E���f�B���O�{�b�N�X�쐬�̃��b�p�[�֐�.
	HRESULT CreateBBoxForMesh(const CStaticMesh& pMesh)
	{
		return m_pBBox->CreateBoxForMesh(pMesh);
	}
	//�o�E���f�B���O�{�b�N�X���I�u�W�F�N�g�̈ʒu�A��]�A�X�P�[���ɍ��킹�čX�V.
	//�I�u�W�F�N�g�̃��[���h�s����l������AABB���X�V����K�v������.
	void UpdateBBox();
	void UpdateBBox(D3DXVECTOR3 MinPos, D3DXVECTOR3 MaxPos);

	////���C�ƃ��b�V���̓����蔻��.
	//bool IsHitForRay(
	//	float* pDistance,			//(out)����.
	//	D3DXVECTOR3* pIntersect);	//(out)�����_.

protected:
	CStaticMesh*		m_pMesh;
	CBoundingSphere*	m_pBSphere;
	BoundingBox*		m_pBBox;
};
