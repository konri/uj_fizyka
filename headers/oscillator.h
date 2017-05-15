#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include <memory>
#include <billmaterialpoint.h>
#include <billvector.h>

class oscillator : public bill::BillMaterialPoint {
protected:
    double k;   // współczynnik sprężystości
    double kp;  // współczynnik sprężystości 2go sąsiada
    double kb;  // współczynnik sprężystości 3go sąsiada
    double l;   // odległość spoczynkowa
    double g;   // stała grawitacji

    std::shared_ptr<oscillator> left;
    std::shared_ptr<oscillator> right;
    std::shared_ptr<oscillator> top;
    std::shared_ptr<oscillator> bottom;
    std::shared_ptr<oscillator> top_left;
    std::shared_ptr<oscillator> top_right;
    std::shared_ptr<oscillator> bottom_left;
    std::shared_ptr<oscillator> bottom_right;

    bill::vector spring_force(std::shared_ptr<oscillator> neighbor, double length);

public:
    oscillator(bill::BillIntegrator algorithm, double k, double l,
               bill::vector position = bill::vector({0., 0., 0.}),
               bill::vector velocity = bill::vector({0., 0., 0.}), double mass = 1.0,
               bill::vector color = bill::vector({1.0, 0.0, 0.0}), double step = 0.2);

    virtual ~oscillator() {}

    void set_right(std::shared_ptr<oscillator> r);
    void set_left(std::shared_ptr<oscillator> l);
    void set_bottom(std::shared_ptr<oscillator> point);
    void set_top(std::shared_ptr<oscillator> point);

    void set_top_left(std::shared_ptr<oscillator> point);
    void set_top_right(std::shared_ptr<oscillator> point);
    void set_bottom_left(std::shared_ptr<oscillator> point);
    void set_bottom_right(std::shared_ptr<oscillator> point);

    virtual bill::vector Force();
}; //end of class oscillator

#endif
