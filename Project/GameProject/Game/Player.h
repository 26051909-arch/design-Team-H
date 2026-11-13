#pragma once
#include"Base/Base.h"


class Player : public Base {
private:

	//アニメーションの種類
	enum {
		eAnimIdleMae = 0,
		eAnimIdleUshiro,
		eAnimIdleYoko,
		eAnimWarkMae,
		eAnimRunMae,
		eAnimWarkUshiro,
		eAnimRunUshiro,
		eAnimWarkYoko,
		eAnimRunYoko,
		eAnimAttackMae,
		eAnimAttackUshiro,
		eAnimAttackYoko,
		eAnimGardMae,
		eAnimGardUshiro,
		eAnimGardYoko,
		eAnimDamageMae,
		eAnimDamageUshiro,
		eAnimDamageYoko,
		eAnimDeathMae,
		eAnimDeathUshiro,
		eAnimDeathYoko,
	};


	//状態
	enum {
		eState_Idle,
		eState_Attack,
		eState_Gard,
		eState_Damage,
		eState_Death,
	};


	//各状態での挙動
	void StateIdle();
	void StateAttack();
	void StateGard();
	void StateDamage();
	void StateDown();



	//状態変数
	int m_state;
	//向き
	int m_direction;
	//反転フラグ
	bool m_flip;

	//体力
	float m_hp;

	CImage m_img;
	bool m_is_ground;

public:


	//プレイヤーの向き
	enum {
		eDirection_Mae,
		eDirection_Ushiro,
		eDirection_Left,
		eDirection_Right,
	};



	Player(const CVector2D& pos, int direction);
	void Update();
	void Draw();
	void Collision(Base* b);
	//アニメーション変更
	void SetPlayerAnimation(int anim);


	//ダメージ処理
	void TakeDamage(float damage);

	float GetHP() {
		return m_hp;
	}

	int GetHear() {
		return m_hear_count;
	}

	//現在のアニメーション
	int m_anim;

	//ゲット髪
	int m_hear_count;

	//過去の位置
	CVector2D m_pos_old;

	static TexAnimData _anim_data[];
};
