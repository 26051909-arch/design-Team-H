#pragma once
#include"Base/Base.h"


class GhostT : public Base
{
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
		eAnimDamageMae,
		eAnimDamageUshiro,
		eAnimDamageYoko,
		eAnimDeathMae,
		eAnimDeathMaeF,
		eAnimDeathUshiro,
		eAnimDeathUshiroF,
		eAnimDeathYoko,
		eAnimDeathYokoF,

	};


	//状態
	enum {
		eState_Idle,
		eState_Attack,
		eState_Damage,
		eState_Death,
	};


	//各状態での挙動
	void StateIdle();
	void StateAttack();
	void StateDamage();
	void StateDeath();


};
