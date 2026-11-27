#include"GhostT.h"
#include"Field.h"
#include"Player.h"


GhostT::GhostT(const CVector2D& pos, int direction) : Base(eType_GhostT)
{
    m_img = COPY_RESOURCE("Ghost", CImage);
    //座標設定
    m_pos_old = m_pos = pos;
    //サイズ設定
    m_img.SetSize(128, 128);
    //中心位置設定
    m_img.SetCenter(64, 123);
   m_rect = CRect(-32, -108, 32, 0);


    //体力の初期化(最大20)
    m_hp = 20.0f;
    //通常状態へ
    m_state = eState_Idle;
    //向きの初期化
    m_direction = direction;


    //視界の設定
    m_sight_len = 250.0f;
    m_sight_height = 80.0f;


    //反転フラグの初期化
    m_flip = (m_direction == eDirection_Left);

    m_anim = -1;

    //向きに応じたアニメーション設定
    switch (m_direction)
    {
    case eDirection_Mae:
        m_img.ChangeAnimation(eAnimWarkMae);
        break;

    case eDirection_Ushiro:
        m_img.ChangeAnimation(eAnimWarkUshiro);
        break;

    case eDirection_Left:
    case eDirection_Right:
        m_img.ChangeAnimation(eAnimWarkYoko);
        break;
    }
}


void GhostT::StateIdle()
{

    //視界にプレイヤーが入った場合
    if (CheckSight())
    {
        m_state = eState_Chase;
        return;
    }


    //歩きの移動速度
    const float walk_speed = 3.0f;

    //移動方向によって速度を変更
    if (m_direction == eDirection_Left)
    {
        m_vec.x = -walk_speed;
    }
    else
    {
        m_vec.x = walk_speed;
    }

    //移動
    m_pos.x += m_vec.x;

}


void GhostT::StateChase()
{

    // 向きに応じてチェイスアニメーションに変更
    switch (m_direction)
    {
    case eDirection_Mae:
        SetPlayerAnimation(eAnimChaseMae);
        break;

    case eDirection_Ushiro:
        SetPlayerAnimation(eAnimChaseUshiro);
        break;

    case eDirection_Left:
    case eDirection_Right:
        SetPlayerAnimation(eAnimChaseYoko);
        break;
    }


    //ここから追跡処理


}


void GhostT::StateAttack()
{

}


void GhostT::StateDeath()
{

}



bool GhostT::CheckSight()
{
    // プレイヤーを取得
    Player* player = dynamic_cast<Player*>(Base::FindObject(eType_Player));

    if (player == nullptr)
        return false;

    // プレイヤーの位置
    float player_x = player->m_pos.x;
    float player_y = player->m_pos.y - 54.0f;

    // 視界の上下範囲
    float top = m_pos.y - 108.0f;
    float bottom = top + m_sight_height;

    // 上下方向の判定
    if (player_y < top || player_y > bottom)
        return false;

    // 右向きの場合
    if (m_direction == eDirection_Right)
    {
        return player_x >= m_pos.x + 32.0f &&
            player_x <= m_pos.x + 32.0f + m_sight_len;
    }

    // 左向きの場合
    if (m_direction == eDirection_Left)
    {
        return player_x <= m_pos.x - 32.0f &&
            player_x >= m_pos.x - 32.0f - m_sight_len;
    }

    return false;
}


void GhostT::DrawSight()
{
    float left;
    float top = m_pos.y - 108.0f;


    //右向きの場合
    if (m_direction == eDirection_Right)
    {
        left = m_pos.x + 32.0f;
    }
    //左向きの場合
    else if (m_direction == eDirection_Left)
    {
        left = m_pos.x - 32.0f - m_sight_len;
    }
    else
    {
        return;
    }


    //赤い視界を描画
    Utility::DrawQuad(
        CVector2D(left, top) - m_scroll,
        CVector2D(m_sight_len, m_sight_height),
        CVector4D(1, 0, 0, 0.25f)
    );

}


void GhostT::Update()
{
    m_pos_old = m_pos;

    switch (m_state)
    {
    case eState_Idle:
        StateIdle();
        break;

    case eState_Chase:
        StateChase();
        break;

    case eState_Attack:
        StateAttack();
        break;

    case eState_Death:
        StateDeath();
        break;
    }


    //アニメーション更新
    m_img.UpdateAnimation();

}


void GhostT::Draw()
{

    //視界を描画
    DrawSight();

    m_img.SetPos(m_pos);
    m_img.SetFlipH(m_flip);

    m_img.Draw();

   // DrawRect();

}


void GhostT::Collision(Base* b)
{
    switch (b->m_type) {
    case eType_Field:
        if (Field* f = dynamic_cast<Field*>(b)) {

            // 足元から体の中央あたりのマップ数字を取得
            int tip = f->GetTip(m_pos + CVector2D(0, -32));
            int t;
            // X軸（横壁）
            t = f->CollisionRect(CVector2D(m_pos.x, m_pos_old.y), m_rect);
            if (t != 0) {

                //元の位置に戻す
                m_pos.x = m_pos_old.x;

                //移動方向を反転
                if (m_direction == eDirection_Right)
                {
                    m_direction = eDirection_Left;
                }
                else {
                    m_direction = eDirection_Right;
                }
              
                // 画像の向き変更
                m_flip = (m_direction == eDirection_Left);

            }

            // Y軸（床・天井）
            t = f->CollisionRect(CVector2D(m_pos_old.x, m_pos.y), m_rect);
            if (t != 0) {
                m_pos.y = m_pos_old.y;
                m_vec.y = 0;

            }
           
        } 
        break;
    }

}




void GhostT::TakeDamage(float damage)
{

    if (m_state == eState_Death)
        return;

    //HP減少
    m_hp = max(m_hp - damage, 0);
    if (m_hp <= 0) {
        m_state = eState_Death;
    }

}



void GhostT::SetPlayerAnimation(int anim)
{

    //現在と同じアニメーションなら何もしない
    if (m_anim == anim)
        return;

    m_anim = anim;

    //アニメーション変更
    m_img.ChangeAnimation(anim);

}




static TexAnim _warkMae[] = {
    {0,10},
    {1,10},
    {2,10},
    {3,10},
};


static TexAnim _chaseMae[] = {
    {7,6},
    {8,6},
    {9,6},
    {10,6},
};

static TexAnim _warkUshiro[] = {
    {14,10},
    {15,10},
    {16,10},
    {17,10},
};

static TexAnim _chaseUshiro[] = {
    {21,6},
    {22,6},
    {23,6},
    {24,6},
};


static TexAnim _warkYoko[] = {
    {28,6},
    {29,6},
    {30,6},
    {31,6},
};


static TexAnim _chaseYoko[] = {
    {35,6},
    {36,6},
    {37,6},
    {38,6},
};


static TexAnim _attackMae[] = {
    {42,4},
    {43,4},
    {44,4},
    {45,4},
    {46,4},
    {47,4},
    {48,4},
};


static TexAnim _attackUshiro[] = {
    {49,4},
    {50,4},
    {51,4},
    {52,4},
    {53,4},
    {54,4},
    {55,4},
};


static TexAnim _attackYoko[] = {
    {56,4},
    {57,4},
    {58,4},
    {59,4},
    {60,4},
    {61,4},
    {62,4},
};


static TexAnim _deathMae[] = {
    {63,5},
    {64,5},
    {65,5},
    {66,5},
    {67,5},
    {68,5},
    {69,5},
};


static TexAnim _deathMaeC[] = {
    {70,5},
    {71,5},
    {72,5},
    {73,5},
    {74,5},
    {75,5},
    {76,5},
};


static TexAnim _deathUshiro[] = {
    {77,5},
    {78,5},
    {79,5},
    {80,5},
    {81,5},
    {82,5},
    {83,5},
};


static TexAnim _deathUshiroC[] = {
    {84,5},
    {85,5},
    {86,5},
    {87,5},
    {88,5},
    {89,5},
    {90,5},
};


static TexAnim _deathYoko[] = {
    {91,5},
    {92,5},
    {93,5},
    {94,5},
    {95,5},
    {96,5},
    {97,5},
};


static TexAnim _deathYokoC[] = {
    {98,5},
    {99,5},
    {100,5},
    {101,5},
    {102,5},
    {103,5},
    {104,5},
};


TexAnimData GhostT::_anim_data[] = {
    ANIMDATA(_warkMae),
    ANIMDATA(_chaseMae),
    ANIMDATA(_warkUshiro),
    ANIMDATA(_chaseUshiro),
    ANIMDATA(_warkYoko),
    ANIMDATA(_chaseYoko),
    ANIMDATA(_attackMae),
    ANIMDATA(_attackUshiro),
    ANIMDATA(_attackYoko),
    ANIMDATA(_deathMae),
    ANIMDATA(_deathMaeC),
    ANIMDATA(_deathUshiro),
    ANIMDATA(_deathUshiroC),
    ANIMDATA(_deathYoko),
    ANIMDATA(_deathYokoC),

};
