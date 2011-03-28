#ifndef STATE_H
#define STATE_H

class CHook;

//A state in a finite deterministic automaton.
class CState
{
public:
	//UseInput is usefull only in hook_iddle.
	virtual void Execute(CHook*, bool UseInput)=0;
	int getState(){return m_HookState; }

protected:
	CState(){}
	~CState(){}
	//This int is used in the network protocol.
	int m_HookState;

private:
	//Because all states are singleton.
	CState(const CState &);
	CState& operator=(const CState &);
};

#endif
