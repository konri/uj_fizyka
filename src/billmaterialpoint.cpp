#include "../headers/billmaterialpoint.h"
#include "../headers/billvector.h"

bill::vector bill::BillMaterialPoint::Force(){
  return bill::vector({0.,0.,0.});
}	

void bill::BillMaterialPoint::CalculateMove(bill::vector& force){
  future = algorithm(present,past,force,step);
}

void bill::BillMaterialPoint::Move(){
  past = present;
  present = future;	
}

bill::BillMaterialPoint::BillMaterialPoint(bill::BillIntegrator algorithm, bill::vector position, bill::vector velocity, double mass, bill::vector color, double step){
  this->algorithm=algorithm;
  this->step=step;

  past    = std::pair<bill::vector,bill::vector>(position-velocity*step,{0,0,0});
  present = std::pair<bill::vector,bill::vector>(position,velocity);
  future  = std::pair<bill::vector,bill::vector>({0,0,0},{0,0,0});

  this->mass = mass;
  this->color = color;
  radius = 0.1;
  ENABLE_SIMULATION=true;
}

bill::BillMaterialPoint::~BillMaterialPoint(){}

void bill::BillMaterialPoint::Draw(){
  bill::GLaux::drawBall(std::get<0>(present),color,radius);
}

bill::vector bill::BillMaterialPoint::xm(){
  return std::get<0>(past);
}
bill::vector bill::BillMaterialPoint::vm(){
  return std::get<1>(past);
}
bill::vector bill::BillMaterialPoint::past_position(){
  return xm();
}
bill::vector bill::BillMaterialPoint::past_velocity(){
  return vm();
}
bill::vector bill::BillMaterialPoint::x(){
  return std::get<0>(present);
}
bill::vector bill::BillMaterialPoint::v(){
  return std::get<1>(present);
}
bill::vector bill::BillMaterialPoint::position(){
  return x();
}
bill::vector bill::BillMaterialPoint::velocity(){
  return v();
}
bill::vector bill::BillMaterialPoint::xp(){
  return std::get<0>(future);
}
bill::vector bill::BillMaterialPoint::vp(){
  return std::get<1>(future);
}
bill::vector bill::BillMaterialPoint::future_position(){
  return xp();
}
bill::vector bill::BillMaterialPoint::future_velocity(){
  return vp();
}

double bill::BillMaterialPoint::m(){
  return mass;
}

void bill::BillMaterialPoint::set_position(bill::vector new_x){
  x(new_x);
}
void bill::BillMaterialPoint::x(bill::vector new_x){
  std::get<0>(present)=new_x;
  past = std::pair<bill::vector,bill::vector>(std::get<0>(present)-std::get<1>(present)*step,std::get<1>(present));
}
void bill::BillMaterialPoint::set_velocity(bill::vector new_v){
  v(new_v);
}
void bill::BillMaterialPoint::v(bill::vector new_v){
  std::get<1>(present)=new_v;
  past = std::pair<bill::vector,bill::vector>(std::get<0>(present)-std::get<1>(present)*step,std::get<1>(present));
}
void bill::BillMaterialPoint::set_future_position(bill::vector new_x){
  xp(new_x);
}
void bill::BillMaterialPoint::xp(bill::vector new_x){
  std::get<0>(future)=new_x;
}
void bill::BillMaterialPoint::set_future_velocity(bill::vector new_v){
  vp(new_v);
}
void bill::BillMaterialPoint::vp(bill::vector new_v){
  std::get<1>(future)=new_v;
}

void bill::BillMaterialPoint::disable(){
  ENABLE_SIMULATION=false;
}
void bill::BillMaterialPoint::enable(){
  ENABLE_SIMULATION=true;
}
bool bill::BillMaterialPoint::should_run(){
  return ENABLE_SIMULATION;
}
//*************************************************************************

void bill::BillSetOfPoints::AddPoint(std::shared_ptr<BillMaterialPoint> p){
	  points.push_back(p);
}

void bill::BillSetOfPoints::AddPoint(BillMaterialPoint* p){
	  points.push_back(std::shared_ptr<BillMaterialPoint>(p));
}

std::shared_ptr<bill::BillMaterialPoint>  bill::BillSetOfPoints::operator[](size_t n){
  if(n >= points.size()){
    std::cerr<<("bill::BillSetOfPoints::operator[]: there is no field to access!\n");
    exit(-1);
  }
  else
    return points[n]; 
}

void bill::BillSetOfPoints::Go(){
  Prepare();

  for(auto& point : points)
    if(point->should_run()){
      bill::vector force = point->Force()/point->m();
      point->CalculateMove(force);
    }

  Act();

  for(auto& point : points)
    if(point->should_run())
      point->Move();

  Clear();  
}

void bill::BillSetOfPoints::ResetToCenterMass(){
  bill::vector CMx({0,0,0});
  bill::vector CMv({0,0,0});
  double CMm=0.;

  for(auto& point : points){
    CMx+=point->m()*point->x();
    CMv+=point->m()*point->v();
    CMm+=point->m();
  }

  CMx/=CMm;
  CMv/=CMm;

  for(auto& point : points){
    point->x(point->x()-CMx);
    point->v(point->v()-CMv);
  }
}

void bill::BillSetOfPoints::Draw(){
  for(auto& point : points)
    point->Draw();	
}

bill::BillSetOfPoints::~BillSetOfPoints(){
  for(auto& point : points)
	point.reset();	
}
