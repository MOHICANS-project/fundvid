//
// Created by nicola on 05/10/17.
//

#ifndef MULTICAMNET_HISTOGRAM_KERNEL_H
#define MULTICAMNET_HISTOGRAM_KERNEL_H


#include <vector>
#include "kernel.h"

class HistogramKernel : public Kernel {
    double bandwidth;
public:
    explicit HistogramKernel(double bandwidth);

    double K(std::pair<double, double> s, const std::vector<std::pair<double, double>> &data) override;

    void declare() {
#ifdef ENABLE_DEBUG
        std::cout << "Initialising Histogram Kernel" << std::endl;
#endif
    }
};


#endif //MULTICAMNET_HISTOGRAM_KERNEL_H
