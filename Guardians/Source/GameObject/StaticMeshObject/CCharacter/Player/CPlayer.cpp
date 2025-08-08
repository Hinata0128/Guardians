#include "CPlayer.h"
#include "Sound/CSoundManager.h"
#include "Collision/BoundingBox/BoundingBox.h"
#include "StaticMash/CStaticMeshManager.h"
#include <cmath> // atan2fのために必要

CPlayer::CPlayer()
    : m_MoveSpeed(0.1f)
    , m_TurnSpeed(D3DXToRadian(5.0f)) // 5度/フレームの回転速度
{
    AttachMesh(*CStaticMeshManager::GetInstance()->GetMeshInstance(CStaticMeshManager::CMeshList::Player));
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{
    D3DXVECTOR3 moveDirection(0.0f, 0.0f, 0.0f);

    // 前方向 (Z軸プラス方向)
    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        moveDirection.z += 1.0f;
    }
    // 後ろ方向 (Z軸マイナス方向)
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        moveDirection.z -= 1.0f;
    }
    // 右方向 (X軸プラス方向)
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        moveDirection.x += 1.0f;
    }
    // 左方向 (X軸マイナス方向)
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        moveDirection.x -= 1.0f;
    }

    // 移動方向ベクトルを正規化して、斜め移動でも速度が一定になるようにする
    // ただし、入力がない場合は正規化しない (ゼロベクトルを正規化するとエラーになるため)
    if (D3DXVec3LengthSq(&moveDirection) > 0) // ベクトルの長さの2乗が0より大きい場合
    {
        D3DXVec3Normalize(&moveDirection, &moveDirection);

        // プレイヤーの位置を更新
        m_vPosition.x += moveDirection.x * m_MoveSpeed;
        m_vPosition.z += moveDirection.z * m_MoveSpeed;

        // プレイヤーの向きを移動方向に合わせる
        // atan2f(Y方向成分, X方向成分) でY軸周りの回転角度を求める
        float targetRotationY = atan2f(moveDirection.x, moveDirection.z);

        // 現在の回転と目標回転の間を滑らかに補間
        // 角度は-PIからPIの範囲で正規化する必要があるかもしれません
        // 簡単な補間として、差分が小さい方を選ぶ
        float diff = targetRotationY - m_vRotation.y;
        if (diff > D3DX_PI) diff -= 2 * D3DX_PI;
        if (diff < -D3DX_PI) diff += 2 * D3DX_PI;

        m_vRotation.y += diff * m_TurnSpeed; // m_TurnSpeedは補間係数として使う

        // 角度を-PIからPIの範囲に正規化
        m_vRotation.y = fmodf(m_vRotation.y, 2 * D3DX_PI);
        if (m_vRotation.y > D3DX_PI) m_vRotation.y -= 2 * D3DX_PI;
        if (m_vRotation.y < -D3DX_PI) m_vRotation.y += 2 * D3DX_PI;
    }


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
    CCharacter::Draw(View, Proj, Light, Camera);
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
