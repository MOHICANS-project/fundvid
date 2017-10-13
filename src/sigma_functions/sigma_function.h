//
// Created by nicola on 03/10/17.
//

#ifndef MULTICAMNET_SIGMA_FUNCTION_H
#define MULTICAMNET_SIGMA_FUNCTION_H

class SigmaFunction {

public:
    virtual ~SigmaFunction() = default;

    virtual double getMinSigma()=0;

    virtual double evaluateSigma(double x)=0;

};

#endif //MULTICAMNET_SIGMA_FUNCTION_H
