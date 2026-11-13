#include "Field.h"

// 迷路のマップデータ（0:床[通り抜けOK], 1:支柱, 2~:壁[通れない]）
static int mazeData[MAP_HEIGHT][MAP_WIDTH] = {
    {2,4,4,4,4,4,4,4,7,4,4,4,4,4,2},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {10,1,1,1,1,1,1,1,1,1,1,1,1,1,9},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {2,3,3,3,3,3,8,3,3,3,3,3,3,3,2},
};

// コンストラクタ：エンジンのBaseへ登録し、画像をセット
Field::Field() : Base(eType_Field) {
    // main.cppのInitで登録した "MapTip" 画像を取得
    m_mapTip = COPY_RESOURCE("MapTip", CImage);
    // マップ配列データをコピー
    memcpy(stage, mazeData, sizeof(mazeData));
}

// 毎フレームの更新処理
void Field::Update() {
}

// 毎フレームの描画処理
void Field::Draw() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            int t = stage[i][j];

            switch (t) {
                // --- 1行目のタイル (Y: 0 ～ 32) ---
            case 0:  m_mapTip.SetRect(32, 0, 32, 32); break; // 床
            case 1:  m_mapTip.SetRect(32, 0, 64, 32); break; // 支柱
            case 2:  m_mapTip.SetRect(64, 0, 96, 32); break; // 壁（上）
            case 3:  m_mapTip.SetRect(96, 0, 128, 32); break; // 壁（下）
            case 4:  m_mapTip.SetRect(128, 0, 160, 32); break; // 壁（右）
            case 5:  m_mapTip.SetRect(160, 0, 192, 32); break; // 壁（左）
                
                // --- 2行目のタイル (Y: 32 ～ 64) ---
            case 6:  m_mapTip.SetRect(0, 32, 32, 64); break; // 左壁
            case 7:  m_mapTip.SetRect(32, 32, 64, 64); break; // 上扉
            case 8:  m_mapTip.SetRect(64, 32, 96, 64); break; // 下扉
            case 9:  m_mapTip.SetRect(96, 32, 128, 64); break; // 右扉
            case 10: m_mapTip.SetRect(128, 32, 160, 64); break; // 左扉

            default:
                m_mapTip.SetRect(0, 0, 32, 32); break;
            }

            m_mapTip.SetSize(MAP_TIP_SIZE, MAP_TIP_SIZE);
            m_mapTip.SetPos(MAP_TIP_SIZE * j - m_scroll.x, MAP_TIP_SIZE * i - m_scroll.y);
            m_mapTip.Draw();
        }
    }
}


// 指定座標のタイル判定（0:床は通り抜けOK、0以外:壁や支柱で通れない）
int Field::CollisionPoint(const CVector2D& pos) {
    int col = (int)(pos.x / MAP_TIP_SIZE);
    if (col < 0) col = 0;
    if (col > MAP_WIDTH - 1) col = MAP_WIDTH - 1;

    int row = (int)(pos.y / MAP_TIP_SIZE);
    if (row < 0) row = 0;
    if (row > MAP_HEIGHT - 1) row = MAP_HEIGHT - 1;

    int t = stage[row][col];

    // 0(床) 以外は全て「壁/支柱」として当たり判定を返す
    if (t != 0) {
        return t;
    }
    return 0; // 床はすり抜けOK
}

// 矩形（キャラクターの四隅）での当たり判定
int Field::CollisionRect(const CVector2D& pos, const CRect& rect) {
    CRect r = CRect(
        pos.x + rect.m_left,
        pos.y + rect.m_top,
        pos.x + rect.m_right,
        pos.y + rect.m_bottom
    );

    int t;
    t = CollisionPoint(CVector2D(r.m_left, r.m_top));     if (t != 0) return t;
    t = CollisionPoint(CVector2D(r.m_right, r.m_top));    if (t != 0) return t;
    t = CollisionPoint(CVector2D(r.m_left, r.m_bottom));  if (t != 0) return t;
    t = CollisionPoint(CVector2D(r.m_right, r.m_bottom)); if (t != 0) return t;

    return 0;
}