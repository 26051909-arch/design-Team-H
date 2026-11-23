#pragma once
#include"Base/Base.h"


class Ghost : public Base
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
		eAnimDeathUshiro,
		eAnimDeathYoko,
	};


	//状態
	enum {
		eState_Idle,
		eState_Attack,
		eState_Damage,
		eState_Death,
	};

};
