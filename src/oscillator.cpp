#include <oscillator.h>

bill::vector oscillator::spring_force(std::shared_ptr<oscillator> neighbor, double length) {
    bill::vector force({0, 0, 0});

    if (neighbor == nullptr) {
        return force;
    }

    const double b = 0.8;

    const bill::vector pos = position();
    const bill::vector pointPos = neighbor->position();
    const bill::vector subPos = pos - pointPos;
    const double normSubPos = bill::vector::norm(subPos);
    const bill::vector subPosUnit = subPos / normSubPos;
    const bill::vector subVelocity = neighbor->velocity() - velocity();
    const bill::vector f =  b * (subVelocity * subPosUnit) * subPosUnit;
    force = -k * subPosUnit * (normSubPos - length) + f;

    return force;
}

bill::vector oscillator::Force() {
    bill::vector ForceTot({0, 0, 0});

    ForceTot += spring_force(left, l);
    ForceTot += spring_force(right, l);
    ForceTot += spring_force(top, l);
    ForceTot += spring_force(bottom, l);

    double len_diag = ::sqrt(2.0) * l;
    ForceTot += spring_force(top_left, len_diag);
    ForceTot += spring_force(top_right, len_diag);
    ForceTot += spring_force(bottom_left, len_diag);
    ForceTot += spring_force(bottom_right, len_diag);

    ForceTot += mass * g * bill::vector({0., -1., 0.});

    return ForceTot;
}

oscillator::oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position,
                       bill::vector velocity, double mass, bill::vector color, double step) :
        bill::BillMaterialPoint(algorithm, position, velocity, mass, color, step) {

    this->k = k;
    this->kp = k;
    this->kb = k;
    this->g = 0.01;
    this->l = l;
    this->right = nullptr;
    this->left = nullptr;
    this->bottom = nullptr;
    this->top = nullptr;
    top_left = nullptr;
    top_right = nullptr;
    bottom_left = nullptr;
    bottom_right = nullptr;
}

void oscillator::set_right(std::shared_ptr<oscillator> r) {
    right = r;
}

void oscillator::set_left(std::shared_ptr<oscillator> l) {
    left = l;
}

void oscillator::set_bottom(std::shared_ptr<oscillator> point) {
    bottom = point;
}

void oscillator::set_top(std::shared_ptr<oscillator> point) {
    top = point;
}

void oscillator::set_top_left(std::shared_ptr<oscillator> point) {
    top_left = point;
}

void oscillator::set_top_right(std::shared_ptr<oscillator> point) {
    top_right = point;
}

void oscillator::set_bottom_left(std::shared_ptr<oscillator> point) {
    bottom_left = point;
}

void oscillator::set_bottom_right(std::shared_ptr<oscillator> point) {
    bottom_right = point;
}
