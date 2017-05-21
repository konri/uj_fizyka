#include "../headers/billRBengine.h"

bill::BillRBEngine::BillRBEngine(){
  starttime=glutGet(GLUT_ELAPSED_TIME);
  now=starttime;
  set=NULL;
}

bill::BillRBEngine::BillRBEngine(BillSetOfRigidBodies& set){ 
  starttime=glutGet(GLUT_ELAPSED_TIME);
  now=starttime;
  this->set = &set;
  this->timestep=timestep;
}

void bill::BillRBEngine::step(){
  if(set!=NULL){
    now=glutGet(GLUT_ELAPSED_TIME);
    if((now-starttime)>20){
      set->Go();
      starttime=now;
    }
  }
}
