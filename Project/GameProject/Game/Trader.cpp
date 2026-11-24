#include"Trader.h"


Trader::Trader(const CVector2D& pos) : Base(eType_Trader)
{
    m_img = COPY_RESOURCE("Trader", CImage);
    m_img.ChangeAnimation(eAnimIdleMae, true);

    //座標
    m_pos_old = m_pos = pos;
    //サイズ設定
    m_img.SetSize(128, 128);
    //中心位置設定
    m_img.SetCenter(64, 123);
    m_rect = CRect(-32, -108, 32, 0);

    //フラグの初期化
    m_is_near = false;
    m_is_shop_open = false;
}



void Trader::Update()
{
    m_img.UpdateAnimation();

    m_is_near = false;

}



void Trader::Draw()
{
    m_img.SetPos(m_pos);
    m_img.Draw();

    DrawRect();

}

void Trader::Collision(Base* b)
{
    if (b->GetType() == eType_Player) {
        //プレイヤーと商人が重なっている（または接近している）かチェック
        if (Base::CollisionRect(this, b)) {
            m_is_near = true;

            //近くにいる状態でZキーが押されたらショップが開閉
            if (PUSH(CInput::eButton1)) {
                m_is_shop_open = !m_is_shop_open;   //trueとfalseを反転
            }
        }
    }
}



static TexAnim _idleMae[] = {
    {0,12},
    {1,12},
};


static TexAnim _idleUshiro[] = {
    {4,12},
    {5,12},
};


static TexAnim _idleYoko[] = {
    {8,12},
    {9,12},
};

TexAnimData Trader::_anim_data[] = {
    ANIMDATA(_idleMae),
    ANIMDATA(_idleUshiro),
    ANIMDATA(_idleYoko),
   

};