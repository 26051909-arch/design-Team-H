#include"Player.h"
#include"Field.h"




Player::Player(const CVector2D& pos, int direction) : Base(eType_Player)
{
    m_pos_old = m_pos;
    m_is_ground = true;
    m_rect = CRect(-16, -32, 16, 0);
    m_img = COPY_RESOURCE("Player", CImage);
    //座標設定
    m_pos_old = m_pos = pos;
    //サイズ設定
    m_img.SetSize(128, 128);
    //中心位置設定
    m_img.SetCenter(64, 123);


    //体力の初期化(最大100)
    m_hp = 100.0f;
    //通常状態へ
    m_state = eState_Idle;
    //向きの初期化
    m_direction = direction;

    //反転フラグの初期化
    m_flip = (m_direction == eDirection_Right);

    //髪の数
    m_hear_count = 0;

    m_anim = -1;

    //向きに応じたアニメーション設定
    switch (m_direction)
    {
    case eDirection_Mae:
        m_img.ChangeAnimation(eAnimIdleMae);
        break;

    case eDirection_Ushiro:
        m_img.ChangeAnimation(eAnimIdleUshiro);
        break;

    case eDirection_Left:
    case eDirection_Right:
        m_img.ChangeAnimation(eAnimIdleYoko);
        break;
    }

}


void Player::StateIdle()
{
    //歩き・走りの移動速度
    const float walk_speed = 4.0f;
    const float run_speed = 7.0f;


    //Shiftを押している間は走る
    bool run_flag = HOLD(CInput::eShift);

    float move_speed = run_flag ? run_speed : walk_speed;

    //移動量
    float move_x = 0.0f;
    float move_y = 0.0f;

    //移動フラグ
    bool move_flag = false;


    //右移動
    if (HOLD(CInput::eRight))
        move_x += 1.0f;


    //左移動
    if (HOLD(CInput::eLeft))
        move_x -= 1.0f;

    //上移動
    if (HOLD(CInput::eUp))
        move_y -= 1.0f;

    //下移動
    if (HOLD(CInput::eDown))
        move_y += 1.0f;


    //移動しているか確認
    move_flag = (move_x != 0.0f || move_y != 0.0f);

    if (move_flag)
    {
        //斜め移動の速度調整
        if (move_x != 0.0f && move_y != 0.0f)
        {
            move_x *= 0.7071f;
            move_y *= 0.7071f;
        }


        //座標更新
        m_pos.x += move_x * move_speed;
        m_pos.y += move_y * move_speed;

        //向きの変更
        if (move_y < 0)
            m_direction = eDirection_Ushiro;
        else if (move_y > 0)
            m_direction = eDirection_Mae;
        else if (move_x < 0)
            m_direction = eDirection_Left;
        else if (move_x > 0)
            m_direction = eDirection_Right;

        //反転フラグ
        m_flip = (m_direction == eDirection_Right);

    }

    //アニメーションの選択
    int anim = -1;

    if (move_flag)
    {
        switch (m_direction)
        {
        case eDirection_Mae:
            anim = run_flag ? eAnimRunMae : eAnimWarkMae;
            break;

        case eDirection_Ushiro:
            anim = run_flag ? eAnimRunUshiro : eAnimWarkUshiro;
            break;

        case eDirection_Left:
        case eDirection_Right:
            anim = run_flag ? eAnimRunYoko : eAnimWarkYoko;
            break;
        }
    }
    else
    {
        switch (m_direction)
        {
        case eDirection_Mae:
            anim = eAnimIdleMae;
            break;

        case eDirection_Ushiro:
            anim = eAnimIdleUshiro;
            break;

        case eDirection_Left:
        case eDirection_Right:
            anim = eAnimIdleYoko;
            break;
        }
    }

    if (anim != -1)
        SetPlayerAnimation(anim);

    //攻撃
    if (PUSH(CInput::eMouseL)) {
        //攻撃状態へ移行
        m_state = eState_Attack;
    }


    //防御
    if (PUSH(CInput::eMouseR)) {
        //防御状態へ移行
        m_state = eState_Gard;
    }

}

void Player::StateAttack()
{
    int anim = -1;


    //向きに応じた攻撃アニメーション
    switch (m_direction)
    {
    case eDirection_Mae:
        anim = eAnimAttackMae;
        break;

    case eDirection_Ushiro:
        anim = eAnimAttackUshiro;
        break;

    case eDirection_Left:
    case eDirection_Right:
        anim = eAnimAttackYoko;
        break;
    }

    //攻撃アニメーションの開始
    if (m_anim != anim && anim != -1)
    {
        m_anim = anim;
        m_img.ChangeAnimation(anim, false);
    }

    //アニメーションが終了したら通常状態へ
    if (m_img.CheckAnimationEnd())
    {
        m_state = eState_Idle;
    }

}



void Player::StateGard()
{
    //右クリックを離したら通常状態へ戻る
    if (!HOLD(CInput::eMouseR))
    {
        m_state = eState_Idle;
        return;
    }


    int anim = -1;

    //向きに応じた攻撃アニメーション
    switch (m_direction)
    {
    case eDirection_Mae:
        anim = eAnimGardMae;
        break;

    case eDirection_Ushiro:
        anim = eAnimGardUshiro;
        break;

    case eDirection_Left:
    case eDirection_Right:
        anim = eAnimGardYoko;
        break;
    }

    //アニメーション変更
    if (anim != -1)
        SetPlayerAnimation(anim);

}


void Player::StateDamage()
{

    int anim = -1;

    //向きに応じたダメージアニメーション
    switch (m_direction)
    {
    case eDirection_Mae:
        anim = eAnimDamageMae;
        break;

    case eDirection_Ushiro:
        anim = eAnimDamageUshiro;
        break;

    case eDirection_Left:
    case eDirection_Right:
        anim = eAnimDamageYoko;
        break;
    }


    //攻撃アニメーションの開始
    if (m_anim != anim && anim != -1)
    {
        m_anim = anim;
        m_img.ChangeAnimation(anim, false);
    }

    //アニメーションが終了したら通常状態へ
    if (m_img.CheckAnimationEnd())
    {
        m_state = eState_Idle;
    }

}


void Player::StateDown()
{

    int anim = -1;

    //向きに応じた死亡アニメーション
    switch (m_direction)
    {
    case eDirection_Mae:
        anim = eAnimDeathMae;
        break;

    case eDirection_Ushiro:
        anim = eAnimDeathUshiro;
        break;

    case eDirection_Left:
    case eDirection_Right:
        anim = eAnimDeathYoko;
        break;
    }


    //死亡アニメーションの開始
    if (m_anim != anim && anim != -1)
    {
        m_anim = anim;
        m_img.ChangeAnimation(anim, false);
    }


    //アニメーション終了確認
    if (m_img.CheckAnimationEnd()) {
        SetKill();
    }
}



void Player::Update()
{
    m_pos_old = m_pos;
    //ダメージアニメーション確認用
    // 1秒につき5ずつ体力を減らす（60FPSの場合）
    if (m_state != eState_Death)
    {
        m_hp = max(m_hp - 5.0f / 60.0f, 0.0f);

        if (m_hp <= 0)
        {
            m_state = eState_Death;
        }
    }

    m_pos_old = m_pos;

    switch (m_state)
    {
    case eState_Idle:
        StateIdle();
        break;

    case eState_Attack:
        StateAttack();
        break;

    case eState_Gard:
        StateGard();
        break;

    case eState_Damage:
        StateDamage();
        break;

    case eState_Death:
        StateDown();
        break;
    }


    //アニメーション更新
    m_img.UpdateAnimation();


}




void Player::Draw()
{
    m_img.SetPos(m_pos);
    m_img.SetFlipH(m_flip);

    m_img.Draw();
}




void Player::Collision(Base* b)
{
    switch (b->m_type) {
    case eType_Map: // フィールド（マップ）との当たり判定
        if (Field* m = dynamic_cast<Field*>(b)) {
            int t;

            // ① 横（X軸）の当たり判定：壁にぶつかったら横座標だけ元に戻す
            t = m->CollisionRect(CVector2D(m_pos.x, m_pos_old.y), m_rect);
            if (t != 0) {
                m_pos.x = m_pos_old.x;
            }

            // ② 縦（Y軸）の当たり判定：床や天井にぶつかったら縦座標だけ元に戻す
            t = m->CollisionRect(CVector2D(m_pos_old.x, m_pos.y), m_rect);
            if (t != 0) {
                m_pos.y = m_pos_old.y;
                m_vec.y = 0;          // 落下速度をストップ
                m_is_ground = true;   // 着地フラグをON
            }
        }
        break;
    }
}


void Player::TakeDamage(float damage)
{
    //HP減少
    m_hp = max(m_hp - damage, 0);
    if (m_hp <= 0) {
        m_state = eState_Death;
    }
    else {
        m_state = eState_Damage;
    }

}



void Player::SetPlayerAnimation(int anim)
{
    //現在と同じアニメーションなら何もしない
    if (m_anim == anim)
        return;

    m_anim = anim;

    //アニメーション変更
    m_img.ChangeAnimation(anim);
}




static TexAnim _idleMae[] = {
    {0,5},
    {1,5},
    {2,5},
};

static TexAnim _idleUshiro[] = {
    {6,5},
    {7,5},
    {8,5},
};

static TexAnim _idleYoko[] = {
    {12,5},
    {13,5},
    {14,5},
};

static TexAnim _warkMae[] = {
    {18,6},
    {19,6},
    {20,6},
    {21,6},
};

static TexAnim _runMae[] = {
    {18,4},
    {19,4},
    {20,4},
    {21,4},
};

static TexAnim _warkUshiro[] = {
    {24,6},
    {25,6},
    {26,6},
    {27,6},
};

static TexAnim _runUshiro[] = {
    {24,4},
    {25,4},
    {26,4},
    {27,4},
};

static TexAnim _warkYoko[] = {
    {30,6},
    {31,6},
    {32,6},
    {33,6},
};

static TexAnim _runYoko[] = {
    {30,4},
    {31,4},
    {32,4},
    {33,4},
};

static TexAnim _attackMae[] = {
    {36,3},
    {37,3},
    {38,3},
    {39,3},
    {40,3},
    {41,3},
};

static TexAnim _attackUshiro[] = {
    {42,3},
    {43,3},
    {44,3},
    {45,3},
    {46,3},
    {47,3},
};

static TexAnim _attackYoko[] = {
    {48,3},
    {49,3},
    {50,3},
    {51,3},
    {52,3},
    {53,3},
};

static TexAnim _gardMae[] = {
   // {54,3},
   // {55,3},
    {56,3},
   // {57,3},
   // {58,3},
};

static TexAnim _gardUshiro[] = {
    //{60,3},
    //{61,3},
    {62,3},
    //{63,3},
    //{64,3},
};

static TexAnim _gardYoko[] = {
    //{66,3},
    //{67,3},
    {68,3},
    //{69,3},
    //{70,3},
};

static TexAnim _damageMae[] = {
    {72,2},
    {73,2},
    {74,2},
    {75,2},
    {76,2},
};

static TexAnim _damageUshiro[] = {
    {78,2},
    {79,2},
    {80,2},
    {81,2},
    {82,2},
};

static TexAnim _damageYoko[] = {
    {84,2},
    {85,2},
    {86,2},
    {87,2},
    {88,2},
};

static TexAnim _deathMae[] = {
    {90,6},
    {91,6},
    {92,6},
    {93,6},
    {94,6},
    {95,6},
};

static TexAnim _deathUshiro[] = {
    {96,6},
    {97,6},
    {98,6},
    {99,6},
    {100,6},
    {101,6},
};

static TexAnim _deathYoko[] = {
    {102,6},
    {103,6},
    {104,6},
    {105,6},
    {106,6},
    {107,6},
};


TexAnimData Player::_anim_data[] = {
    ANIMDATA(_idleMae),	
    ANIMDATA(_idleUshiro),
    ANIMDATA(_idleYoko),
    ANIMDATA(_warkMae),
    ANIMDATA(_runMae),
    ANIMDATA(_warkUshiro),
    ANIMDATA(_runUshiro),
    ANIMDATA(_warkYoko),
    ANIMDATA(_runYoko),	
    ANIMDATA(_attackMae),
    ANIMDATA(_attackUshiro),
    ANIMDATA(_attackYoko),
    ANIMDATA(_gardMae),
    ANIMDATA(_gardUshiro),
    ANIMDATA(_gardYoko),
    ANIMDATA(_damageMae),
    ANIMDATA(_damageUshiro),
    ANIMDATA(_damageYoko),
    ANIMDATA(_deathMae),
    ANIMDATA(_deathUshiro),
    ANIMDATA(_deathYoko),
	
};