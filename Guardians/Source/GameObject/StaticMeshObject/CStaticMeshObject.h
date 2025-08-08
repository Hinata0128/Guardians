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

	//CGameObjectで純粋仮想関数の宣言がされてるのでこちらで定義を書く.
	virtual void Update() override;
	virtual void Draw( D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera ) override;

	//メッシュを接続する.
	void AttachMesh( CStaticMesh& pMesh ){
		m_pMesh = &pMesh;
	}
	//メッシュを切り離す.
	void DetachMesh(){
		m_pMesh = nullptr;
	}
	//バウンディングスフィア取得
	CBoundingSphere* GetBSphere() const {
		return m_pBSphere;
	}
	//モデルに合わせたバウンディングスフィア作成のラッパー関数
	HRESULT CreateBSphereForMesh(const CStaticMesh& pMesh) {
		return m_pBSphere->CreateSphereForMesh(pMesh);
	}
	//バウンディングスフィアをオブジェクト位置に合わせる
	//※モデルの原点が中心の場合を想定
	void UpdateBSpherePos() {
		m_pBSphere->SetPosition(m_vPosition);
	}

	//バウンディングボックス取得.
	BoundingBox* GetBBox() const
	{
		return m_pBBox;
	}
	//モデルに合わせたバウンディングボックス作成のラッパー関数.
	HRESULT CreateBBoxForMesh(const CStaticMesh& pMesh)
	{
		return m_pBBox->CreateBoxForMesh(pMesh);
	}
	//バウンディングボックスをオブジェクトの位置、回転、スケールに合わせて更新.
	//オブジェクトのワールド行列を考慮してAABBを更新する必要がある.
	void UpdateBBox();
	void UpdateBBox(D3DXVECTOR3 MinPos, D3DXVECTOR3 MaxPos);

	////レイとメッシュの当たり判定.
	//bool IsHitForRay(
	//	float* pDistance,			//(out)距離.
	//	D3DXVECTOR3* pIntersect);	//(out)交差点.

protected:
	CStaticMesh*		m_pMesh;
	CBoundingSphere*	m_pBSphere;
	BoundingBox*		m_pBBox;
};
