//
// Created by nicola on 05/10/17.
//

#ifndef MULTICAMNET_EPANECHNIKOV_KERNEL_H
#define MULTICAMNET_EPANECHNIKOV_KERNEL_H


#include <string>
#include <iostream>
#include "kernel.h"

class EpanechnikovKernel : public Kernel {
    double bandwidth;
public:
    explicit EpanechnikovKernel(double bandwidth);

    double K(std::pair<double, double> s, const std::vector<std::pair<double, double>> &data) override;

    void declare() {
#ifdef ENABLE_DEBUG
        std::cout << "Initialising Epanechnikov Kernel" << std::endl;
#endif
    }

};


#endif //MULTICAMNET_EPANECHNIKOV_KERNEL_H
