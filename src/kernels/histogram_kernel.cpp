//
// Created by nicola on 05/10/17.
//

#include <cmath>
#include <iostream>
#include "histogram_kernel.h"

HistogramKernel::HistogramKernel(double bandwidth) : bandwidth(bandwidth) {}

double HistogramKernel::K(std::pair<double, double> s, const std::vector<std::pair<double, double>> &data) {
    double scale = (1.0 / (M_PI * bandwidth * bandwidth));
    double sum = 0;
    for (auto &&datum : data) {
        double norm = (datum.first - s.first) * (datum.first - s.first) +
                      (datum.second - s.second) * (datum.second - s.second);
        norm /= (bandwidth * bandwidth);
        sum += norm < 1 ? 1 : 0;
    }
    return scale * sum;
}

