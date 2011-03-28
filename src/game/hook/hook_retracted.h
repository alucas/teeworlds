#ifndef HOOK_RETRACTED_H
#define HOOK_RETRACTED_H

#include "state.h"

class CHookRetracted: public CState{
public:
	void Execute(CHook*hook, bool UseInput);
	~CHookRetracted();
	static CHookRetracted* getInstance();

private:
	CHookRetracted();
	CHookRetracted(const CHookRetracted &);
	CHookRetracted& operator=(const CHookRetracted &);
};

#endif
