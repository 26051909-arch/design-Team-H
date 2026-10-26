#include "Base.h"
std::list<Base*> Base::m_list;
//スクロール値の定義
CVector2D Base::m_scroll(0, 0);

Base::Base(int type) : m_type(type),m_pos(0, 0),m_rad(0), m_kill(false)
{
    //コンストラクタ
    //オブジェクト生成時に呼び出される
    //初期化処理を記述

    Base::Add(this);
}

Base::~Base()
{

}

void Base::Update()
{
    //更新処理
    //派生先で記述
}

void Base::Draw()
{
    //描画処理
    //派生先で記述
}
void Base::Collision(Base* b)
{
    //当たり判定の検証
    //派生先で記述
}

bool Base::CollisionCircle(Base* b1, Base* b2)
{
    //b2からb1へのベクトル
    CVector2D v = b1->m_pos - b2->m_pos;
    //ベクトルの大きさ。b1-b2の距離。
    float l = v.Length();
    //b1-b2の距離と、b1,b2の半径の合算値との比較で衝突検証
    if (l < b1->m_rad + b2->m_rad) {
        return true;
    }
    return false;
}

bool Base::CollisionRect(Base* b1, Base* b2)
{
    //b1の矩形
    CRect rect1 = CRect(
        b1->m_pos.x + b1->m_rect.m_left,
        b1->m_pos.y + b1->m_rect.m_top,
        b1->m_pos.x + b1->m_rect.m_right,
        b1->m_pos.y + b1->m_rect.m_bottom);
    //b2の矩形
    CRect rect2 = CRect(
        b2->m_pos.x + b2->m_rect.m_left,
        b2->m_pos.y + b2->m_rect.m_top,
        b2->m_pos.x + b2->m_rect.m_right,
        b2->m_pos.y + b2->m_rect.m_bottom);

    //矩形同士の判定
    if (rect1.m_left <= rect2.m_right && rect1.m_right >= rect2.m_left &&
        rect1.m_top <= rect2.m_bottom && rect1.m_bottom >= rect2.m_top)
        return true;

    return false;
}
void Base::DrawRect()
{
    //デバッグ用　矩形の表示
    CRect rect = CRect(
        m_pos.x + m_rect.m_left,
        m_pos.y + m_rect.m_top,
        m_pos.x + m_rect.m_right,
        m_pos.y + m_rect.m_bottom);
    Utility::DrawQuad(
        CVector2D(rect.m_left, rect.m_top) - m_scroll,
        CVector2D(rect.m_width, rect.m_height),
        CVector4D(1, 0, 0, 0.5f));
}
void Base::CollisionAll()
{
    //リストの先頭から末尾まで繰り返し
    for (auto it1 = m_list.begin(); it1 != m_list.end(); it1++) {
        // it2はit1の「次の要素」からスタート
        for (auto it2 = std::next(it1); it2 != m_list.end(); it2++) {
            (*it1)->Collision(*it2);
            (*it2)->Collision(*it1);
        }
    }
}


void Base::UpdateAll()
{
    //リスト内全ての要素の更新
    for (auto& b : m_list) {
        b->Update();
    }
}

void Base::DrawAll()
{
    //リスト内全ての要素の描画
    for (auto& b : m_list) {
        b->Draw();
    }
}

void Base::Kill(unsigned long mask)
{
    for (auto& b : m_list) {
        if ((1 << b->m_type) & (mask))
            b->SetKill();
    }
}
void Base::CheckKillAll()
{
    //先頭から末尾まで繰り返して
    //削除対象をリストから削除
    auto it = m_list.begin();
    auto last = m_list.end();
    while (it != last) {
        if ((*it)->m_kill) {
            delete (*it);
            //リストから除外
            it = m_list.erase(it);
        } else {
            //次へ
            it++;
        }
    }
}


void Base::Add(Base* b)
{
    //Type順によるソート
    for (auto it = m_list.begin(); it != m_list.end();it++) {
        if ((*it)->m_type > b->m_type) {
            m_list.insert(it, b);
            return;
        }
    }
    //リストの末尾へ追加
    m_list.push_back(b);
}

void Base::KillAll()
{
    for (auto& b : m_list)
        b->SetKill();
}

Base* Base::FindObject(int type)
{
    //リストの先頭から末尾まで繰り返し
    //typeが一致するオブジェクトを返却する
    for (auto& b : m_list) {
        if (b->m_type == type) {
            //見つけたオブジェクトの返却
            return b;
        }
    }
    return nullptr;
}

std::vector<Base*> Base::FindObjects(int type)
{
    std::vector<Base*> ret;
    //リストの先頭から末尾まで繰り返し
    //typeが一致するオブジェクト配列を返却する
    for (auto& b : m_list) {
        if (b->m_type == type) {
            //見つけたオブジェクトを追加
            ret.push_back(b);
        }
    }
    return ret;
}
CVector2D Base::GetScreenPos(const CVector2D& pos)
{
    //座標-スクロール値=画面上での位置
    return pos - m_scroll;

}