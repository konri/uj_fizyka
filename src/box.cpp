#include <box.h>

box::box(bill::BillRBIntegrator algorithm, double _a, double _b, double _c, bill::vector position, bill::vector velocity, bill::quaternion rotation, bill::vector angular):bill::BillRigidBody(algorithm,position,velocity,rotation,angular){
 a = _a;
 b = _b;
 c = _c;


 colors.push_back(bill::vector({ 0.95686275,  0.34117647,  0.03921569}));
 colors.push_back(bill::vector({ 0.95686275,  0.34117647,  0.03921569}));
 colors.push_back(bill::vector({ 0.56470588,  0.6627451,   0.78039216}));
 colors.push_back(bill::vector({ 0.56470588,  0.6627451,   0.78039216}));
 colors.push_back(bill::vector({ 0.95294118,  0.75294118,  0.78431373}));
 colors.push_back(bill::vector({ 0.95294118,  0.75294118,  0.78431373}));

 radius = sqrt(0.25*(a*a+b*b+c*c));

}

void box::rotatevector(bill::vector& v){
  std::get<2>(present).rotateMe(v);
}

void box::rotatedcoordinates(bill::vector& i, bill::vector& j, bill::vector& k){
 i= bill::vector({1.,0.,0.});
 j= bill::vector({0.,1.,0.});
 k= bill::vector({0.,0.,1.});

 rotatevector(i);
 rotatevector(j);
 rotatevector(k);
}

void box::vertices(std::vector<bill::vector>& vers){
 vers.clear();

 bill::vector i,j,k;
 rotatedcoordinates(i,j,k); 

 bill::vector x = std::get<0>(present);

 vers.push_back(x+0.5*(-a*i+b*j+c*k));
 vers.push_back(x+0.5*( a*i+b*j+c*k));
 vers.push_back(x+0.5*( a*i+b*j-c*k));
 vers.push_back(x+0.5*(-a*i+b*j-c*k));
 vers.push_back(x+0.5*(-a*i-b*j+c*k));
 vers.push_back(x+0.5*( a*i-b*j+c*k));
 vers.push_back(x+0.5*( a*i-b*j-c*k));
 vers.push_back(x+0.5*(-a*i-b*j-c*k));
}

void box::Draw(){
  std::vector<bill::vector> points;
  vertices(points);

  bill::GLaux::drawBox(points,colors,1.);
}

double box::get_size(const unsigned int i) const{
  switch(i){
    case 0:  return a;
    case 1:  return b;
    case 2:  return c;
    default: return -1.;
  }
} //zwraca wymiar prostopadłościaniu w kierunku: 0:x, 1:y, 2:z 


bill::vector box::get_versor(const unsigned int i){
  bill::vector result({0.,0.,0.});
  result[i] = 1.;
  rotatevector(result);
  return result;
} // zwraca wersor tworzący wierzchołek: 0:x, 1:y, 2:z 


void box::get_vertices(std::vector<bill::vector>& vers){
  vertices(vers);
} // zwraca std::vector wektorów do wierzchołków

