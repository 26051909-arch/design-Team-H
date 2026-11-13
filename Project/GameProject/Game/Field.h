#pragma once
#include "Base/Base.h"

// マップのサイズ設定（横15マス、縦8マス、1マス128px）
#define MAP_WIDTH    15
#define MAP_HEIGHT   8
#define MAP_TIP_SIZE 128

class Field : public Base {
private:
    CImage m_mapTip;                     // マップチップ画像オブジェクト
    int stage[MAP_HEIGHT][MAP_WIDTH];    // マップ配置データ（0:床, 1以上:壁など）

public:
    // コンストラクタ（初期化）
    Field();

    // 毎フレーム呼ばれる更新・描画関数
    void Update() override;
    void Draw() override;

    // 当たり判定用関数
    int CollisionPoint(const CVector2D& pos);
    int CollisionRect(const CVector2D& pos, const CRect& rect);
};