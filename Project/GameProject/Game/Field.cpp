#include "Field.h"

// 迷路のマップデータ（0:空, 1:床, 2~6:壁, 7~8:移動口）
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
    {2,3,3,3,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,2,3,3,8,3,3,2,2,2,2,2},
};

static int mazeData4[MAP_HEIGHT][MAP_WIDTH] = {
    {2,2,2,2,2,4,4,4,4,4,2,2,2,2,2},
    {2,1,1,1,1,1,1,1,1,1,1,1,1,1,2},
    {10,1,1,1,1,1,1,1,1,1,1,1,1,1,9},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,5,1,1,1,1,1,6,2,2,2,2},
    {2,2,2,2,2,3,3,3,3,3,2,2,2,2,2},
};

int g_currentStage = 1;

// コンストラクタ
Field::Field() : Base(eType_Field) {
    m_img = COPY_RESOURCE("MapTip", CImage);
}

void Field::Update() {
    // 空っぽでOK
}

// 指定位置のマップ番号を取得
int Field::GetTip(const CVector2D& pos) {
    int col = (int)(pos.x / MAP_TIP_SIZE);
    if (col < 0) col = 0;
    if (col > MAP_WIDTH - 1) col = MAP_WIDTH - 1;

    int row = (int)(pos.y / MAP_TIP_SIZE);
    if (row < 0) row = 0;
    if (row > MAP_HEIGHT - 1) row = MAP_HEIGHT - 1;

    // ステージ番号に合わせて配列を自動選択！
    if (g_currentStage == 1)      return mazeData1[row][col];
    else if (g_currentStage == 2) return mazeData2[row][col];
    else if (g_currentStage == 3) return mazeData3[row][col];
    return 1;
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
    for (int i = 0; i < MAP_HEIGHT; i++) {     // 行 (Y)
        for (int j = 0; j < MAP_WIDTH; j++) {  // 列 (X)

            int t = 0;
            if (g_currentStage == 1)      t = mazeData1[i][j];
            else if (g_currentStage == 2) t = mazeData2[i][j];
            else if (g_currentStage == 3) t = mazeData3[i][j];

            if (t == 0) continue; // 0(空) は描画スキップ

            // ★ 1行に並んでいるタイルの枚数（5か6に合わせて書き換えてね！）
            int TILES_PER_ROW = 6;

            // 2列目以降も正しく切り抜く計算
            int texX = (t % TILES_PER_ROW) * 32;
            int texY = (t / TILES_PER_ROW) * 32;

            m_img.SetRect(texX, texY, texX + 32, texY + 32);

            // 画面上での表示サイズと位置をセット
            m_img.SetSize(MAP_TIP_SIZE, MAP_TIP_SIZE);
            m_img.SetPos(CVector2D(j * MAP_TIP_SIZE, i * MAP_TIP_SIZE));

            m_img.Draw();
        }
    }
}