#pragma once
#include"Base/Base.h"


class GhostT : public Base
{
private:
	//アニメーションの種類
	enum {
		eAnimWarkMae = 0,
		eAnimChaseMae,
		eAnimWarkUshiro,
		eAnimChaseUshiro,
		eAnimWarkYoko,
		eAnimChaseYoko,
		//eAnimRunMae,
		//eAnimRunUshiro,
		//eAnimRunYoko,
		eAnimAttackMae,
		eAnimAttackUshiro,
		eAnimAttackYoko,
		/*eAnimDamageMae,
		eAnimDamageUshiro,
		eAnimDamageYoko,*/
		eAnimDeathMae,
		eAnimDeathMaeC,
		eAnimDeathUshiro,
		eAnimDeathUshiroC,
		eAnimDeathYoko,
		eAnimDeathYokoC,

	};


	//状態
	enum {
		eState_Idle,
		eState_Chase,
		eState_Attack,
		eState_Death,
	};


	//各状態での挙動
	void StateIdle();
	void StateChase();
	void StateAttack();
	void StateDeath();

	//状態変数
	int m_state;
	//向き
	int m_direction;
	//反転フラグ
	bool m_flip;

	float  m_sight_len;   // 視界の長さ（例: 250px）
	float  m_sight_height;// 視界の上下幅（例: 40px）

	//体力
	float m_hp;

	CImage m_img;


public:


	//亡霊の向き
	enum {
		eDirection_Mae,
		eDirection_Ushiro,
		eDirection_Left,
		eDirection_Right,
	};



	GhostT(const CVector2D& pos, int direction);
	//視界の判定
	bool CheckSight();
	//視界の描画
	void DrawSight();
	void Update();
	void Draw();
	void Collision(Base* b);
	//アニメーション変更
	void SetPlayerAnimation(int anim);


	//ダメージ処理
	void TakeDamage(float damage);

	//現在のアニメーション
	int m_anim;

	static TexAnimData _anim_data[];

};
