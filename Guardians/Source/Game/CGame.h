#pragma once
#include "DirectX/CDirectX9.h"
#include "DirectX/CDirectX11.h"
#include "Sprite3D/CSprite3D.h"
#include "Sprite2D/CSprite2D.h"
#include "SceneManager/CSceneManager.h"


//標準テンプレートライブラリー.
//可変長配列.
#include <vector>


/********************************************************************************
*	ゲームクラス.
**/
class CGame
{
public:
	static constexpr int ENEMY_MAX = 3;	//エネミーの最大数

	CGame(CDirectX9& pDx9, CDirectX11& pDx11, HWND hWnd);
	~CGame();

	void Create();
	HRESULT LoadData();
	void Release();

	void Update();
	void Draw();

private:
	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Projection();

	//三人称カメラ.
	void ThirdPersonCamera(
		CAMERA* pCamera, const D3DXVECTOR3& TagetPos, float TagetRotY);


private:
	CDirectX9* m_pDx9;
	CDirectX11* m_pDx11;


	//ウィンドウハンドル.
	HWND			m_hWnd;

	//カメラ情報.
	CAMERA			m_Camera;
	//ライト情報
	LIGHT			m_Light;

	//行列.
	D3DXMATRIX		m_mView;	//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;	//射影（プロジェクション）行列.

	//ゲームで扱うスプライトデータ(使いまわす資源).
	CSprite3D* m_pSpriteGround;
	CSprite3D* m_pSpritePlayer;
	CSprite3D* m_pSpriteExplosion;

	//スプライト2Dデータ(使いまわす資源)

private:
	//=delete「削除定義」と呼ばれる機能.
	//指定された場合、その関数は呼び出せなくなる.
	CGame() = delete;	//デフォルトコンストラクタ禁止.
	CGame(const CGame&) = delete;
	CGame& operator = (const CGame& rhs) = delete;
};

// ここから下はCGame.cppの内容なので、CGame.hには含めないでください。
// おそらくCGame.hファイルの下にCGame.cppの内容が誤ってコピーされているため、
// コンパイルエラーが発生しています。
// CGame.hファイルは、クラスの宣言とメンバ変数の定義のみを含み、
// 関数の実装はCGame.cppに記述してください。
// CGame.h はここで閉じます。