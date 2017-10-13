//
// Created by nicola on 03/10/17.
//

#include "sigma_step_function.h"


SigmaStepFunction::SigmaStepFunction(double sigma_low, double sigma_high, double threshold) : sigma_low(sigma_low),
                                                                                              sigma_high(sigma_high),
                                                                                              threshold(threshold) {}

SigmaStepFunction::~SigmaStepFunction() = default;

double SigmaStepFunction::evaluateSigma(double x) {
    return x < threshold ? sigma_high : sigma_low;
}