#include <iostream>
#include <vector>
#include <tuple>
#include "billmaterialpoint.h"
#include "billvector.h"

namespace bill{

bill::BillIntegrator HollyWood = [](std::pair<bill::vector,bill::vector> PhasePoint0, std::pair<bill::vector,bill::vector> PhasePointM, bill::vector Force, double step){

bill::vector x = std::get<0>( PhasePoint0 );
bill::vector v = std::get<1>( PhasePoint0 );

x+=step*v;

return std::pair<bill::vector,bill::vector>(x,v);

};

bill::BillIntegrator Euler = [](std::pair<bill::vector,bill::vector> PhasePoint0, std::pair<bill::vector,bill::vector> PhasePointM, bill::vector Force, double step){

bill::vector x = std::get<0>( PhasePoint0 );
bill::vector v = std::get<1>( PhasePoint0 );

x+=step*v;
v+=step*Force;

return std::pair<bill::vector,bill::vector>(x,v);

};

bill::BillIntegrator ModEuler = [](std::pair<bill::vector,bill::vector> PhasePoint0, std::pair<bill::vector,bill::vector> PhasePointM, bill::vector Force, double step){

bill::vector x = std::get<0>( PhasePoint0 );
bill::vector v = std::get<1>( PhasePoint0 );

v+=step*Force;
x+=step*v;

return std::pair<bill::vector,bill::vector>(x,v);

};

bill::BillIntegrator Verlet = [](std::pair<bill::vector,bill::vector> PhasePoint0, std::pair<bill::vector,bill::vector> PhasePointM, bill::vector Force, double step){

bill::vector x0 = std::get<0>( PhasePoint0 );
bill::vector xm = std::get<0>( PhasePointM );

bill::vector xp = 2*x0 - xm + Force*step*step;
bill::vector vp = (xp-xm)/(2.*step);

return std::pair<bill::vector,bill::vector>(xp,vp);

};
} // end of namespace bill
