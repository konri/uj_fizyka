#ifndef BILLENGINE_H
#define BILLENGINE_H

#include <iostream>
#include <cmath>
#include "billmaterialpoint.h"

namespace bill{

class BillEngine{
private:
  BillSetOfPoints* set;
  unsigned int starttime;
  unsigned int now;
  double timestep;

public:

  BillEngine();
  BillEngine(BillSetOfPoints& set);
  void step();
}; //end of class BillEngine
} //end of namespace bill
#endif
