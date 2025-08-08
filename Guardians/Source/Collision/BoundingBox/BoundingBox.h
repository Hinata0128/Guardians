#pragma once

#include "StaticMash/CStaticMesh.h"
#include <d3dx9.h>       // D3DXVECTOR3 の定義が必要
#include <cfloat>        // FLT_MAX, -FLT_MAX のために必要

/**************************************************
*	バウンディングボックス.
**/

class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();

	//モデルに合わせたバウンディングボックスを作成.
	//メッシュの外接AABBを計算する.
	HRESULT CreateBoxForMesh(const CStaticMesh& pMesh);

	//他のボックスとの当たり判定(AABB vs AABB).
	bool IsHit(const BoundingBox& pBBox) const;

	//現在のAABBの最小座標を取得する.
	const D3DXVECTOR3& GetMinPosition() const { return m_MinPosition; }
	//現在のAABBの最大座標を取得する.
	const D3DXVECTOR3& GetMaxPosition() const { return m_MaxPosition; }

	//現在のAABBの最小座標を設定する.
	void SetMinPosition(const D3DXVECTOR3& MinPos) { m_MinPosition = MinPos; }
	//現在のAABBの最大座標を設定する.
	void SetMaxPosition(const D3DXVECTOR3& MaxPos) { m_MaxPosition = MaxPos; }

	// BoundingBoxの中心座標を取得
	D3DXVECTOR3 GetCenter() const;

	// BoundingBoxのサイズ（幅、高さ、奥行き）を取得するメソッド
	D3DXVECTOR3 GetSize() const;

	// オリジナルのローカルAABBの最小・最大座標を保存/取得する
	void SetOriginalLocalMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos);
	D3DXVECTOR3 GetOriginalLocalMin() const { return m_OriginalLocalMin; }
	D3DXVECTOR3 GetOriginalLocalMax() const { return m_OriginalLocalMax; }

private:
	D3DXVECTOR3 m_MinPosition;	// ワールド空間、または変換後のAABBの最小座標.
	D3DXVECTOR3 m_MaxPosition;	// ワールド空間、または変換後のAABBの最大座標.
	D3DXVECTOR3 m_OriginalLocalMin; // CreateBoxForMeshで計算されたローカルAABBの最小座標.
	D3DXVECTOR3 m_OriginalLocalMax; // CreateBoxForMeshで計算されたローカルAABBの最大座標.
};