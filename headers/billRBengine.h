#ifndef BILLRBENGINE_H
#define BILLRBENGINE_H

#include <iostream>
#include <cmath>
#include "billrigidbody.h"

namespace bill{

class BillRBEngine{
private:
  BillSetOfRigidBodies* set;
  unsigned int starttime;
  unsigned int now;
  double timestep;

public:

  BillRBEngine();
  BillRBEngine(BillSetOfRigidBodies& set);
  void step();
}; //end of class BillRBEngine

} //end of namespace bill
#endif
