//
// Created by nicola on 04/10/17.
//

#ifndef MULTICAMNET_DENSITY_ESTIMATOR_H
#define MULTICAMNET_DENSITY_ESTIMATOR_H

#include <vector>

class DensityEstimator {
public:
    virtual void initEstimator(std::vector<std::pair<double, double>> &pointCloud)=0;

    virtual double estimateDensity(std::pair<double, double> s)=0;
};

#endif //MULTICAMNET_DENSITY_ESTIMATOR_H
