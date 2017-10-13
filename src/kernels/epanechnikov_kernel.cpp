//
// Created by nicola on 05/10/17.
//

#include <cmath>
#include <vector>
#include "epanechnikov_kernel.h"

EpanechnikovKernel::EpanechnikovKernel(double bandwidth) : bandwidth(bandwidth) {}

double EpanechnikovKernel::K(std::pair<double, double> s, const std::vector<std::pair<double, double>> &data) {
    double scale = (2.0 / (M_PI * bandwidth * bandwidth));
    double sum = 0;
    for (auto &&datum : data) {
        double norm = (datum.first - s.first) * (datum.first - s.first) +
                      (datum.second - s.second) * (datum.second - s.second);
        norm /= (bandwidth * bandwidth);
        sum += norm < 1 ? (1 - norm) : 0;
    }
    return scale * sum;
}



