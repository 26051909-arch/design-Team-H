#include "Trader.h"

// 他のファイル（Field.cppなど）で定義されているステージ番号を参照
extern int g_currentStage;

// ==========================================
// アニメーションデータ定義
// ==========================================
static TexAnim _idleMae[] = {
    {0, 12},
    {1, 12},
};

static TexAnim _idleUshiro[] = {
    {4, 12},
    {5, 12},
};

static TexAnim _idleYoko[] = {
    {8, 12},
    {9, 12},
};

TexAnimData Trader::_anim_data[] = {
    ANIMDATA(_idleMae),
    ANIMDATA(_idleUshiro),
    ANIMDATA(_idleYoko),
};

// ==========================================
// コンストラクタ
// ==========================================
Trader::Trader(const CVector2D& pos) : Base(eType_Trader)
{
    m_img = COPY_RESOURCE("Trader", CImage);
    m_img.ChangeAnimation(eAnimIdleMae, true);

    // 初期座標の設定
    m_pos_old = m_pos = pos;

    // 表示サイズ・中心位置の設定
    m_img.SetSize(128, 128);
    m_img.SetCenter(64, 123);

    // 当たり判定用矩形の設定
    m_rect = CRect(-32, -108, 32, 0);

    // フラグ初期化
    m_is_near = false;
    m_is_shop_open = false;
}

// ==========================================
// 更新処理
// ==========================================
void Trader::Update()
{
    // ★ ステージ5以外になったら即座に自滅（削除）する！
    if (g_currentStage != 5) {
        m_is_near = false;
        m_is_shop_open = false;
        SetKill(); // エンジン側に自身を削除させる命令
        return;
    }

    m_img.UpdateAnimation();
    m_is_near = false;
}

// ==========================================
// 描画処理
// ==========================================
void Trader::Draw()
{
    // ★ ステージ5以外なら描画しない
    if (g_currentStage != 5) return;

    m_img.SetPos(m_pos);
    m_img.Draw();

     DrawRect(); // デバッグ用の枠線は非表示
}

// ==========================================
// 当たり判定処理
// ==========================================
void Trader::Collision(Base* b)
{
    // ★ ステージ5以外なら判定を即遮断（判定残り防止）
    if (g_currentStage != 5) {
        m_is_near = false;
        return;
    }

    if (b->GetType() == eType_Player) {
        // プレイヤーと重なっているかチェック
        if (Base::CollisionRect(this, b)) {
            m_is_near = true;

            // 接近中にZキー（eButton1）が押されたらショップ画面の開閉をトグル
            if (PUSH(CInput::eButton1)) {
                m_is_shop_open = !m_is_shop_open;
            }
        }
    }
}