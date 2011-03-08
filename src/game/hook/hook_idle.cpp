#include "hook_idle.h"

CHookIdle::CHookIdle* getInstance(){

  static CHookIdle *singleton;
  return singleton;

} 
