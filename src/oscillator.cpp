#include <oscillator.h>

bill::vector oscillator::spring_force(std::shared_ptr<oscillator> neighbor, double spring_length) {
    bill::vector force({0, 0, 0});

    if (neighbor == nullptr) {
        return force;
    }

    const bill::vector subPosition = neighbor->position() - position();
    const bill::vector subVelocity = neighbor->velocity() - velocity();

    const double normSubPosition = bill::vector::norm(subPosition);
    const bill::vector subPositionUnit = subPosition / normSubPosition; //wektor jednostkowy

    const bill::vector f = -kd * (subVelocity * subPositionUnit) * subPositionUnit;
    force = ks * (subPosition / normSubPosition) * (normSubPosition - spring_length) - f; //    k*(rp/norm_rp)*(norm_rp - l);

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

    ForceTot += -0.1*v() + mass * g * bill::vector({0., -1., 0.});

    return ForceTot;
}

oscillator::oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position,
                       bill::vector velocity, double mass, bill::vector color, double step) :
        bill::BillMaterialPoint(algorithm, position, velocity, mass, color, step) {

    this->ks = k;
    this->kd = 0.12; //tlumienie sprezystosci w zależności od predkosci ruchu punktow materialnych
    this->g = 0.005; // stała grawitacyjna
    this->l = l;
    this->right = nullptr;
    this->left = nullptr;
    this->bottom = nullptr;
    this->top = nullptr;
    this->top_left = nullptr;
    this->top_right = nullptr;
    this->bottom_left = nullptr;
    this->bottom_right = nullptr;
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
