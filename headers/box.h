#ifndef BOX_H
#define BOX_H

#include "billrigidbody.h"
#include "billvector.h"

class box : public bill::BillRigidBody{
protected:
	double a,b,c; // długości boków
 	std::vector<bill::vector> colors;

	void rotatevector(bill::vector& v); // rotuje zadany wektor korzystając z obrotu bryły
	void rotatedcoordinates(bill::vector& i, bill::vector& j, bill::vector& k); // podaje rotowane koordynaty
	void vertices(std::vector<bill::vector>& vers); // podaje bezwzględne położenia wierzchołków
public:
	box(bill::BillRBIntegrator algorithm, double _a = .2, double _b = .2, double _c = .2, bill::vector position = bill::vector({0.,0.,0.}),bill::vector velocity = bill::vector({0.,0.,0.}), bill::quaternion rotation = bill::quaternion({0.,0.,1.,0.}),bill::vector angular=bill::vector({0.2,0.2,0.}));
	virtual ~box(){};

	void Draw();

	double get_size(const unsigned int i) const; //zwraca wymiar prostopadłościaniu w kierunku: 0:x, 1:y, 2:z
	bill::vector get_versor(const unsigned int i); // zwraca wersor tworzący wierzchołek: 0:x, 1:y, 2:z
	void get_vertices(std::vector<bill::vector>& vers); // zwraca std::vector wektorów do wierzchołków

}; // end of class box

#endif //end of box.h
