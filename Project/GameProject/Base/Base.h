#pragma once
enum {
    eType_Field,
    eType_Map,
    eType_Door,
    eType_AreaChange,
    eType_Player,
    eType_Enemy,
    eType_Goal,
    eType_Player_Attack,
    eType_Enemy_Attack,
    eType_Effect,
    eType_UI,
    eType_Scene,
};

//重力加速度
#define GRAVITY (9.8f/20)
class Base {
public:
    int m_type;         //オブジェクトの種類
    CVector2D m_pos;    //座標
    CVector2D m_pos_old;    //座標
    float m_rad;        //当たり判定：円半径
    CVector2D m_vec;    //移動ベクトル
    CRect   m_rect;     //矩形
    bool m_kill;        //削除フラグ
	
    static CVector2D m_scroll;          //スクロール値
    static std::list<Base*> m_list;     //オブジェクトリスト
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Base(int type);
    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~Base();
    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update();
    /// <summary>
    /// 描画処理
    /// </summary>
    virtual void Draw();
    /// <summary>
    /// オブジェクト削除
    /// </summary>
    void SetKill() { m_kill = true; }
    /// <summary>
    /// オブジェクトの種類を取得
    /// </summary>
    /// <returns></returns>
    int GetType() { return m_type; }
    /// <summary>
    /// 当たり判定検証
    /// </summary>
    /// <param name="b">判定対象</param>
    virtual void Collision(Base* b);
    /// <summary>
    /// 円同士の当たり判定
    /// </summary>
    /// <param name="b1">対象1</param>
    /// <param name="b2">対象2</param>
    /// <returns></returns>
    static bool CollisionCircle(Base* b1, Base* b2);

    /// <summary>
    /// 矩形同士の当たり判定
    /// </summary>
    /// <param name="b1">対象1</param>
    /// <param name="b2">対象2</param>
    /// <returns></returns>
    static bool CollisionRect(Base* b1, Base* b2);
    /// <summary>
    /// 当たり判定矩形のデバッグ表示
    /// </summary>
    void DrawRect();
    /// <summary>
    /// 全てのオブジェクトの更新
    /// </summary>
    static void UpdateAll();
    /// <summary>
    /// 全てのオブジェクトの描画
    /// </summary>
    static void DrawAll();
    /// <summary>
    /// 全てのオブジェクト同士の組み合わせで当たり判定検証
    /// </summary>
    static void CollisionAll();
    /// <summary>
    /// 全てのオブジェクトの削除チェック
    /// </summary>
    static void CheckKillAll();

    /// <summary>
    /// オブジェクトをリストへ追加
    /// </summary>
    /// <param name="b">追加オブジェクト</param>
    static void Add(Base* b);
    /// <summary>
    /// 指定オブジェクトの削除
    /// </summary>
    /// <param name="mask">削除対象オブジェクトのマスク</param>
    static void Kill(unsigned long mask);
    /// <summary>
    /// 全てのオブジェクト削除
    /// </summary>
    static void KillAll();
    /// <summary>
    /// オブジェクトの探索、取得
    /// </summary>
    /// <param name="type">探索対象の種類</param>
    /// <returns>最初に発見した対象種類のオブジェクトのポインタ</returns>
    static Base* FindObject(int type);

    /// <summary>
    /// オブジェクトの探索、取得(複数)
    /// </summary>
    /// <param name="type">探索対象の種類</param>
    /// <returns>対象種類のオブジェク配列</returns>
    static std::vector<Base*> FindObjects(int type);

    /// <summary>
    /// スクロール値から画面上での座標を計算
    /// </summary>
    /// <param name="pos">変換対象座標</param>
    /// <returns>画面上の位置</returns>
    static CVector2D GetScreenPos(const CVector2D& pos);


};
