#ifndef HOOK_FLYING_H
#define HOOK_FLYING_H

#include "state.h"

class CHookFlying: public CState{
public:
	void Execute(CHook*hook, bool UseInput);
	~CHookFlying();
	static CHookFlying* getInstance();

private:
	CHookFlying();
	CHookFlying(const CHookFlying &);
	CHookFlying& operator=(const CHookFlying &);
};

#endif
