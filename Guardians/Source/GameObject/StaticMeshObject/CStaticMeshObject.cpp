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

	//オブジェクトの位置、回転、スケールが更新されたらBoundingBoxも更新する.
	UpdateBBox();
}

void CStaticMeshObject::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	if( m_pMesh == nullptr ){
		return;
	}

	//描画直前で座標や回転情報などを更新.
	m_pMesh->SetPosition( m_vPosition );
	m_pMesh->SetRotation( m_vRotation );
	m_pMesh->SetScale( m_vScale );

	//レンダリング.
	m_pMesh->Render( View, Proj, Light, Camera.vPosition );
}

// バウンディングボックスをオブジェクトのワールド変換に合わせて更新する
void CStaticMeshObject::UpdateBBox()
{
    if (m_pMesh == nullptr || m_pBBox == nullptr) {
        return; // メッシュまたはバウンディングボックスが有効でない場合は何もしない
    }

    // AABB (Axis-Aligned Bounding Box) は「軸並行」な特性を持つ。
    // そのため、オブジェクトが回転したり、各軸で異なるスケールが適用されたりすると、
    // 単にオブジェクトの中心を移動させるだけでは正しいAABBを表現できない。
    // ここでは、よりロバストな方法として、オブジェクトの**ローカル空間にある元のAABB**の8つの角を、
    // オブジェクトの**ワールド行列で変換**し、その変換された8点から**新しいワールド空間でのAABB**を再構築する。

    // 1. オブジェクトの現在のワールド行列を構築する
    D3DXMATRIX mWorld, mTran, mRot, mScale, mYaw, mPitch, mRoll;

    // 平行移動行列の作成
    D3DXMatrixTranslation(&mTran, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    // 回転行列の作成 (m_vRotation を使用することを想定)
    // ※ 既存コードでは m_vPosition.y/x/z が使われていたが、通常は角度である m_vRotation を使う
    D3DXMatrixRotationY(&mYaw, m_vRotation.y);
    D3DXMatrixRotationX(&mPitch, m_vRotation.x);
    D3DXMatrixRotationZ(&mRoll, m_vRotation.z);
    mRot = mYaw * mPitch * mRoll; // Y -> X -> Z の順で適用（レンダーの計算順序に合わせる）

    // スケール行列の作成
    D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);

    // 最終的なワールド行列を計算: スケール -> 回転 -> 平行移動の順が一般的
    mWorld = mScale * mRot * mTran;

    // 2. 元のメッシュのローカル空間でのAABBを取得する
    // このローカルAABBは、CreateBBoxForMeshで一度計算されているはずだが、
    // ここではデモンストレーションのために再度メッシュから取得する。
    // （※ 実際の運用では、CBoundingBoxクラスが元のローカルAABBのmin/maxを保持しておく方が効率的）
    LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
    void* pVertices = nullptr;
    D3DXVECTOR3 localMin, localMax;

    // 頂点バッファの取得とロック
    if (FAILED(m_pMesh->GetMesh()->GetVertexBuffer(&pVB))) {
        return; // エラー、またはメッシュが無効な場合は処理を中断
    }
    if (FAILED(pVB->Lock(0, 0, &pVertices, 0))) {
        SAFE_RELEASE(pVB); // ロック失敗時はリソースを解放
        return;
    }
    // メッシュの頂点データからローカルAABBを計算
    D3DXComputeBoundingBox(
        static_cast<D3DXVECTOR3*>(pVertices),
        m_pMesh->GetMesh()->GetNumVertices(),
        D3DXGetFVFVertexSize(m_pMesh->GetMesh()->GetFVF()),
        &localMin,
        &localMax);
    // 頂点バッファのアンロックと解放
    if (pVB != nullptr) {
        pVB->Unlock();
        SAFE_RELEASE(pVB);
    }

    // 3. ローカルAABBの8つのコーナーポイント（角の座標）を計算する
    // これらの点は、元のAABBの形状を定義する
    D3DXVECTOR3 corners[8];
    corners[0] = localMin;
    corners[1] = D3DXVECTOR3(localMax.x, localMin.y, localMin.z);
    corners[2] = D3DXVECTOR3(localMin.x, localMax.y, localMin.z);
    corners[3] = D3DXVECTOR3(localMax.x, localMax.y, localMin.z);
    corners[4] = D3DXVECTOR3(localMin.x, localMin.y, localMax.z);
    corners[5] = D3DXVECTOR3(localMax.x, localMin.y, localMax.z);
    corners[6] = D3DXVECTOR3(localMin.x, localMax.y, localMax.z);
    corners[7] = localMax;

    // 4. 各コーナーポイントを構築したワールド行列で変換し、新しいワールド空間AABBのmin/maxを決定する
    // 初期値として、それぞれ可能な限り大きい値と小さい値を使用
    D3DXVECTOR3 newMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
    D3DXVECTOR3 newMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; ++i) {
        D3DXVECTOR3 transformedCorner;
        // 各コーナーポイントをワールド空間に変換
        D3DXVec3TransformCoord(&transformedCorner, &corners[i], &mWorld);

        // 変換後の座標から、ワールド空間でのAABBの最小点と最大点を更新
        newMin.x = min(newMin.x, transformedCorner.x);
        newMin.y = min(newMin.y, transformedCorner.y);
        newMin.z = min(newMin.z, transformedCorner.z);

        newMax.x = max(newMax.x, transformedCorner.x);
        newMax.y = max(newMax.y, transformedCorner.y);
        newMax.z = max(newMax.z, transformedCorner.z);
    }

    // 5. CBoundingBoxインスタンスに新しいワールド空間AABBの最小点と最大点を設定
    m_pBBox->SetMinPosition(newMin);
    m_pBBox->SetMaxPosition(newMax);
}

void CStaticMeshObject::UpdateBBox(D3DXVECTOR3 MinPos, D3DXVECTOR3 MaxPos)
{
    if (m_pMesh == nullptr || m_pBBox == nullptr) {
        return; // メッシュまたはバウンディングボックスが有効でない場合は何もしない
    }

    // 1. オブジェクトの現在のワールド行列を構築する
    D3DXMATRIX mWorld, mTran, mRot, mScale, mYaw, mPitch, mRoll;

    // 平行移動行列の作成
    D3DXMatrixTranslation(&mTran, m_vPosition.x, m_vPosition.y, m_vPosition.z);

    // 回転行列の作成 (m_vRotation を使用することを想定)
    // ※ 既存コードでは m_vPosition.y/x/z が使われていたが、通常は角度である m_vRotation を使う
    D3DXMatrixRotationY(&mYaw, m_vRotation.y);
    D3DXMatrixRotationX(&mPitch, m_vRotation.x);
    D3DXMatrixRotationZ(&mRoll, m_vRotation.z);
    mRot = mYaw * mPitch * mRoll; // Y -> X -> Z の順で適用（レンダーの計算順序に合わせる）

    // スケール行列の作成
    D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);

    // 最終的なワールド行列を計算: スケール -> 回転 -> 平行移動の順が一般的
    mWorld = mScale * mRot * mTran;

    // 3. ローカルAABBの8つのコーナーポイント（角の座標）を計算する
    // これらの点は、元のAABBの形状を定義する
    D3DXVECTOR3 corners[8];
    corners[0] = MinPos;
    corners[1] = D3DXVECTOR3(MinPos.x, MinPos.y, MinPos.z);
    corners[2] = D3DXVECTOR3(MinPos.x, MaxPos.y, MinPos.z);
    corners[3] = D3DXVECTOR3(MaxPos.x, MaxPos.y, MinPos.z);
    corners[4] = D3DXVECTOR3(MinPos.x, MinPos.y, MaxPos.z);
    corners[5] = D3DXVECTOR3(MaxPos.x, MinPos.y, MaxPos.z);
    corners[6] = D3DXVECTOR3(MinPos.x, MaxPos.y, MaxPos.z);
    corners[7] = MaxPos;

    // 4. 各コーナーポイントを構築したワールド行列で変換し、新しいワールド空間AABBのmin/maxを決定する
    // 初期値として、それぞれ可能な限り大きい値と小さい値を使用
    D3DXVECTOR3 newMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
    D3DXVECTOR3 newMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 8; ++i) {
        D3DXVECTOR3 transformedCorner;
        // 各コーナーポイントをワールド空間に変換
        D3DXVec3TransformCoord(&transformedCorner, &corners[i], &mWorld);

        // 変換後の座標から、ワールド空間でのAABBの最小点と最大点を更新
        newMin.x = min(newMin.x, transformedCorner.x);
        newMin.y = min(newMin.y, transformedCorner.y);
        newMin.z = min(newMin.z, transformedCorner.z);

        newMax.x = max(newMax.x, transformedCorner.x);
        newMax.y = max(newMax.y, transformedCorner.y);
        newMax.z = max(newMax.z, transformedCorner.z);
    }

    // 5. CBoundingBoxインスタンスに新しいワールド空間AABBの最小点と最大点を設定
    m_pBBox->SetMinPosition(newMin);
    m_pBBox->SetMaxPosition(newMax);

}

////レイとメッシュの当たり判定.
//bool CStaticMeshObject::IsHitForRay(
//	float* pDistance,
//	D3DXVECTOR3* pIntersect)
//{
//	//D3DXVECTOR3 vAxis;		//軸ベクトル.
//	//D3DXMATRIX	mRotationY;	//Y軸回転行列.
//
//	////Y軸回転行列を作成.
//	//D3DXMatrixRotationY(&mRotationY, pRay.RotationY);
//	////軸ベクトルを現在の回転状態に変換する.
//	//D3DXVec3TransformCoord(&vAxis, &pRay.Axis, &mRotationY);
//
//	////レイの始点と頂点.
//	//D3DXVECTOR3 StartPoint, EndPoint;
//	//StartPoint = pRay.Position;	//レイの始点を設定.
//	//EndPoint = pRay.Position + (vAxis * pRay.Lenght);	//レイの終点を設定.
//
//	////レイを当てたいメッシュが移動している場合でも.
//	////対象のworld行列の逆行列を用いれば正しくレイが当たる.
//	//D3DXMATRIX mWorld, mInverseWoreld;
//	//D3DXMATRIX mTran;
//	////移動処理.
//	//D3DXMatrixTranslation(
//	//	&mWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);
//
//	////回転処理.
//	////※この行列計算は「CStaticMesh::Raender()関数」と同じにする必要あり.
//	//D3DXMATRIX mRot, mYaw, mPitch, mRoll;
//	////Y軸回転行列を作成
//	//D3DXMatrixRotationY(&mYaw, m_vPosition.y);
//	////X軸回転行列を作成
//	//D3DXMatrixRotationX(&mPitch, m_vPosition.x);
//	////Z軸回転行列を作成
//	//D3DXMatrixRotationZ(&mRoll, m_vPosition.z);
//	////回転行列を作成.
//	//mRot = mYaw * mPitch * mRoll;
//
//	////拡縮処理.
//	//D3DXMATRIX mScale;
//	//D3DXMatrixScaling(&mScale, m_vScale.x, m_vScale.y, m_vScale.z);
//
//	////ワールド行列計算.
//	////拡縮x回転x移動　※順番がとても大切!!
//	//mWorld = mScale * mRot * mTran;
//
//	//return false;
//}
