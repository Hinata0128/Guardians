#pragma once
#include "CStaticMesh.h"

/**********************************************
*	スタティックメッシュマネージャークラス.
**/

class CStaticMeshManager
{
public:
	//モデルの種類の列挙型.
	enum CMeshList
	{
		Player,
		Collision,
		Enemy, 

		max,
	};
public:
		~CStaticMeshManager();

		void Create(CDirectX9& pDx9, CDirectX11& pDx11);
		void Init();

		//スタティッククラスのゲットインスタンスを書く
		CStaticMesh* GetMeshInstance(CMeshList MeshNo)
		{
			return m_pMesh[MeshNo];
		};

		//インスタンス取得(唯一のアクセス経路).
		//※関数の前にstaticを付けることでインスタンス生成しなくても使用できる.
		static CStaticMeshManager* GetInstance()
		{
			//唯一のインスタンスを作成する.
			//※staticで作成されたので2回目以降は、下の1行は無視される.
			static CStaticMeshManager s_Instance;	//s_:staticの意味.
			return &s_Instance;
		}


private:
	//外部からコンストラクタへのアクセスを禁止する.
	CStaticMeshManager();
	//コピーコンストラクタによるコピーを禁止する.
	//「=delete」で関数の定義を削除できる.
	CStaticMeshManager(const CStaticMeshManager& rhs) = delete;
	//代入演算子によるコピーを禁止する.
	//operator(オペレータ):演算子のオーバーロードで、演算の中身を拡張できる.
	CStaticMeshManager& operator = (const CStaticMeshManager& rhs) = delete;

	CStaticMesh* m_pMesh[CMeshList::max];

};