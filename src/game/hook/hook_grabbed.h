#ifndef HOOK_GRABBED_H
#define HOOK_GRABBED_H

#include "state.h"

class CHookGrabbed: public CState{
public:
	void Execute(CHook*hook, bool UseInput);
	~CHookGrabbed();
	static CHookGrabbed* getInstance();

private:
	CHookGrabbed();
	CHookGrabbed(const CHookGrabbed &);
	CHookGrabbed& operator=(const CHookGrabbed &);
};

#endif
