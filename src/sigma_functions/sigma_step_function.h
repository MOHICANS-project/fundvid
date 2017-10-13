//
// Created by nicola on 03/10/17.
//

#ifndef MULTICAMNET_SIGMA_STEP_FUNCTION_H
#define MULTICAMNET_SIGMA_STEP_FUNCTION_H

#include "sigma_function.h"

class SigmaStepFunction : public SigmaFunction {

    double sigma_low;
    double sigma_high;
    double threshold;

public:
    SigmaStepFunction(double sigma_low, double sigma_high, double threshold);

    ~SigmaStepFunction() override;

    double evaluateSigma(double x) override;

    double getMinSigma() override {
        return sigma_low;
    }
};


#endif //MULTICAMNET_SIGMA_STEP_FUNCTION_H
