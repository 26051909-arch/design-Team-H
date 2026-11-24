#include "Field.h"
#include"Trader.h"

// 迷路のマップデータ（0:空, 1:床, 2~6:壁, 7~10:移動口）
static int mazeData1[MAP_HEIGHT][MAP_WIDTH] = {
    {2,4,4,4,4,4,4,7,4,4,4,4,4,4,2},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {2,3,3,3,3,3,3,3,3,3,3,3,3,3,2},
};

static int mazeData2[MAP_HEIGHT][MAP_WIDTH] = {
    {2,2,2,2,2,4,4,7,4,4,2,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,2,3,3,8,3,3,2,2,2,2,2},
};

static int mazeData3[MAP_HEIGHT][MAP_WIDTH] = {
    {2,4,4,4,4,4,4,4,4,4,2,2,2,2,2},
    {5,1,1,1,1,1,1,1,1,1,6,2,2,2,2},
    {10,1,1,1,1,1,1,1,1,1,6,2,2,2,2},
    {2,3,3,3,11,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,2,3,3,8,3,3,2,2,2,2,2},
};

static int mazeData4[MAP_HEIGHT][MAP_WIDTH] = {
    {2,2,2,2,2,4,4,4,4,4,4,4,4,4,2},
    {2,2,2,2,5,1,1,1,1,1,1,1,1,1,6},
    {2,2,2,2,5,1,1,1,1,1,1,1,1,1,9},
    {2,2,2,2,5,1,1,1,1,1,12,3,3,3,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,2,3,3,8,3,3,2,2,2,2,2},
};

static int mazeData5[MAP_HEIGHT][MAP_WIDTH] = {
    {2,4,4,4,4,4,4,7,4,4,4,4,4,4,2},
    {5,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
    {10,1,1,1,1,1,1,1,1,1,1,1,1,1,9},
    {2,3,3,3,11,1,1,1,1,1,12,3,3,3,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,2,3,3,3,3,3,2,2,2,2,2},
};

int g_currentStage = 1;

int GetMapTile(int stage, int row, int col) {
    if (row < 0 || row >= MAP_HEIGHT || col < 0 || col >= MAP_WIDTH) return 1;

    switch (stage) {
    case 1: return mazeData1[row][col];
    case 2: return mazeData2[row][col];
    case 3: return mazeData3[row][col];
    case 4: return mazeData4[row][col];
    case 5: return mazeData5[row][col];
    default: return 1;
    }
}

// コンストラクタ
Field::Field() : Base(eType_Field) {
    m_img = COPY_RESOURCE("MapTip", CImage);
}

void Field::Update() {
    
    //ステージ５に入ったら商人を作成
    if (g_currentStage == 5 && !m_stage5CharacterCreated)
    {
        new Trader(CVector2D(960, 540));

        m_stage5CharacterCreated = true;
    }
}

int Field::GetTip(const CVector2D& pos) {
    int col = (int)(pos.x / MAP_TIP_SIZE);
    int row = (int)(pos.y / MAP_TIP_SIZE);

    return GetMapTile(g_currentStage, row, col);
}
// 当たり判定チェック（1, 7以上 はすり抜けOK）
int Field::CollisionPoint(const CVector2D& pos) {
    int t = GetTip(pos);

    if (t == 1 || t == 7 || t == 8 || t == 9 || t == 10) {
        return 0; // すり抜けOK
    }

    return 1; // それ以外の壁は押し戻す
}

int Field::CollisionRect(const CVector2D& pos, const CRect& rect) {
    CRect r = CRect(
        pos.x + rect.m_left,
        pos.y + rect.m_top,
        pos.x + rect.m_right,
        pos.y + rect.m_bottom
    );
    int t;
    t = CollisionPoint(CVector2D(r.m_left, r.m_top));
    if (t != 0) return t;
    t = CollisionPoint(CVector2D(r.m_right, r.m_top));
    if (t != 0) return t;
    t = CollisionPoint(CVector2D(r.m_left, r.m_bottom));
    if (t != 0) return t;
    t = CollisionPoint(CVector2D(r.m_right, r.m_bottom));
    if (t != 0) return t;

    return 0;
}

void Field::Draw() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {

            int t = GetMapTile(g_currentStage, i, j);
            if (t == 0) continue;

            int drawTile = t;
            if (t == 9) {
                drawTile = 9; // ドア(9,10)を床の見た目で描画
            }

            int TILES_PER_ROW = 6;
            int texX = (drawTile % TILES_PER_ROW) * 32;
            int texY = (drawTile / TILES_PER_ROW) * 32;

            m_img.SetRect(texX, texY, texX + 32, texY + 32);
            m_img.SetSize(MAP_TIP_SIZE, MAP_TIP_SIZE);
            m_img.SetPos(CVector2D(j * MAP_TIP_SIZE, i * MAP_TIP_SIZE));

            m_img.Draw();
        }
    }
}