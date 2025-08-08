#include "CPlayer.h"
#include "Sound/CSoundManager.h"
#include "Collision/BoundingBox/BoundingBox.h"
#include "StaticMash/CStaticMeshManager.h"

CPlayer::CPlayer()
	: m_TurnSpeed	(0.1f)	//きっちりやりたい場合はラジアン値を設定すること.
	, m_MoveSpeed	(0.1f)
	, m_MoveState	( enMoveState::stop )
{
	AttachMesh(*CStaticMeshManager::GetInstance()->GetMeshInstance(CStaticMeshManager::CMeshList::Player));
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
#if 1
	//前進.
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_MoveState = enMoveState::Forward;
	}
	//後退.
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_MoveState = enMoveState::Backward;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_vRotation.y += m_TurnSpeed;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_vRotation.y -= m_TurnSpeed;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_vPosition.y -= 0.1f;
	}if (GetAsyncKeyState('W') & 0x8000)
	{
		m_vPosition.y += 0.1f;
	}

	//プレイヤーの動作(前後の進コード).
	RadioControl();
#else
	float add_value = 0.1f;
	if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
		m_vPosition.y += add_value;
	}
	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		m_vPosition.y -= add_value;
	}
	if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		m_vPosition.x += add_value;
	}
	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		m_vPosition.x -= add_value;
	}
#endif

	//前回のフレームで弾を飛ばしているかも知れないのでfalseにする
	m_Shot = false;

	//弾を飛ばしたい!
	if (GetAsyncKeyState('Z') & 0x8000) {
		m_Shot = true;
	}

	CCharacter::Update();
}

void CPlayer::Draw(
	D3DXMATRIX& View, D3DXMATRIX& Proj, LIGHT& Light, CAMERA& Camera)
{
	CCharacter::Draw( View, Proj, Light, Camera );
}

//ラジコン操作.
void CPlayer::RadioControl()
{
	//Z軸ベクトル(Z+方向への単位ベクトル).
	//※大きさ(長さ) が1のベクトルを単位ベクトルという.
	D3DXVECTOR3 vecAxiszZ(0.f, 0.f, 1.f);

	//Y方向の回転行列.
	D3DXMATRIX mRotationY;
	//Y軸回転行列を作成.
	D3DXMatrixRotationY(
		&mRotationY,		//(out)行列.
		m_vRotation.y);		//プレイヤーのY軸方向の回転軸.

	//Y軸回転行列を使ってZ軸ベクトルを座標返還する.
	D3DXVec3TransformCoord(
		&vecAxiszZ,			//(out)Z軸ベクトル.
		&vecAxiszZ,			//(in)Z軸ベクトル.
		&mRotationY);		//Y軸回転行列.

	//移動状態によって処理を分ける.
	switch (m_MoveState)
	{
	case CPlayer::stop:
		break;
	case CPlayer::Forward:
		//前進.
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			m_vPosition += vecAxiszZ * m_MoveSpeed;
		}
		break;
	case CPlayer::Backward:
		//後退.
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			m_vPosition -= vecAxiszZ * m_MoveSpeed;
		}
		break;
	case CPlayer::TurnLeft:
		break;
	case CPlayer::TurnRight:
		break;
	default:
		break;
	}
	//上記の移動処理は終われば停止状態にしておく.
	m_MoveState = enMoveState::stop;
}

void CPlayer::HandleGroundCollision(CStaticMeshObject* pGroundObject)
{
	// プレイヤーと地面のBoundingBoxを取得
	BoundingBox* playerBBox = this->GetBBox();
	BoundingBox* groundBBox = pGroundObject->GetBBox();

	// 両方のBoundingBoxが有効であることを確認
	if (playerBBox == nullptr || groundBBox == nullptr) {
		return; // 無効なBoundingBoxがあれば処理を中断
	}

	// プレイヤーのAABBの底が、地面のAABBのトップよりも下にある場合
	if (playerBBox->GetMinPosition().y < groundBBox->GetMaxPosition().y) {
		// プレイヤーのAABBの高さの半分を計算します。
		float playerHalfHeight = (playerBBox->GetMaxPosition().y - playerBBox->GetMinPosition().y) / 2.0f;

		// プレイヤーの中心Y座標を、地面の最上部にプレイヤーの底が接するように調整します。
		this->SetPosition(m_vPosition.x, groundBBox->GetMaxPosition().y + playerHalfHeight, m_vPosition.z);

		// ★★★ 重力や落下速度を管理している場合のみ、以下のコードを追加 ★★★
		// 例: プレイヤーが落下している場合、地面に衝突したら垂直方向の速度をゼロにリセットします。
		// if (m_vVelocity.y < 0) { // 下方向への速度がある場合のみ
		//     m_vVelocity.y = 0.0f; // Y方向の速度をゼロにする
		// }
		// その他のフラグ（例: isGrounded = true;）を設定することもできます。
	}
}