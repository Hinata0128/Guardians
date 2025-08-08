#include "CStaticMeshManager.h"
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning(disable:4005)

CStaticMeshManager::CStaticMeshManager()
	: m_pMesh		()
{

}

CStaticMeshManager::~CStaticMeshManager()
{
}

void CStaticMeshManager::Create(CDirectX9& pDx9, CDirectX11& pDx11)
{
	for (int i = 0; i < CMeshList::max; ++i)
	{
		m_pMesh[i] = new CStaticMesh();
	}
	struct MeshList
	{
		int listNo;				//CMeshList列挙型を設定.
		const TCHAR path[256];	//ファイルの名前(パス付き).
	};
	MeshList MList[] =
	{
		{CMeshList::Player, _T("Data\\Mesh\\Static\\Fighter\\Fighter.x")},
		{CMeshList::Collision, 	_T("Data\\Collision\\Sphere.x")},
		{CMeshList::Collision, 	_T("Data\\Mesh\\Static\\Enemy\\block.x")},
	};
	int list_max = sizeof(MList) / sizeof(MList[0]);
	for (int i = 0; i < list_max; ++i)
	{
		if (FAILED(m_pMesh[i]->Init(
			pDx9,
			pDx11,
			MList[i].path))) {

			return;
		};
	}
}

void CStaticMeshManager::Init()
{
}