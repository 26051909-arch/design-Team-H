#pragma once
#include"Base/Base.h"

class Trader : public Base {
private:
	//アニメーションの種類
	enum {
		eAnimIdleMae,
		eAnimIdleUshiro,
		eAnimIdleYoko,
	};

	CImage m_img;
	bool m_is_near;	//プレイヤーが近くにいるかどうかのフラグ
	bool m_is_shop_open;	//売買画面が開いているかどうかのフラグ


public:
	Trader(const CVector2D& pos);

	void Update();
	void Draw();
	void Collision(Base* b);

	bool IsShopOpen()const {
		return m_is_shop_open;
	}

	static TexAnimData _anim_data[];
};