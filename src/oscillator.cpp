#include "oscillator.h"

bill::vector oscillator::Force() {
    bill::vector x0 = position(); // pozycja punktu

    bill::vector ForceLeft({0, 0, 0}); // siła z lewej
    bill::vector ForceRight({0, 0, 0}); // siła z prawej

    if (left != NULL) {
        bill::vector xl = left->position(); // pozycja punktu z lewej
        bill::vector rl = xl - x0;
        double norm_rl = bill::vector::norm(rl);
        ForceLeft = k * (rl / norm_rl) * (norm_rl - l);
    }

    if (right != NULL) {
        bill::vector xp = right->position(); // pozycja punktu z prawej
        bill::vector rp = xp - x0;
        double norm_rp = bill::vector::norm(rp);
        ForceRight = k * (rp / norm_rp) * (norm_rp - l);
    }

    if (second_left != NULL) {
        bill::vector xl = second_left->position(); // pozycja punktu z lewej
        bill::vector rl = xl - x0;
        double norm_rl = bill::vector::norm(rl);
        ForceLeft += kp * (rl / norm_rl) * (norm_rl - 2 * l);
    }

    if (second_right != NULL) {
        bill::vector xp = second_right->position(); // pozycja punktu z prawej
        bill::vector rp = xp - x0;
        double norm_rp = bill::vector::norm(rp);
        ForceRight += kp * (rp / norm_rp) * (norm_rp - 2 * l);
    }

    if (third_left != NULL) {
        bill::vector xl = third_left->position(); // pozycja punktu z lewej
        bill::vector rl = xl - x0;
        double norm_rl = bill::vector::norm(rl);
        ForceLeft += kb * (rl / norm_rl) * (norm_rl - 3 * l);
    }

    if (third_right != NULL) {
        bill::vector xp = third_right->position(); // pozycja punktu z prawej
        bill::vector rp = xp - x0;
        double norm_rp = bill::vector::norm(rp);
        ForceRight += kb * (rp / norm_rp) * (norm_rp - 3 * l);
    }

    return (ForceLeft + ForceRight - 0.1 * v() + mass * g * bill::vector({0., -1., 0.}));
}

oscillator::oscillator(bill::BillIntegrator algorithm, double k, double l, bill::vector position, bill::vector velocity,
                       double mass, bill::vector color, double step) : bill::BillMaterialPoint(algorithm, position,
                                                                                               velocity, mass, color,
                                                                                               step) {
    this->k = k;
    this->kp = k;
    this->kb = k;
    this->g = 0.005;
    this->l = l;
    this->right = NULL;
    this->left = NULL;
    this->second_right = NULL;
    this->second_left = NULL;
    this->third_right = NULL;
    this->third_left = NULL;
}

void oscillator::set_right(std::shared_ptr<oscillator> r) {
    right = r;
}

void oscillator::set_left(std::shared_ptr<oscillator> l) {
    left = l;
}

void oscillator::set_2nd_right(std::shared_ptr<oscillator> r) {
    second_right = r;
}

void oscillator::set_2nd_left(std::shared_ptr<oscillator> l) {
    second_left = l;
}

void oscillator::set_3rd_right(std::shared_ptr<oscillator> r) {
    third_right = r;
}

void oscillator::set_3rd_left(std::shared_ptr<oscillator> l) {
    third_left = l;
}

