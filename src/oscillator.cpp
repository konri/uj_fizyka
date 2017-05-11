#include <oscillator.h>

bill::vector oscillator::spring_force(std::shared_ptr<oscillator> neighbor) {
    bill::vector force({0, 0, 0});

    if (neighbor == nullptr) {
        return force;
    }

    const double b = 0.02;

//    bill::vector r12 = position() - neighbor->position();
//    const double d = bill::vector::norm(r12);
//    const bill::vector r12_unit = r12 / d;
//
////    const bill::vector v12 = neighbor->velocity() - velocity();
//
////    bill::vector f = -2 * b * (v12 * r12_unit) * r12_unit;
//
//    force = -k * (d - l) * r12_unit;


    const bill::vector pos = position();
    const bill::vector pointPos = neighbor->position();
    const bill::vector subPos = pointPos - pos;
    const double normSubPos = bill::vector::norm(subPos);
    const bill::vector subPosUnit = subPos / normSubPos; // wektor jednostkowy
    const bill::vector subVelocity = neighbor->velocity() - velocity();
    const bill::vector f = -b * (subVelocity * subPosUnit) * subPosUnit;
    force = k * (subPos / normSubPos) * (normSubPos - l) - f;


    return force;


//    vec::subtract<3>(point1->r, point2->r, r12);
//    vec::subtract<3>(point1->v, point2->v, v12);

//    double v12_dot_r12 = vec::dot_product<3>(v12, r12);
//    double d = vec::norm<3>(r12);
//    double f = -m_ks * (d - m_length) - m_kd * v12_dot_r12;

//    double F[3];
//    vec::scalar_mul<3>(r12, f, F);

}

bill::vector oscillator::Force() {
    bill::vector ForceTot({0, 0, 0});

    ForceTot += spring_force(left);
    ForceTot += spring_force(right);
    ForceTot += spring_force(top);
    ForceTot += spring_force(bottom);

    ForceTot += mass * g * bill::vector({0., -1., 0.});

    return ForceTot;
}

oscillator::oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position,
                       bill::vector velocity, double mass, bill::vector color, double step) :
        bill::BillMaterialPoint(algorithm, position, velocity, mass, color, step) {

    this->k = k;
    this->kp = k;
    this->kb = k;
    this->g = 0.005;
    this->l = l;
    this->right = nullptr;
    this->left = nullptr;
    this->bottom = nullptr;
    this->top = nullptr;
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
