#pragma once
#include "Base/Base.h"

// マップのサイズ設定（横20マス、縦15マス、1マス32px）
#define MAP_WIDTH    20
#define MAP_HEIGHT   15
#define MAP_TIP_SIZE 32

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