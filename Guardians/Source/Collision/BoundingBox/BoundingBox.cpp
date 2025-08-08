#include "BoundingBox.h"
#include "StaticMash/CStaticMesh.h"
#include <string>        // std::to_string のために必要
#include <algorithm>     // std::min, std::max のために必要
#include <d3dx9.h>       // D3DXComputeBoundingBox, D3DXGetFVFVertexSize などのために必要
#include <windows.h>     // OutputDebugStringA のために必要
#include <cfloat>        // FLT_MAX のために必要 (ヘッダーにもあったが、念のため)

// SAFE_RELEASE マクロの定義 (もしグローバルに定義されていなければここに書く)
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#endif

// MAX_FVF_DECL_SIZE の定義 (d3d9types.hなどに定義されているはず)
#ifndef MAX_FVF_DECL_SIZE
#define MAX_FVF_DECL_SIZE (27) // MAXD3DDECLLENGTH + 1
#endif


BoundingBox::BoundingBox()
    : m_MinPosition(FLT_MAX, FLT_MAX, FLT_MAX)
    , m_MaxPosition(-FLT_MAX, -FLT_MAX, -FLT_MAX)
    , m_OriginalLocalMin(FLT_MAX, FLT_MAX, FLT_MAX) // 初期化
    , m_OriginalLocalMax(-FLT_MAX, -FLT_MAX, -FLT_MAX) // 初期化
{
}

BoundingBox::~BoundingBox()
{
}

HRESULT BoundingBox::CreateBoxForMesh(const CStaticMesh& rMesh)
{
    ID3DXMesh* d3dxMesh = rMesh.GetMeshForRay(); // GetMeshForRay を使用
    if (d3dxMesh == nullptr) {
        OutputDebugStringA("BoundingBox::CreateBoxForMesh: d3dxMesh (from GetMeshForRay) is nullptr! Check CStaticMesh loading or GetMeshForRay.\n");
        return E_FAIL;
    }

    LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
    void* pVertices = nullptr;
    HRESULT hr = S_OK;
    D3DXVECTOR3 tempMin, tempMax; // D3DXComputeBoundingBox の出力用一時変数

    hr = d3dxMesh->GetVertexBuffer(&pVB);
    if (FAILED(hr)) {
        OutputDebugStringA("BoundingBox::CreateBoxForMesh: Failed to get vertex buffer!\n");
        return hr;
    }

    hr = pVB->Lock(0, 0, &pVertices, 0);
    if (FAILED(hr)) {
        OutputDebugStringA("BoundingBox::CreateBoxForMesh: Failed to lock vertex buffer!\n");
        SAFE_RELEASE(pVB);
        return hr;
    }

    DWORD fvf = d3dxMesh->GetFVF();
    DWORD vertexStride = D3DXGetFVFVertexSize(fvf);

    if (vertexStride == 0) {
        D3DVERTEXELEMENT9 elements[MAX_FVF_DECL_SIZE];
        if (FAILED(d3dxMesh->GetDeclaration(elements))) {
            OutputDebugStringA("BoundingBox::CreateBoxForMesh: Failed to get vertex declaration!\n");
            pVB->Unlock();
            SAFE_RELEASE(pVB);
            return E_FAIL;
        }
        vertexStride = D3DXGetDeclVertexSize(elements, 0);
    }

    if (vertexStride == 0) {
        OutputDebugStringA("BoundingBox::CreateBoxForMesh: Could not determine vertex stride! Check FVF or vertex declaration.\n");
        pVB->Unlock();
        SAFE_RELEASE(pVB);
        return E_FAIL;
    }

    D3DXComputeBoundingBox(
        static_cast<const D3DXVECTOR3*>(pVertices),
        d3dxMesh->GetNumVertices(),
        vertexStride,
        &tempMin, // 一時変数に計算
        &tempMax
    );

    pVB->Unlock();
    SAFE_RELEASE(pVB);

    // 計算されたMin/Maxを、現在のAABBとオリジナルのローカルAABBの両方に設定
    m_MinPosition = tempMin;
    m_MaxPosition = tempMax;
    m_OriginalLocalMin = tempMin; // ここが重要
    m_OriginalLocalMax = tempMax; // ここが重要

    // ボックスのY軸方向の最大値を2.0fに設定
    // 現在のmin.yからの相対的な高さとして2.0fを指定
    const float desiredHeight = 0.0f;
    m_MaxPosition.y = m_MinPosition.y + desiredHeight;
    m_OriginalLocalMax.y = m_OriginalLocalMin.y + desiredHeight; // オリジナルも更新


    OutputDebugStringA("BoundingBox::CreateBoxForMesh - Calculated Local BBox:\n");
    OutputDebugStringA(("  Min: (" + std::to_string(m_MinPosition.x) + ", " + std::to_string(m_MinPosition.y) + ", " + std::to_string(m_MinPosition.z) + ")\n").c_str());
    OutputDebugStringA(("  Max: (" + std::to_string(m_MaxPosition.x) + ", " + std::to_string(m_MaxPosition.y) + ", " + std::to_string(m_MaxPosition.z) + ")\n").c_str());

    return S_OK;
}

bool BoundingBox::IsHit(const BoundingBox& otherBBox) const
{
    // X軸での重なりがないかチェック
    if (m_MaxPosition.x < otherBBox.m_MinPosition.x || m_MinPosition.x > otherBBox.m_MaxPosition.x)
    {
        return false;
    }
    // Y軸での重なりがないかチェック
    if (m_MaxPosition.y < otherBBox.m_MinPosition.y || m_MinPosition.y > otherBBox.m_MaxPosition.y)
    {
        return false;
    }
    // Z軸での重なりがないかチェック
    if (m_MaxPosition.z < otherBBox.m_MinPosition.z || m_MinPosition.z > otherBBox.m_MaxPosition.z)
    {
        return false;
    }

    // 全ての軸で重なっていれば衝突
    return true;
}

D3DXVECTOR3 BoundingBox::GetCenter() const
{
    return (m_MinPosition + m_MaxPosition) * 0.5f;
}

D3DXVECTOR3 BoundingBox::GetSize() const
{
    return m_MaxPosition - m_MinPosition;
}

void BoundingBox::SetOriginalLocalMinMax(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos)
{
    m_OriginalLocalMin = minPos;
    m_OriginalLocalMax = maxPos;
}