#ifndef STATE_H
#define STATE_H

class CHook;

//A state in a finite deterministic automaton.
class CState
{
public:
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
