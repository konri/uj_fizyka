#ifndef BILLMATERIALPOINT_H
#define BILLMATERIALPOINT_H

#include <iostream>
#include <tuple>
#include <functional>
#include <vector>
#include <memory>
#include "billvector.h"
#include "billGLfunctions.h"

namespace bill{

using BillIntegrator = std::function<std::pair<vector,vector>(std::pair<vector,vector>,std::pair<vector,vector>,vector,double)>;

class BillMaterialPoint{
protected:
	BillIntegrator algorithm;

	double step;

	std::pair<vector,vector> past; // 0-> position 1-> velocity
	std::pair<vector,vector> present;
	std::pair<vector,vector> future;

	double mass;
	vector color;
	float radius;

	bool ENABLE_SIMULATION;

public:
	BillMaterialPoint(BillIntegrator algorithm, vector position=vector({0.,0.,0.}), vector velocity=vector({0.,0.,0.}), double mass=1.0, vector color=vector({1.0,0.0,0.0}), double step=0.2);
	virtual ~BillMaterialPoint();
	virtual void Draw();

	virtual vector Force();
	void CalculateMove(bill::vector& force);
	void Move();

	double m();

// position and velocity getters
	bill::vector past_position();
	bill::vector xm();
	bill::vector past_velocity();
	bill::vector vm();
	bill::vector position();
	bill::vector x();
	bill::vector velocity();
	bill::vector v();
	bill::vector future_position();
	bill::vector xp();
	bill::vector future_velocity();
	bill::vector vp();

// position and velocity setters
	void set_position(bill::vector new_x);
	void x(bill::vector new_x);
	void set_velocity(bill::vector new_v);
	void v(bill::vector new_v);
	void set_future_position(bill::vector new_x);
	void xp(bill::vector new_x);
	void set_future_velocity(bill::vector new_v);
	void vp(bill::vector new_v);

// disable/enable simulation
	void disable();
	void enable();
	bool should_run();

}; // end of class BillMaterialPoint

class BillSetOfPoints{
protected:
	std::vector<std::shared_ptr<BillMaterialPoint>> points;
	
	virtual void Prepare(){};
	virtual void Act(){};
	virtual void Clear(){};

public:
	BillSetOfPoints(){};
	virtual	~BillSetOfPoints();

	void AddPoint(std::shared_ptr<BillMaterialPoint> p);
	void AddPoint(BillMaterialPoint* p);

	std::shared_ptr<BillMaterialPoint> operator[](size_t n);

	void Go();
	void ResetToCenterMass();

	virtual void Draw();
}; // end of class BillSetOfPoints

} //end namespace bill
#endif
