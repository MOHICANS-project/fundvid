//
// Created by nicola on 03/10/17.
//

#ifndef MULTICAMNET_SIGMA_SIGMOID_FUNCTION_H
#define MULTICAMNET_SIGMA_SIGMOID_FUNCTION_H

#include "sigma_function.h"

class SigmaSigmoidFunction : public SigmaFunction {

    double sigma_low;
    double sigma_high;
    double cross_point;
    double steepness;

    double mirroredSigmoid(double x);

public:
    SigmaSigmoidFunction(double sigma_low, double sigma_high, double cross_point, double steepness);

    ~SigmaSigmoidFunction() override;

    double evaluateSigma(double x) override;

    double getMinSigma() override {
        return sigma_low;
    }
};


#endif //MULTICAMNET_SIGMA_SIGMOID_FUNCTION_H
