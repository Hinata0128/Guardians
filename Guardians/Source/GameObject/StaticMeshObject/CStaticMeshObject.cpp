#include "CStaticMeshObject.h"

CStaticMeshObject::CStaticMeshObject()
	: m_pMesh			( nullptr )
	, m_pBSphere		( nullptr )
	, m_pBBox			( nullptr )
{
	m_pBSphere	= new CBoundingSphere();
	m_pBBox		= new BoundingBox();
}

CStaticMeshObject::~CStaticMeshObject()
{
	SAFE_DELETE( m_pBSphere );
	SAFE_DELETE( m_pBBox );
	DetachMesh();
}

void CStaticMeshObject::Update()
{
	if( m_pMesh == nullptr ){
		return;
	}

	//�I�u�W�F�N�g�̈ʒu�A��]�A�X�P�[�����X�V���ꂽ��BoundingBox���X�V����.
	UpdateBBox();
}

void CStaticMeshObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if( m_pMesh == nullptr ){
		return;
	}

	//�`�撼�O�ō��W���]���Ȃǂ��X�V.
	m_pMesh->SetPosition( m_vPosition );
	m_pMesh->SetRotation( m_vRotation );
	m_pMesh->SetScale( m_vScale );

	//�����_�����O.
	m_pMesh->Render( View, Proj, Light, Camera.vPosition );
}

// �o�E���f�B���O�{�b�N�X���I�u�W�F�N�g�̃��[���h�ϊ��ɍ��킹�čX�V����
void CStaticMeshObject::UpdateBBox()
{
    if (m_pMesh == nullptr || m_pBBox == nullptr) {
        return; // ���b�V���܂��̓o�E���f�B���O�{�b�N�X���L���łȂ��ꍇ�͉������Ȃ�
    }

    // AABB (Axis-Aligned Bounding Box) �́u�����s�v�ȓ��������B
    // ���̂��߁A�I�u�W�F�N�g����]������A�e���ňقȂ�X�P�[�����K�p���ꂽ�肷��ƁA
    // �P�ɃI�u�W�F�N�g�̒��S���ړ������邾���ł͐�����AABB��\���ł��Ȃ��B
    // �����ł́A��胍�o�X�g�ȕ��@�Ƃ��āA�I�u�W�F�N�g��**���[�J����Ԃɂ��錳��AABB**��8�̊p���A
    // �I�u�W�F�N�g��**���[���h�s��ŕϊ�**���A���̕ϊ����ꂽ8�_����**�V�������[���h��Ԃł�AABB**���č\�z����B

    // 1. �I�u�W�F�N�g�̌��݂̃��[���h�s����\�z����
    D3DXMATRIX mWorld, mTran, mRot, mScale, mYaw, mPitch, mRoll;

    // ���s�ړ��s��̍쐬
    D3DXMatrixTranslation(&mTran, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    // ��]�s��̍쐬 (m_vRotation ���g�p���邱�Ƃ�z��)
    // �� �����R�[�h�ł� m_vPosition.y/x/z ���g���Ă������A�ʏ�͊p�x�ł��� m_vRotation ���g��
    D3DXMatrixRotationY(&mYaw, m_vRotation.y);
    D3DXMatrixRotationX(&mPitch, m_vRotation.x);
    D3DXMatrixRotationZ(&mRoll, m_vRotation.z);
    mRot = mYaw * mPitch * mRoll; // Y -> X -> Z �̏��œK�p�i�����_�[�̌v�Z�����ɍ��킹��j

    // �X�P�[���s��̍쐬
    D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);

    // �ŏI�I�ȃ��[���h�s����v�Z: �X�P�[�� -> ��] -> ���s�ړ��̏�����ʓI
    mWorld = mScale * mRot * mTran;

    // 2. ���̃��b�V���̃��[�J����Ԃł�AABB���擾����
    // ���̃��[�J��AABB�́ACreateBBoxForMesh�ň�x�v�Z����Ă���͂������A
    // �����ł̓f�����X�g���[�V�����̂��߂ɍēx���b�V������擾����B
    // �i�� ���ۂ̉^�p�ł́ACBoundingBox�N���X�����̃��[�J��AABB��min/max��ێ����Ă������������I�j
    LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
    void* pVertices = nullptr;
    D3DXVECTOR3 localMin, localMax;

    // ���_�o�b�t�@�̎擾�ƃ��b�N
    if (FAILED(m_pMesh->GetMesh()->GetVertexBuffer(&pVB))) {
        return; // �G���[�A�܂��̓��b�V���������ȏꍇ�͏����𒆒f
    }
    if (FAILED(pVB->Lock(0, 0, &pVertices, 0))) {
        SAFE_RELEASE(pVB); // ���b�N���s���̓��\�[�X�����
        return;
    }
    // ���b�V���̒��_�f�[�^���烍�[�J��AABB���v�Z
    D3DXComputeBoundingBox(
        static_cast<D3DXVECTOR3*>(pVertices),
        m_pMesh->GetMesh()->GetNumVertices(),
        D3DXGetFVFVertexSize(m_pMesh->GetMesh()->GetFVF()),
        &localMin,
        &localMax);
    // ���_�o�b�t�@�̃A�����b�N�Ɖ��
    if (pVB != nullptr) {
        pVB->Unlock();
        SAFE_RELEASE(pVB);
    }

    // 3. ���[�J��AABB��8�̃R�[�i�[�|�C���g�i�p�̍��W�j���v�Z����
    // �����̓_�́A����AABB�̌`����`����
    D3DXVECTOR3 corners[8];
    corners[0] = localMin;
    corners[1] = D3DXVECTOR3(localMax.x, localMin.y, localMin.z);
    corners[2] = D3DXVECTOR3(localMin.x, localMax.y, localMin.z);
    corners[3] = D3DXVECTOR3(localMax.x, localMax.y, localMin.z);
    corners[4] = D3DXVECTOR3(localMin.x, localMin.y, localMax.z);
    corners[5] = D3DXVECTOR3(localMax.x, localMin.y, localMax.z);
    corners[6] = D3DXVECTOR3(localMin.x, localMax.y, localMax.z);
    corners[7] = localMax;

    // 4. �e�R�[�i�[�|�C���g���\�z�������[���h�s��ŕϊ����A�V�������[���h���AABB��min/max�����肷��
    // �����l�Ƃ��āA���ꂼ��\�Ȍ���傫���l�Ə������l���g�p
    D3DXVECTOR3 newMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
    D3DXVECTOR3 newMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; ++i) {
        D3DXVECTOR3 transformedCorner;
        // �e�R�[�i�[�|�C���g�����[���h��Ԃɕϊ�
        D3DXVec3TransformCoord(&transformedCorner, &corners[i], &mWorld);

        // �ϊ���̍��W����A���[���h��Ԃł�AABB�̍ŏ��_�ƍő�_���X�V
        newMin.x = min(newMin.x, transformedCorner.x);
        newMin.y = min(newMin.y, transformedCorner.y);
        newMin.z = min(newMin.z, transformedCorner.z);

        newMax.x = max(newMax.x, transformedCorner.x);
        newMax.y = max(newMax.y, transformedCorner.y);
        newMax.z = max(newMax.z, transformedCorner.z);
    }

    // 5. CBoundingBox�C���X�^���X�ɐV�������[���h���AABB�̍ŏ��_�ƍő�_��ݒ�
    m_pBBox->SetMinPosition(newMin);
    m_pBBox->SetMaxPosition(newMax);
}

void CStaticMeshObject::UpdateBBox(D3DXVECTOR3 MinPos, D3DXVECTOR3 MaxPos)
{
    if (m_pMesh == nullptr || m_pBBox == nullptr) {
        return; // ���b�V���܂��̓o�E���f�B���O�{�b�N�X���L���łȂ��ꍇ�͉������Ȃ�
    }

    // 1. �I�u�W�F�N�g�̌��݂̃��[���h�s����\�z����
    D3DXMATRIX mWorld, mTran, mRot, mScale, mYaw, mPitch, mRoll;

    // ���s�ړ��s��̍쐬
    D3DXMatrixTranslation(&mTran, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    // ��]�s��̍쐬 (m_vRotation ���g�p���邱�Ƃ�z��)
    // �� �����R�[�h�ł� m_vPosition.y/x/z ���g���Ă������A�ʏ�͊p�x�ł��� m_vRotation ���g��
    D3DXMatrixRotationY(&mYaw, m_vRotation.y);
    D3DXMatrixRotationX(&mPitch, m_vRotation.x);
    D3DXMatrixRotationZ(&mRoll, m_vRotation.z);
    mRot = mYaw * mPitch * mRoll; // Y -> X -> Z �̏��œK�p�i�����_�[�̌v�Z�����ɍ��킹��j

    // �X�P�[���s��̍쐬
    D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);

    // �ŏI�I�ȃ��[���h�s����v�Z: �X�P�[�� -> ��] -> ���s�ړ��̏�����ʓI
    mWorld = mScale * mRot * mTran;

    // 3. ���[�J��AABB��8�̃R�[�i�[�|�C���g�i�p�̍��W�j���v�Z����
    // �����̓_�́A����AABB�̌`����`����
    D3DXVECTOR3 corners[8];
    corners[0] = MinPos;
    corners[1] = D3DXVECTOR3(MinPos.x, MinPos.y, MinPos.z);
    corners[2] = D3DXVECTOR3(MinPos.x, MaxPos.y, MinPos.z);
    corners[3] = D3DXVECTOR3(MaxPos.x, MaxPos.y, MinPos.z);
    corners[4] = D3DXVECTOR3(MinPos.x, MinPos.y, MaxPos.z);
    corners[5] = D3DXVECTOR3(MaxPos.x, MinPos.y, MaxPos.z);
    corners[6] = D3DXVECTOR3(MinPos.x, MaxPos.y, MaxPos.z);
    corners[7] = MaxPos;

    // 4. �e�R�[�i�[�|�C���g���\�z�������[���h�s��ŕϊ����A�V�������[���h���AABB��min/max�����肷��
    // �����l�Ƃ��āA���ꂼ��\�Ȍ���傫���l�Ə������l���g�p
    D3DXVECTOR3 newMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
    D3DXVECTOR3 newMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; ++i) {
        D3DXVECTOR3 transformedCorner;
        // �e�R�[�i�[�|�C���g�����[���h��Ԃɕϊ�
        D3DXVec3TransformCoord(&transformedCorner, &corners[i], &mWorld);

        // �ϊ���̍��W����A���[���h��Ԃł�AABB�̍ŏ��_�ƍő�_���X�V
        newMin.x = min(newMin.x, transformedCorner.x);
        newMin.y = min(newMin.y, transformedCorner.y);
        newMin.z = min(newMin.z, transformedCorner.z);

        newMax.x = max(newMax.x, transformedCorner.x);
        newMax.y = max(newMax.y, transformedCorner.y);
        newMax.z = max(newMax.z, transformedCorner.z);
    }

    // 5. CBoundingBox�C���X�^���X�ɐV�������[���h���AABB�̍ŏ��_�ƍő�_��ݒ�
    m_pBBox->SetMinPosition(newMin);
    m_pBBox->SetMaxPosition(newMax);

}

////���C�ƃ��b�V���̓����蔻��.
//bool CStaticMeshObject::IsHitForRay(
//	float* pDistance,
//	D3DXVECTOR3* pIntersect)
//{
//	//D3DXVECTOR3 vAxis;		//���x�N�g��.
//	//D3DXMATRIX	mRotationY;	//Y����]�s��.
//
//	////Y����]�s����쐬.
//	//D3DXMatrixRotationY(&mRotationY, pRay.RotationY);
//	////���x�N�g�������݂̉�]��Ԃɕϊ�����.
//	//D3DXVec3TransformCoord(&vAxis, &pRay.Axis, &mRotationY);
//
//	////���C�̎n�_�ƒ��_.
//	//D3DXVECTOR3 StartPoint, EndPoint;
//	//StartPoint = pRay.Position;	//���C�̎n�_��ݒ�.
//	//EndPoint = pRay.Position + (vAxis * pRay.Lenght);	//���C�̏I�_��ݒ�.
//
//	////���C�𓖂Ă������b�V�����ړ����Ă���ꍇ�ł�.
//	////�Ώۂ�world�s��̋t�s���p����ΐ��������C��������.
//	//D3DXMATRIX mWorld, mInverseWoreld;
//	//D3DXMATRIX mTran;
//	////�ړ�����.
//	//D3DXMatrixTranslation(
//	//	&mWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);
//
//	////��]����.
//	////�����̍s��v�Z�́uCStaticMesh::Raender()�֐��v�Ɠ����ɂ���K�v����.
//	//D3DXMATRIX mRot, mYaw, mPitch, mRoll;
//	////Y����]�s����쐬
//	//D3DXMatrixRotationY(&mYaw, m_vPosition.y);
//	////X����]�s����쐬
//	//D3DXMatrixRotationX(&mPitch, m_vPosition.x);
//	////Z����]�s����쐬
//	//D3DXMatrixRotationZ(&mRoll, m_vPosition.z);
//	////��]�s����쐬.
//	//mRot = mYaw * mPitch * mRoll;
//
//	////�g�k����.
//	//D3DXMATRIX mScale;
//	//D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);
//
//	////���[���h�s��v�Z.
//	////�g�kx��]x�ړ��@�����Ԃ��ƂĂ����!!
//	//mWorld = mScale * mRot * mTran;
//
//	//return false;
//}
