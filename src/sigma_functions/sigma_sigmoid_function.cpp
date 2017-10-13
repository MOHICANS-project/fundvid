//
// Created by nicola on 03/10/17.
//

#include "sigma_sigmoid_function.h"
#include <cmath>

SigmaSigmoidFunction::SigmaSigmoidFunction(double sigma_low, double sigma_high, double cross_point, double steepness)
        : sigma_low(sigma_low), sigma_high(sigma_high), cross_point(cross_point), steepness(steepness) {}


double SigmaSigmoidFunction::mirroredSigmoid(double x) {
    return 1.0 / (1 + exp(steepness * (x - cross_point / 2)));
}


double SigmaSigmoidFunction::evaluateSigma(double x) {
    return sigma_low + (sigma_high - sigma_low) * mirroredSigmoid(x);
}


SigmaSigmoidFunction::~SigmaSigmoidFunction() = default;
