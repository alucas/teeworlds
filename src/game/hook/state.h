#ifndef STATE_H
#define STATE_H

class CHook;

//A state in a finite deterministic automaton.
class CState
{
public:
	//UseInput seems to be usefull only in hook_iddle;
	virtual void Execute(CHook*, bool UseInput)=0;
	~CState(){}

protected:
	CState(){}

private:
	//Singleton
	CState(const CState &);
	CState& operator=(const CState &);
};

#endif
